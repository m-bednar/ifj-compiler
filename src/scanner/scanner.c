/**
 * IFJ20 Compiler
 * @file scanner.c
 * @authors František Lukeš (xlukes07), Martin Bednář (xbedna77)
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>
#include "scanner.h"
#include "../memory.h"
#include "../error.h"

typedef enum state_e {
   STATE_START,
   STATE_IDENTIFIER_KEYWORD,
   STATE_NUM,
   STATE_NUM_UNDERSCORE,
   STATE_NUM_ZERO,
   STATE_DECIMAL,
   STATE_DECIMAL_UNDERSCORE,
   STATE_BASE,
   STATE_BASE_UNDERSCORE,
   STATE_EXP_START,
   STATE_EXP,
   STATE_EXP_UNDERSCORE,
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
   STATE_HEXADECIMAL_ESCAPE_SEQUENCE_START,
   STATE_HEXADECIMAL_ESCAPE_SEQUENCE_END,
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

/**
 * Determines next state from STATE_START
 */
state_e determine_next_state(int c) {
   if (isalpha(c) || c == '_') { // c is a-z, A-Z or _
      return STATE_IDENTIFIER_KEYWORD;
   } else if (isdigit(c)) {
      if (c == '0') {
         return STATE_NUM_ZERO;
      } else {
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
   case '/':
      return STATE_OPERATOR_OR_COMMENT;
   case '<':
      return STATE_OPERATOR_LESS;
   case '>':
      return STATE_OPERATOR_MORE;
   case ':': // Start of varible declaration operator (:=)
      return STATE_OPERATOR_DECLARE;
   case '=':
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
   exit(ERRCODE_LEXICAL_ERROR);
}

/**
 * Resizes str to fit one more character and inserts character c to the end.
 */
char* append(char c, char* str) {
   int str_size = str == NULL ? 2 : strlen(str) + 2;
   str = safe_realloc(str, str_size * sizeof(char));
   str[str_size - 2] = c;
   str[str_size - 1] = '\0';
   return str;
}

/**
 * Returns number indicating base (2, 8, 16), or 0 if none of the other apply
 */
int determine_base(int c) {
   if (c == 'b' || c == 'B') { 
      return BASE_BIN;
   } else if (c == 'o' || c == 'O') {
      return BASE_OCT;
   } else if (c == 'x' || c == 'X') { 
      return BASE_HEX;
   }
   return 0;
}

/**
 * Checks if there are any characters after decimal point
 */
bool are_decimal_numbers_present(char* str) {
   char* pch = strchr(str, '.'); // find "."
   unsigned int pos = pch - str + 1;
   if (pos == strlen(str)) { // "." is at the end of a string
      return false;
   }
   return true;
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
   } else if (strcmp(string, "true") == 0 || strcmp(string, "false") == 0) {
      return TOKENID_BOOL_LITERAL;
   } else {
      return TOKENID_IDENTIFIER;
   }
}

bool str_to_bool(char* str) {
   return strcmp(str, "true") == 0 ? true : false;
}

/**
 * Returns true if char c belongs to given base, otherwise returns false
 */
bool char_belongs_to_base(char c, int base) {
   if (base == BASE_BIN && c >= '0' && c <= '1') {
      return true;
   } else if (base == BASE_OCT && c >= '0' && c <= '7') {
      return true;
   } else if (base == BASE_HEX && (isdigit(c) || (toupper(c) >= 'A' && toupper(c) <= 'F'))) {
      return true;
   }
   return false;
}

token_t* get_next_token() {
   state_e state = STATE_START;
   int c = '\0';
   static int prev = '\0'; // previously read character
   char* buffer = NULL;
   int base = 0;
   token_value_u value;
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
               buffer = append((char) c, buffer);
            } else {
               prev = c;
               tokenid_e id = compare_keywords(buffer);
               if (id == TOKENID_IDENTIFIER) {
                  value.string_value = buffer;
               } else if (id == TOKENID_BOOL_LITERAL) {
                  value.bool_value = str_to_bool(buffer);
                  free(buffer);
               } else {
                  free(buffer);
               }
               return token_ctor(id, value);
            }
            break;
         case STATE_NUM:
            if (isdigit(c) != 0) { // digit
               buffer = append((char) c, buffer);
            } else if (c == '.') { // decimal
               buffer = append((char) c, buffer);
               state = STATE_DECIMAL;
            } else if ((c == 'e') || (c == 'E')) {
               buffer = append((char)c, buffer);
               state = STATE_EXP_START;
            } else if (c == '_') {
               state = STATE_NUM_UNDERSCORE;
            } else { // end of num
               prev = c;
               char* pEnd;
               value.int_value = (int64_t) strtoll(buffer, &pEnd, base);
               free(buffer);
               return token_ctor(TOKENID_NUM, value);
            }
            break;
         case STATE_NUM_UNDERSCORE:
            if(isdigit(c)) {
               prev = c;
               state = STATE_NUM;
            } else if (c == '_') {
               exit(ERRCODE_LEXICAL_ERROR);
            } else { // _ is start of new token
               ungetc(c, stdin);
               prev = '_';
               char* pEnd;
               value.int_value = (int64_t) strtoll(buffer, &pEnd, base);
               free(buffer);
               return token_ctor(TOKENID_NUM, value);
            }
            break;
         case STATE_NUM_ZERO:
            if ((base = determine_base(c)) != 0) { // number will be read in different base
               state = STATE_BASE;
            } else if (c == '.') { // 0. ->decimal
               buffer = append((char) c, buffer);
               state = STATE_DECIMAL;
            } else if (c == 'e' || c == 'E') {
               buffer = append((char)'0', buffer);
               buffer = append((char)c, buffer);
               state = STATE_EXP_START;
            } else if (isdigit(c)) { // other digits after 0 not allowed
               exit(ERRCODE_LEXICAL_ERROR);
            } else { // just 0
               prev = c;
               value.int_value = 0;
               return token_ctor(TOKENID_NUM, value);
            }
            break;
         case STATE_DECIMAL:
            if (isdigit(c)) {
               buffer = append((char) c, buffer);
            } else if (c == 'e' || c == 'E') { // exponent
               if (!are_decimal_numbers_present(buffer)) {
                  exit(ERRCODE_LEXICAL_ERROR);
               }
               buffer = append((char)c, buffer);
               state = STATE_EXP_START;
            } else if (c == '_') {
               if (!isdigit(buffer[strlen(buffer)-1])) {
                  exit(ERRCODE_LEXICAL_ERROR);
               }
               state = STATE_DECIMAL_UNDERSCORE;
            } else {
               if (!are_decimal_numbers_present(buffer)) {
                  exit(ERRCODE_LEXICAL_ERROR);
               }
               prev = c;
               char* pEnd;
               value.decimal_value = (double) strtod(buffer, &pEnd);
               free(buffer);
               return token_ctor(TOKENID_NUM_DECIMAL, value);
            }
            break;
         case STATE_DECIMAL_UNDERSCORE:
            if (isdigit(c)) {
               prev = c;
               state = STATE_DECIMAL;
            } else if (c == '_') {
               exit(ERRCODE_LEXICAL_ERROR);
            } else {
               ungetc(c, stdin);
               prev = '_';
               char* pEnd;
               value.decimal_value = (double) strtod(buffer, &pEnd);
               free(buffer);
               return token_ctor(TOKENID_NUM_DECIMAL, value);
            }
            break;
         case STATE_BASE:
            if (char_belongs_to_base(c, base)) {
               buffer = append((char) c, buffer);
            } else if (c == '_') {
                  if (buffer == NULL) { // underscore at the start of num (after 0x)
                     exit(ERRCODE_LEXICAL_ERROR);
                  }
               state = STATE_BASE_UNDERSCORE;
            } else {
               prev = c;
               char* pEnd;
               value.int_value = (int64_t) strtoll(buffer, & pEnd, base);
               free(buffer);
               return token_ctor(TOKENID_NUM, value);
            }
            break;
         case STATE_BASE_UNDERSCORE:
            if (char_belongs_to_base(c, base)) {
               prev = c;
               state = STATE_BASE;
            } else if (c == '_') {
               exit(ERRCODE_LEXICAL_ERROR);
            } else {
               ungetc(c, stdin); // return char after _ to stream
               prev = '_';
               char* pEnd;
               value.int_value = (int64_t) strtoll(buffer, & pEnd, base);
               free(buffer);
               return token_ctor(TOKENID_NUM, value);
            }
            break;
         case STATE_EXP_START:
            ;
            char* pEnd;
            if (c == '-') {
               buffer = append((char) c, buffer);
            } else if (isdigit(c)) {
               prev = c; // put number back to be read in the next state
            } else if (c != '+') {
               exit(ERRCODE_LEXICAL_ERROR);
            }
            state = STATE_EXP;
            break;
         case STATE_EXP:
            if (isdigit(c)) {
               buffer = append((char) c, buffer);
            } else if (c == '_') {
               if (!isdigit(buffer[strlen(buffer)-1])) {
                  exit(ERRCODE_LEXICAL_ERROR);
               }
               state = STATE_EXP_UNDERSCORE;
            } else {
               if (strlen(buffer) == 0 || (strlen(buffer) == 1 && buffer[0] == '-')) { // No digits were read as an exponent
                  exit(ERRCODE_LEXICAL_ERROR);
               }
               value.decimal_value = strtod(buffer, &pEnd);
               free(buffer);
               return token_ctor(TOKENID_NUM_DECIMAL, value);
            }
            break;
         case STATE_EXP_UNDERSCORE:
            if (isdigit(c)) {
               prev = c;
               state = STATE_EXP;
            } else if (c == '_') {
               exit(ERRCODE_LEXICAL_ERROR);
            } else {
               ungetc(c, stdin); // return char after _ to stream
               prev = '_';
               char* pEnd;
               value.decimal_value = strtod(buffer, &pEnd);
               free(buffer);
               return token_ctor(TOKENID_NUM_DECIMAL, value);
            }
            break;
         case STATE_OPERATOR_ADD:
               prev = c;
               return token_ctor(TOKENID_OPERATOR_ADD, value);
         case STATE_OPERATOR_SUB:
               prev = c;
               return token_ctor(TOKENID_OPERATOR_SUB, value);
         case STATE_OPERATOR_MUL:
               prev = c;
               return token_ctor(TOKENID_OPERATOR_MUL, value);
         case STATE_OPERATOR_LESS:
            if (c == '=') {
               return token_ctor(TOKENID_OPERATOR_LESS_OR_EQUAL, value);
            } else {
               prev = c;
               return token_ctor(TOKENID_OPERATOR_LESS, value);
            }
         case STATE_OPERATOR_MORE:
            if (c == '=') {
               return token_ctor(TOKENID_OPERATOR_GREATER_OR_EQUAL, value);
            } else {
               prev = c;
               return token_ctor(TOKENID_OPERATOR_GREATER, value);
            }
         case STATE_OPERATOR_OR_COMMENT:
            if (c == '/') {
               state = STATE_COMMENT;
            } else if (c == '*') {
               state = STATE_BLOCK_COMMENT;
            } else {
               prev = c;
               return token_ctor(TOKENID_OPERATOR_DIV, value);
            }
            break;
         case STATE_OPERATOR_DECLARE:
            if (c == '=') {
               return token_ctor(TOKENID_OPERATOR_DECLARE, value);
            } else {
               exit(ERRCODE_LEXICAL_ERROR);
            }
            break;
         case STATE_OPERATOR_ASSIGN:
            if (c == '=') {
               return token_ctor(TOKENID_OPERATOR_EQUALS, value);
            } else {
               prev = c;
               return token_ctor(TOKENID_OPERATOR_ASSIGN, value);
            }
         case STATE_OPERATOR_NOT:
            if (c == '=') {
               return token_ctor(TOKENID_OPERATOR_NOT_EQUAL, value);
            } else {
               prev = c;
               return token_ctor(TOKENID_OPERATOR_NOT, value);
            }
         case STATE_OPERATOR_AND:
            if (c == '&') {
               return token_ctor(TOKENID_OPERATOR_AND, value);
            } else {
               exit(ERRCODE_LEXICAL_ERROR);
            }
         case STATE_OPERATOR_OR:
            if (c == '|') {
               return token_ctor(TOKENID_OPERATOR_OR, value);
            } else {
               exit(ERRCODE_LEXICAL_ERROR);
            }
         case STATE_QUOTATION_MARKS:
            if (c == '\\') {
               state = STATE_ESCAPE_SEQUENCE;
            } else if (c != '"') {
               buffer = append((char) c, buffer);
               if (c == EOF) {
                  exit(ERRCODE_LEXICAL_ERROR);
               }
            } else {
               value.string_value = buffer;
               return token_ctor(TOKENID_STRING_LITERAL, value);
            }
            break;
         case STATE_ESCAPE_SEQUENCE:
            switch (c) {
            case 'x':
               state = STATE_HEXADECIMAL_ESCAPE_SEQUENCE_START;
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
         case STATE_HEXADECIMAL_ESCAPE_SEQUENCE_START:;
            char* temp = NULL; // temporary varaible to store and later determine escape sequence
            if (isdigit(c) || (toupper(c) >= 'A' && toupper(c) <= 'F')) {
               temp = append((char) c, temp);
               state = STATE_HEXADECIMAL_ESCAPE_SEQUENCE_END;
            } else {
               exit(ERRCODE_LEXICAL_ERROR);
            }
            break;
         case STATE_HEXADECIMAL_ESCAPE_SEQUENCE_END:
            if (isdigit(c) || (toupper(c) >= 'A' && toupper(c) <= 'F')) {
               temp = append((char) c, temp);
            } else {
               exit(ERRCODE_LEXICAL_ERROR);
            }
            if (strlen(temp) == 2) {
               char* pEnd;
               buffer = append((char) strtol(temp, &pEnd, BASE_HEX), buffer);
               free(temp);
               state = STATE_QUOTATION_MARKS;
            }
            break;
         case STATE_LEFT_PARENTHESES:
            prev = c;
            return token_ctor(TOKENID_LEFT_PARENTHESES, value);
         case STATE_RIGHT_PARENTHESES:
            prev = c;
            return token_ctor(TOKENID_RIGHT_PARENTHESES, value);
         case STATE_LEFT_BRACKET:
            prev = c;
            return token_ctor(TOKENID_LEFT_BRACKET, value);
         case STATE_RIGHT_BRACKET:
            prev = c;
            return token_ctor(TOKENID_RIGHT_BRACKET, value);
         case STATE_SEMICOLON:
            prev = c;
            return token_ctor(TOKENID_SEMICOLON, value);
         case STATE_COMMA:
            prev = c;
            return token_ctor(TOKENID_COMMA, value);
         case STATE_NEWLINE:
            prev = c;
            return token_ctor(TOKENID_NEWLINE, value);
         case STATE_EOF:
            // prev = c;
            return token_ctor(TOKENID_END_OF_FILE, value);
         case STATE_COMMENT:
            if (c == '\n' || c == EOF) {
               prev = c;
               state = STATE_START;
            }
            break;
         case STATE_BLOCK_COMMENT:
            if (c == '*') {
               state = STATE_BLOCK_COMMENT_END;
            } else if (c == EOF) {
               exit(ERRCODE_LEXICAL_ERROR);
            }
            break;
         case STATE_BLOCK_COMMENT_END:
            if (c == '/') {
               state = STATE_START;
            } else if (c == EOF) {
               exit(ERRCODE_LEXICAL_ERROR);

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
