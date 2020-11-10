/**
 * IFJ20 Compiler
 * @file scanner.c
 * @authors František Lukeš (xlukes07)
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <math.h>
#include "scanner.h"
#include "../memory.h"
#include "../error.h"

typedef enum state_e {
  STATE_START,
  STATE_IDENTIFIER_KEYWORD,
  STATE_NUM,
  STATE_NUM_ZERO,
  STATE_DECIMAL,
  STATE_BASE,
  STATE_EXP_START,
  STATE_EXP,
  STATE_OPERATOR_ADD,
  STATE_OPERATOR_SUB,
  STATE_OPERATOR_MUL,
  STATE_OPERATOR_OR_COMMENT,
  STATE_OPERATOR_LESS,
  STATE_OPERATOR_MORE,
  STATE_OPERATOR_DECLARE,
  STATE_OPERATOR_ASSIGN,
  STATE_OPERATOR_NOT,
  STATE_OPERATOR_AND,
  STATE_OPERATOR_OR,
  STATE_QUOTATION_MARKS,
  STATE_ESCAPE_SEQUENCE,
  STATE_HEXADECIMAL_ESCAPE_SEQUENCE,
  STATE_LEFT_PARENTHESES,
  STATE_RIGHT_PARENTHESES,
  STATE_LEFT_BRACKET,
  STATE_RIGHT_BRACKET,
  STATE_SEMICOLON,
  STATE_COMMA,
  STATE_SPACE,
  STATE_NEWLINE,
  STATE_EOF,
  STATE_COMMENT,
  STATE_BLOCK_COMMENT,
  STATE_BLOCK_COMMENT_END
} state_e;

static const int BASE_BIN = 2;
static const int BASE_OCT = 8;
static const int BASE_HEX = 16;

token_t* token_ctor(tokenid_e id, token_value_u value) {
  token_t* token = safe_alloc(sizeof(token_t));
  token->id = id;
  token->value = value;
  return token;
}

void token_dtor(token_t* token) {
  guard(token != NULL);
  if (token->id == TOKENID_IDENTIFIER || token->id == TOKENID_STRING_LITERAL) {
    free(token->value.string_value);
  }
  free(token);
}

/**
 * Determines next state from STATE_START
 */
state_e determine_next_state(int c) {
  if (isalpha(c) || c == '_') {  // c is a character (a-z / A-Z) or _
    return STATE_IDENTIFIER_KEYWORD;
  } else if (isdigit(c)) {  // c is a digit
    if (c == '0') {         // c is 0
      return STATE_NUM_ZERO;
    } else {  // c is digit other than 0
      return STATE_NUM;
    }
  }
  switch (c) {
    case '+':
      return STATE_OPERATOR_ADD;
    case '-':
      return STATE_OPERATOR_SUB;
    case '*':
      return STATE_OPERATOR_MUL;
    case '/':  // c is either and operator (/) or start of comment
      return STATE_OPERATOR_OR_COMMENT;
    case '<':
      return STATE_OPERATOR_LESS;
    case '>':
      return STATE_OPERATOR_MORE;
    case ':':  // Start of varible declaration operator (:=)
      return STATE_OPERATOR_DECLARE;
    case '=':  // c is assign operator (=)
      return STATE_OPERATOR_ASSIGN;
    case '!':
      return STATE_OPERATOR_NOT;
    case '&':
      return STATE_OPERATOR_AND;
    case '|':
      return STATE_OPERATOR_OR;
    case '"':
      return STATE_QUOTATION_MARKS;
    case '(':
      return STATE_LEFT_PARENTHESES;
    case ')':
      return STATE_RIGHT_PARENTHESES;
    case '{':
      return STATE_LEFT_BRACKET;
    case '}':
      return STATE_RIGHT_BRACKET;
    case ';':
      return STATE_SEMICOLON;
    case ',':
      return STATE_COMMA;
    case '\n':
      return STATE_NEWLINE;
    case EOF:
      return STATE_EOF;
  }
  if (isspace(c)) {
    return STATE_START;
  }
  exit(1);
}

/**
 * Resizes buffer to fit one more character and inserts character c to the end.
 */
char* append(char c, char* buffer) {
  int buffer_size = buffer == NULL ? 2 : strlen(buffer) + 2;
  buffer = safe_realloc(buffer, buffer_size * sizeof(char));
  buffer[buffer_size - 2] = c;
  buffer[buffer_size - 1] = '\0';
  return buffer;
}

tokenid_e decide_operator(char c) {
  switch (c) {
    case '+':
      return TOKENID_OPERATOR_PLUS;
    case '-':
      return TOKENID_OPERATOR_MINUS;
    case '*':
      return TOKENID_OPERATOR_MUL;
    default:
      exit(ERRCODE_LEXICAL_ERROR);
  }
}

/**
 * Returns number indicating base (2, 8, 16), or 0 if none of the other apply
 */
int determine_base(int c) {
  int base = 0;
  if (c == 'b' || c == 'B') {  // number is binary
    base = BASE_BIN;
  } else if (c == 'o' || c == 'O') {  // number is octal
    base = BASE_OCT;
  } else if (c == 'x' || c == 'X') {  // number is hexadecimal
    base = BASE_HEX;
  }
  return base;
}

/**
 * If string passed as an argument matches a keyword, the keyword's id is
 * returned.
 * Otherwise identifier id is returned.
 */
tokenid_e compare_keywords(char* string) {
  if (strcmp(string, "if") == 0) {
    return TOKENID_KEYWORD_IF;
  } else if (strcmp(string, "for") == 0) {
    return TOKENID_KEYWORD_FOR;
  } else if (strcmp(string, "else") == 0) {
    return TOKENID_KEYWORD_ELSE;
  } else if (strcmp(string, "return") == 0) {
    return TOKENID_KEYWORD_RETURN;
  } else if (strcmp(string, "package") == 0) {
    return TOKENID_KEYWORD_PACKAGE;
  } else if (strcmp(string, "func") == 0) {
    return TOKENID_KEYWORD_FUNC;
  } else if (strcmp(string, "float64") == 0) {
    return TOKENID_KEYWORD_FLOAT64;
  } else if (strcmp(string, "int") == 0) {
    return TOKENID_KEYWORD_INT;
  } else if (strcmp(string, "string") == 0) {
    return TOKENID_KEYWORD_STRING;
  } else if (strcmp(string, "bool") == 0) {
    return TOKENID_KEYWORD_BOOL;
  } else {
    return TOKENID_IDENTIFIER;
  }
}

token_t* get_next_token() {
  int state = STATE_START;
  int c = '\0';
  static int prev = '\0';  // previously read character
  char* buffer = NULL;
  char *temp = NULL; // temporary varaible to store and later determine escape sequence
  int base = 0;
  double num;  // used to store numerical value to free up space in buffer for
               // exponent
  token_value_u value;
  token_t* token;
  while (1) {
    if (prev == '\0') {
      c = getchar();
    } else {
      c = prev;
      prev = '\0';
    }
    switch (state) {
      case STATE_START:
        state = determine_next_state(c);
        if (state == STATE_IDENTIFIER_KEYWORD || state == STATE_NUM) {
          prev = c;
        }
        break;
      case STATE_IDENTIFIER_KEYWORD:
        if ((isalpha(c) != 0) || c == '_' || (isdigit(c) != 0)) {
          buffer = append((char)c, buffer);
        } else {
          prev = c;
          value.string_value = buffer;
          token = token_ctor(compare_keywords(value.string_value), value);
          return token;
        }
        break;
      case STATE_NUM:
        if (isdigit(c) != 0) {  // digit
          buffer = append((char)c, buffer);
        } else if (c == '.') {  // decimal
          buffer = append((char)c, buffer);
          state = STATE_DECIMAL;
        } else if ((c == 'e') || (c == 'E')) {
          state = STATE_EXP_START;
        } else {  // end of num
          prev = c;
          char* pEnd;
          value.int_value = (int64_t)strtoll(buffer, &pEnd, base);
          free(buffer);
          token = token_ctor(TOKENID_NUM, value);
          return token;
        }
        break;
      case STATE_NUM_ZERO:
        // TODO: FORMATING
        if ((base = determine_base(c)) !=
            0) {  // number will be read in different base
          state = STATE_BASE;
        } else if (c == '.') {  // 0. ->decimal
          buffer = append((char)c, buffer);
          state = STATE_DECIMAL;
        } else if (c == 'e' || c == 'E') {
          buffer = append((char)'0', buffer);
          state = STATE_EXP_START;
        } else if (isdigit(c)) {  // other digits after 0 not allowed ->error
          exit(ERRCODE_LEXICAL_ERROR);
        } else {  // just 0
          prev = c;
          value.int_value = 0;
          token = token_ctor(TOKENID_NUM, value);
          return token;
        }
        break;
      case STATE_DECIMAL:
        if (isdigit(c) != 0) {
          // TODO: at least one number must be present after decimal point
          buffer = append((char)c, buffer);
        } else if (c == 'e' || c == 'E') {  // exponent
          state = STATE_EXP_START;
        } else {
          prev = c;
          char* pEnd;
          value.decimal_value = (double)strtod(buffer, &pEnd);
          free(buffer);
          token = token_ctor(TOKENID_NUM_DECIMAL, value);
          return token;
        }
        break;
      case STATE_BASE:
        if (isdigit(c) || c == '_' ||
            (base == BASE_HEX && (toupper(c) >= 'A') && (toupper(c) <= 'F'))) {
          if (c != '_') {
            buffer = append((char)c, buffer);
          }
        } else {
          prev = c;
          char* pEnd;
          value.int_value = (int64_t)strtoll(buffer, &pEnd, base);
          free(buffer);
          token = token_ctor(TOKENID_NUM, value);
          return token;
        }
        break;
      case STATE_EXP_START:;
        char* pEnd;
        num = (double)strtod(buffer, &pEnd);
        free(buffer);
        buffer = NULL;
        // buffer = safe_alloc(1*sizeof(char));
        /*if(c == '+') {
           //do nothing
        }else if (c == '-') {
           buffer = insert_into_buffer((char)c, buffer);
        }else if(isdigit(c) != 0) {
           prev = c; // put number back to be read in the next state
        }else {
           //TODO: throw error
           printf("error");
        }*/
        if (c == '-') {
          buffer = append((char)c, buffer);
        } else if (isdigit(c)) {
          prev = c;  // put number back to be read in the next state
        } else if (c != '+') {
          exit(ERRCODE_LEXICAL_ERROR);
        }
        state = STATE_EXP;
        break;
      case STATE_EXP:
        // TODO: ensure at least one digit
        if (isdigit(c)) {
          buffer = append((char)c, buffer);
        } else {
          if (strlen(buffer) == 0) {  // No digits were read as an exponent
            exit(ERRCODE_LEXICAL_ERROR);
          }
          value.decimal_value = pow(num, (double)strtod(buffer, &pEnd));
          free(buffer);
          token = token_ctor(TOKENID_NUM_DECIMAL, value);
          return token;
        }
        break;
      case STATE_OPERATOR_ADD:
         if (c == '=') {
            token = token_ctor(TOKENID_OPERATOR_ADD_AND_ASSIGN, value);
         } else {
            prev = c;
            token = token_ctor(TOKENID_OPERATOR_ADD, value);
        }
        return token;
      case STATE_OPERATOR_SUB:
         if (c == '=') {
            token = token_ctor(TOKENID_OPERATOR_SUB_AND_ASSIGN, value);
        } else {
            prev = c;
            token = token_ctor(TOKENID_OPERATOR_SUB, value);
        }
          return token;
      case STATE_OPERATOR_MUL:
         if (c == '=') {
            token = token_ctor(TOKENID_OPERATOR_MUL_AND_ASSIGN, value);
         } else {
            prev = c;
            token = token_ctor(TOKENID_OPERATOR_MUL, value);
        }
        return token;
      case STATE_OPERATOR_LESS:
        if (c == '=') {
          token = token_ctor(TOKENID_OPERATOR_LESS_OR_EQUAL, value);
        } else {
          prev = c;
          token = token_ctor(TOKENID_OPERATOR_LESS, value);
        }
        return token;
      case STATE_OPERATOR_MORE:
        if (c == '=') {
          token = token_ctor(TOKENID_OPERATOR_GREATER_OR_EQUAL, value);
        } else {
          prev = c;
          token = token_ctor(TOKENID_OPERATOR_GREATER, value);
        }
        return token;
      case STATE_OPERATOR_OR_COMMENT:
        if (c == '/') {
          state = STATE_COMMENT;
        } else if (c == '*') {
          state = STATE_BLOCK_COMMENT;
        } else {
          prev = c;
          token = token_ctor(TOKENID_OPERATOR_DIV, value);
          return token;
        }
        break;
      case STATE_OPERATOR_DECLARE:
        if (c == '=') {
          token = token_ctor(TOKENID_OPERATOR_DECLARE, value);
          return token;
        } else {
          exit(ERRCODE_LEXICAL_ERROR);
        }
        break;
      case STATE_OPERATOR_ASSIGN:
        prev = c;
        token = token_ctor(TOKENID_OPERATOR_ASSIGN, value);
        return token;
      case STATE_OPERATOR_NOT:
         if (c == '=') {
          token = token_ctor(TOKENID_OPERATOR_NOT_EQUAL, value);
         } else {
          prev = c;
          token = token_ctor(TOKENID_OPERATOR_NOT, value);
         }
         return token;
      case STATE_OPERATOR_AND:
         if(c == '&') {
            token = token_ctor(TOKENID_OPERATOR_AND, value);
         }else {
            exit(ERRCODE_LEXICAL_ERROR);
         }
         return token;
      case STATE_OPERATOR_OR:
         if(c == '|') {
            token = token_ctor(TOKENID_OPERATOR_OR, value);
         }else {
            exit(ERRCODE_LEXICAL_ERROR);
         }
         return token;
      case STATE_QUOTATION_MARKS:
         if(c == '\\') {
            state = STATE_ESCAPE_SEQUENCE;
         }else if (c != '"') {
          buffer = append((char)c, buffer);
        } else {
          value.string_value = buffer;
          token = token_ctor(TOKENID_STRING_LITERAL, value);
          return token;
        }
        break;
      case STATE_ESCAPE_SEQUENCE:
         switch(c) {
            case 'x':
               state = STATE_HEXADECIMAL_ESCAPE_SEQUENCE;
               break;
            case '"':
               buffer = append('"', buffer);
               state = STATE_QUOTATION_MARKS;
               break;
            case 'n':
               buffer = append('\n', buffer);
               state = STATE_QUOTATION_MARKS;
               break;
            case 't':
               buffer = append('\t', buffer);
               state = STATE_QUOTATION_MARKS;
               break;
            case '\\':
               buffer = append('\\', buffer);
               state = STATE_QUOTATION_MARKS;
               break;
            default:
               exit(ERRCODE_LEXICAL_ERROR);
         }
         break;
      case STATE_HEXADECIMAL_ESCAPE_SEQUENCE:
         if(isdigit(c) || ((toupper(c) >= 'A') && (toupper(c) <= 'F'))) {
            temp = append((char)c, temp);
         }else {
            exit(ERRCODE_LEXICAL_ERROR);
         }
         if(strlen(temp) == 2) {
            //temp = prepend('x', temp);
            //temp = prepend('0', temp);
            char *pEnd;
            buffer = append((char) strtol(temp, &pEnd, BASE_HEX), buffer);
            free(temp);
            state = STATE_QUOTATION_MARKS;
         }
         break;
      case STATE_LEFT_PARENTHESES:
        prev = c;
        token = token_ctor(TOKENID_LEFT_PARENTHESES, value);
        return token;
      case STATE_RIGHT_PARENTHESES:
        prev = c;
        token = token_ctor(TOKENID_RIGHT_PARENTHESES, value);
        return token;
      case STATE_LEFT_BRACKET:
        prev = c;
        token = token_ctor(TOKENID_LEFT_BRACKET, value);
        return token;
      case STATE_RIGHT_BRACKET:
        prev = c;
        token = token_ctor(TOKENID_RIGHT_BRACKET, value);
        return token;
      case STATE_SEMICOLON:
        prev = c;
        token = token_ctor(TOKENID_SEMICOLON, value);
        return token;
      case STATE_COMMA:
        prev = c;
        token = token_ctor(TOKENID_COMMA, value);
        return token;
      case STATE_NEWLINE:
        prev = c;
        token = token_ctor(TOKENID_NEWLINE, value);
        return token;
      case STATE_EOF:
        // prev = c;
        token = token_ctor(TOKENID_END_OF_FILE, value);
        return token;
      case STATE_COMMENT:
        if (c == '\n') {
          prev = c;
          state = STATE_START;
        }
        break;
      case STATE_BLOCK_COMMENT:
        if (c == '*') {
          state = STATE_BLOCK_COMMENT_END;
        }
        break;
      case STATE_BLOCK_COMMENT_END:
        if (c == '/') {
          state = STATE_START;
        } else {
          state = STATE_BLOCK_COMMENT;
        }
        break;
      default:
        exit(ERRCODE_LEXICAL_ERROR);
    }
  }
  return NULL;
}
