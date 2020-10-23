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

char control_keywords[4][6] = {"if", "for", "else", "return"} ;
char declaration_keywords[2][7] = {"package", "func"};
char datatypes[4][7] = {"float64", "int", "string", "bool"};

typedef enum state {
   STATE_START,
   STATE_IDENTIFIER_KEYWORD,
   STATE_NUM,
   STATE_OPERATOR,
   STATE_DECLARATION_OPERATOR,
   STATE_ASSIGN_OPERATOR,
   STATE_SPACE,
   STATE_NEWLINE,
   STATE_EOF
} state;

token_t* token_ctor(tokenid id) {
   token_t* token = safe_alloc(sizeof(token_t));
   token->id = id;
   return token;
}

void token_dtor(token_t* token) {
   guard(token != NULL);
   free(token);
}

token_t* get_next_token() {
   token_t* token = token_ctor(0);
   int state = STATE_START;
   char c = '\0';
   static char prev = '\0';
   while(1) {
      if(prev == '\0') {
         c = getchar();
      } else {
         c = prev;
         prev = '\0';
      }
      //char buffer[20]; //TODO:
      int i;
      switch (state) {
      case STATE_START:
         i = 0;
         if(isalpha(c) != 0) { // c is a character (a-z / A-Z)
            //buffer[i] = c;
            token->value.string_value[i] = c;
            i++;
            state = STATE_IDENTIFIER_KEYWORD;
         }else if(isdigit(c) != 0) { // c is a digit
            state = STATE_NUM;
         }else if(c == '+') {
            state = STATE_OPERATOR;
         }else if(c == '-') {
            state = STATE_OPERATOR;
         }else if(c == '*') {
            state = STATE_OPERATOR;
         }else if(c == '/') {
            state = STATE_OPERATOR;
         }else if (c == ':') {
            state = STATE_DECLARATION_OPERATOR; // Bylo nalezeno ':', očekáváme '=' pro deklaraci proměnné
         }else if (c == '=') {
            state = STATE_ASSIGN_OPERATOR;
         }else if (c == ' ') {
            state = STATE_SPACE;
         }else if (c == '\n') {
            state = STATE_NEWLINE;
         }else if (c == EOF) {
            state = STATE_EOF;
         }
         break;
      case STATE_IDENTIFIER_KEYWORD:
         if((isalpha(c) != 0) || (isdigit(c) != 0)) {
            //buffer[i] = c;
            token->value.string_value[i] = c;
            i++;
         }else {
            //buffer[i] = '\0';
            //ungetc(c, stdin);
            prev = c;
            token->value.string_value[i] = '\0';
            token->id = TOKENID_IDENTIFIER;
            state = STATE_START;
            return token;
         }
         break;
      case STATE_NUM:
         break;
      case STATE_OPERATOR:
         //ungetc(c, stdin);
         prev = c;
         token->id = TOKENID_OPERATOR;
         token->value.string_value[i] = c;
         token->value.string_value[i+1] = '\0';
         return token;
         break;
      case STATE_DECLARATION_OPERATOR:
         break;
      case STATE_ASSIGN_OPERATOR:
         //ungetc(c, stdin);
         prev = c;
         state = STATE_START;
         token->id = TOKENID_ASSIGN_OPERATOR;
         token->value.string_value[i] = c;
         token->value.string_value[i+1] = '\0';
         return token;
         break;
      case STATE_SPACE:
         //ungetc(c, stdin);
         prev = c;
         state = STATE_START;
         token->id = TOKENID_SPACE;
         token->value.string_value[i] = ' ';
         token->value.string_value[i+1] = '\0';
         return token;
         break;
      case STATE_NEWLINE:
         //ungetc(c, stdin);
         prev = c;
         state = STATE_START;
         token->id = TOKENID_NEWLINE;
         token->value.string_value[i] = c;
         token->value.string_value[i+1] = '\0';
         return token;
         break;
      case STATE_EOF:
         //ungetc(c, stdin);
         prev = c;
         token->id = TOKENID_END_OF_FILE;
         token->value.string_value[i] = c;
         token->value.string_value[i+1] = '\0';
         return token;
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

