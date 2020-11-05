/**
 * IFJ20 Compiler
 * @file scanner.c
 * @authors František Lukeš (xlukes07)
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "scanner.h"
#include "../memory.h"
#include "../error.h"
#define KEYWORD_COUNT 10

const char *control_keywords[] = {"if", "for", "else", "return"};
const char *declaration_keywords[] = {"package", "func"};
const char *datatypes[] = {"float64", "int", "string", "bool"};

typedef enum state {
   STATE_START,
   STATE_IDENTIFIER_KEYWORD,
   STATE_NUM,
   STATE_NUM_ZERO,
   STATE_DECIMAL,
   STATE_BASE,
   STATE_EXP_START,
   STATE_EXP,
   STATE_OPERATOR,
   STATE_OPERATOR_LESS,
   STATE_OPERATOR_MORE,
   STATE_OPERATOR_OR_COMMENT,
   STATE_DECLARATION_OPERATOR,
   STATE_ASSIGN_OPERATOR,
   STATE_QUOTATION_MARKS,
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
} state;

token_t* token_ctor(tokenid id, token_value_u value) {
   token_t* token = safe_alloc(sizeof(token_t));
   token->id = id;
   token->value = value;
   return token;
}

void token_dtor(token_t* token) {
   guard(token != NULL);
   free(token);
}

/*
 * Determines next state from STATE_START
 */
int determine_next_state(int c) {
   if(isalpha(c) != 0 || c == '_') { // c is a character (a-z / A-Z)
      return STATE_IDENTIFIER_KEYWORD;
   }else if(isdigit(c) != 0) { // c is a digit
      if(c == '0') { // c is 0
         return STATE_NUM_ZERO;
      }else { // c is digit other than 0
         return STATE_NUM;
      }
   }else if(c == '+' || c == '-' || c == '*' || c == '<' || c == '>') { // c is an operator (+, -, *, <, >)
      return STATE_OPERATOR;
   }else if(c == '/') { // c is either and operator (/) or start of comment
      return STATE_OPERATOR_OR_COMMENT;
   }else if (c == ':') { // Start of varible declaration operator (:=)  
      return STATE_DECLARATION_OPERATOR;
   }else if (c == '=') { // c is assign operator (=)
      return STATE_ASSIGN_OPERATOR;
   }else if(c == '"') {
      return STATE_QUOTATION_MARKS;
   }else if (c == '('){
      return STATE_LEFT_PARENTHESES;
   }else if (c == ')'){
      return STATE_RIGHT_PARENTHESES;
   }else if (c == '{'){
      return STATE_LEFT_BRACKET;
   }else if (c == '}'){
      return STATE_RIGHT_BRACKET;
   }else if (c == ';') {
      return STATE_SEMICOLON;
   }else if (c == ',') {
      return STATE_COMMA;
   }else if (c == '\n') { // c is newline
      return STATE_NEWLINE;
   }else if (c == EOF) { // end of file
      return STATE_EOF;
   }
   return STATE_START;
}

/*
 * Resizes buffer to fit one more character and inserts character c to the end.
 */
char* insert_into_buffer(int c, char* buffer, int buffer_size) {
   buffer = safe_realloc(buffer, buffer_size*(sizeof(char)));
   buffer[buffer_size-2] = c; 
   return buffer;
}

int decide_operator(char c){
   int id;
   switch(c) {
      case '+' :
         id = TOKENID_OPERATOR_PLUS;
         break;
      case '-' :
         id = TOKENID_OPERATOR_MINUS;
         break;
      case '*' :
         id = TOKENID_OPERATOR_MUL;
         break;
   }
   return id;
}
/*
 * Returns number indicating base (2, 8, 16), or 0 if none of the other apply
 */
int determine_base(int c) {
   int base = 0;
   if(c == 'b' || c == 'B') { // number is binary
      base = 2;   
   }else if (c == 'o' || c == 'O') { // number is octal
      base = 8;
   }else if (c == 'x' || c == 'X') { // number is hexadecimal
      base = 16;
   }
   return base;
}

/**
 * Determines whether c is a keyword or identifier.
 */
tokenid is_keyword(char *c){
   for(int i = 0; i < 4; i++){
      if((strcmp(c, control_keywords[i])) == 0){
         return TOKENID_CONTROL_KEYWORD;
      }
   }
   for(int i = 0; i < 2; i++){
      if((strcmp(c, declaration_keywords[i])) == 0){
         return TOKENID_DECLARATION_KEYWORD;
      }
   }
   for(int i = 0; i < 4; i++){
      if((strcmp(c, datatypes[i])) == 0){
         return TOKENID_DATATYPE;
      }
   }
   return TOKENID_IDENTIFIER;
}

token_t* get_next_token() {
   int state = STATE_START;
   int c = '\0';
   static int prev = '\0'; // previously read character
   int buffer_size = 1;    // buffer_size starts at 1 to fit at least '\0'
   char *buffer = NULL;
   int base = 0;
   double num; // used to store numerical value to free up space in buffer for exponent
   token_value_u value;
   token_t* token;
   while(1) {
      if(prev == '\0') {
         c = getchar();
      } else {
         c = prev;
         prev = '\0';
      }
      switch (state) {
      case STATE_START:
         if((state = determine_next_state(c)) != 0) {
            if(state == STATE_IDENTIFIER_KEYWORD || state == STATE_NUM_ZERO || state == STATE_NUM) {
               buffer = safe_alloc(buffer_size*sizeof(char));
               prev = c;
            }else if(state == STATE_OPERATOR) {
               prev = c;
            }else if(state == STATE_QUOTATION_MARKS) {
               buffer = safe_alloc(buffer_size*sizeof(char));
            }
         }
         break;
      case STATE_IDENTIFIER_KEYWORD:
         if((isalpha(c) != 0) || c == '_' || (isdigit(c) != 0)) {
            buffer = insert_into_buffer((char)c, buffer, ++buffer_size);
         }else {
            prev = c;
            buffer[buffer_size-1] = '\0';
            value.string_value = buffer;
            token = token_ctor(is_keyword(value.string_value), value);
            return token;
         }
         break;
      case STATE_NUM:
         if(isdigit(c) != 0) { // digit
            buffer = insert_into_buffer((char)c, buffer, ++buffer_size);
         }else if(c == '.') { // decimal
            buffer = insert_into_buffer((char)c, buffer, ++buffer_size);
            state = STATE_DECIMAL;
         }else if((c == 'e') || (c == 'E')) {
            state = STATE_EXP_START;
         }else {  // end of num
            prev = c;
            buffer[buffer_size-1] = '\0';
            char *pEnd;
            value.int_value = (int64_t) strtoll(buffer, &pEnd, base);
            token = token_ctor(TOKENID_NUM, value);
            return token;
         }
         break;
      case STATE_NUM_ZERO:
         //TODO: FORMATING
         if((base = determine_base(c)) != 0) { // number will be read in different base
            state = STATE_BASE;
         }else if(c == '.') { // 0. ->decimal
            buffer = insert_into_buffer((char) c, buffer, ++buffer_size);
            state = STATE_DECIMAL;
         }else if(c == 'e' || c == 'E') {
            buffer = insert_into_buffer((char) 0, buffer, ++buffer_size);
            state = STATE_EXP_START;
         }else if((isdigit(c) != 0) || (isalpha(c) != 0)) { // other digits or a-z/A-Z after 0 not allowed ->error
            //TODO: error implementation
         }else { //just 0
            prev = c;
            value.int_value = 0;
            token = token_ctor(TOKENID_NUM, value);
            return token;
         }
         break;
      case STATE_DECIMAL:
         if(isdigit(c) != 0) {
            // TODO: at least one number must be present after decimal point
            buffer = insert_into_buffer((char)c, buffer, ++buffer_size);
         }else if (c == 'e' || c == 'E') { // exponent
            state = STATE_EXP_START;
         }else {
            prev = c;
            buffer[buffer_size-1] = '\0';
            char *pEnd;
            value.decimal_value = (double) strtod(buffer, &pEnd);
            token = token_ctor(TOKENID_NUM_DECIMAL, value);
            return token;
         }
         break;
      case STATE_BASE:
         if(isdigit(c) != 0 || c == '_' || (base == 16 && (toupper(c) > 34) && (toupper(c) < 71))) {
            if(c != '_') {
               buffer = insert_into_buffer((char)c, buffer, ++buffer_size);
            }
         }else {
            buffer[buffer_size-1] = '\0';
            prev = c;
            char *pEnd;
            value.int_value = (int64_t) strtoll(buffer, &pEnd, base);
            token = token_ctor(TOKENID_NUM, value);
            return token;
         }
         break;
      case STATE_EXP_START:
         buffer[buffer_size-1] = '\0';
         char *pEnd;
         num = (double) strtod(buffer, &pEnd);
         free(buffer);
         buffer_size = 1;
         buffer = safe_alloc(buffer_size*sizeof(char));
         if(c == '+') {
            //do nothing
         }else if (c == '-') {
            buffer = insert_into_buffer(c, buffer, ++buffer_size);
         }else if(isdigit(c) != 0) {
            prev = c; // put number back to be read in the next state
         }else {
            //TODO: throw error
            printf("error");
         }
         state = STATE_EXP;
         break;
      case STATE_EXP:
         //TODO: ensure at least one digit
         if(isdigit(c) != 0) {
            buffer = insert_into_buffer(c, buffer, ++buffer_size);
         }else{
            buffer[buffer_size-1] = '\0';
            if(strlen(buffer) == 0) { // No digits were read as an exponent
               //TODO: throw an error - no nums in exp
               printf("ERROR - NO DIGITS IN EXPONENT");
            }
            //TODO: NUM EXP
            value.decimal_value = calculate_exponent(num, (double) strtod(buffer, &pEnd));
            token = token_ctor(TOKENID_NUM_DECIMAL, value);
            return token;
         }
         break;
      case STATE_OPERATOR:
         if(c == '<') {
            state = STATE_OPERATOR_LESS;
         }else if(c == '>') {
            state = STATE_OPERATOR_MORE;
         }else {
            token = token_ctor(decide_operator(c), value);
            return token;
         }
         break;
      case STATE_OPERATOR_LESS:
         if(c == '=') {
            token = token_ctor(TOKENID_OPERATOR_LESS_OR_EQUAL, value);
         }else{
         prev = c;
            token = token_ctor(TOKENID_OPERATOR_LESS, value);
         }
         return token;
      case STATE_OPERATOR_MORE:
         if(c == '=') {
            token = token_ctor(TOKENID_OPERATOR_MORE_OR_EQUAL, value);
         }else{
            prev = c;
            token = token_ctor(TOKENID_OPERATOR_MORE, value);
         }
         return token;
      case STATE_OPERATOR_OR_COMMENT:
         if(c == '/'){
            state = STATE_COMMENT;
         }else if(c == '*') {
            state = STATE_BLOCK_COMMENT;
         }else {
            prev = c;
            token = token_ctor(TOKENID_OPERATOR_DIV, value);
            return token;
         }
         break;
      case STATE_DECLARATION_OPERATOR:
         if(c == '=') {
            token = token_ctor(TOKENID_DECLARATION_OPERATOR, value);
            return token;
         }else {
            //TODO: THROW ERROR
         }
         break;
      case STATE_ASSIGN_OPERATOR:
         prev = c;
         token = token_ctor(TOKENID_ASSIGN_OPERATOR, value);
         return token;
         return token;
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
         //prev = c;
         token = token_ctor(TOKENID_END_OF_FILE, value);
         return token;
      case STATE_COMMENT:
         if(c == '\n') {
         prev = c;
            state = STATE_START;
         }
         break;
      case STATE_BLOCK_COMMENT:
         if(c == '*') {
            state = STATE_BLOCK_COMMENT_END;
         }
         break;
      case STATE_BLOCK_COMMENT_END:
         if(c == '/') {
            state = STATE_START;
         }else {
            state = STATE_BLOCK_COMMENT;
         }
         break; 
      default:
         break;
      }
   }
   return NULL;
}

/**
 * Determines whether c is a keyword or identifier.
 */
tokenid is_keyword(char *c){
   for(int i = 0; i < 4; i++){
      if((strcmp(c, control_keywords[i])) == 0){
         printf("it is control keyword!\n");
         return 4;
      }
   }
   for(int i = 0; i < 2; i++){
      if((strcmp(c, declaration_keywords[i])) == 0){
         printf("it is declaration keyword!\n");
         return 5;
      }
   }
   for(int i = 0; i < 4; i++){
      if((strcmp(c, datatypes[i])) == 0){
         printf("it is type keyword!\n");
         return 6;
      }
   }
   printf("it is identifier\n");
   return 3;
}

