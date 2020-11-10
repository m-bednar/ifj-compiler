/**
 * IFJ20 Compiler
 * @file main.c
 * @authors Martin Bednář (xbedna77)
 */

#include <stdio.h>
#include <stdlib.h>
#include "scanner/scanner.h"

int main() {
   /*token_t* token = get_next_token();
   //printf("VALUE: %s\n", token->value.string_value);
   printf("TOKEN ID: %d\n", token->id);
   //printf("POINTER ADDRESS: %p\n", token->value.string_value);
   printf("-------------------------\n");
   token_t *token2 = get_next_token();
   //printf("VALUE: %s\n", token2->value.string_value);
   printf("TOKEN ID: %d\n", token2->id);
   //printf("POINTER ADDRESS: %p\n", token2->value.string_value);
   printf("-------------------------\n");
   token_t *token3 = get_next_token();
   //printf("VALUE: %s\n", token3->value.string_value);
   printf("TOKEN ID: %d\n", token3->id);
   //printf("POINTER ADDRESS: %p\n", token3->value.string_value);
   printf("-------------------------\n");
   token_t *token4 = get_next_token();
   //printf("VALUE: %s\n", token4->value.string_value);
   printf("TOKEN ID: %d\n", token4->id);
   //printf("POINTER ADDRESS: %p\n", token4->value.string_value);
   printf("-------------------------\n");
   token_t *token5 = get_next_token();
   //printf("VALUE: %s\n", token5->value.string_value);
   printf("TOKEN ID: %d\n", token5->id);
   //printf("POINTER ADDRESS: %p\n", token5->value.string_value);
   printf("-------------------------\n");
   token_t *token6 = get_next_token();
   //printf("VALUE: %s\n", token6->value.string_value);
   printf("TOKEN ID: %d\n", token6->id);
   //printf("POINTER ADDRESS: %p\n", token6->value.string_value);
   printf("-------------------------\n");
   token_dtor(token);
   token_dtor(token2);
   token_dtor(token3);
   token_dtor(token4);
   token_dtor(token5);
   token_dtor(token6);*/
   int i = 0;
   token_t* token;
   while(i < 50){
      token = get_next_token();
      printf("-%d-\n", i);
      if(token->id == TOKENID_IDENTIFIER || token->id == TOKENID_STRING_LITERAL) {
         printf("STRING VALUE: %s - ", token->value.string_value);
         printf("POINTER ADDRESS: %p\n", token->value.string_value);
      }else if (token->id == TOKENID_NUM) {
         printf("INT VALUE: %ld\n", token->value.int_value);
      }else if (token->id == TOKENID_NUM_DECIMAL) {
         printf("DOUBLE VALUE: %f\n", token->value.decimal_value);
      }
      printf("TOKEN ID: %d\n", token->id);
      printf("-------------------------\n");
      i++;
      token_dtor(token);
   }
   return EXIT_SUCCESS;
}
