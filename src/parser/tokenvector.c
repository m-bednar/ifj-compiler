/**
 * IFJ20 Compiler
 * @file ast.h
 * @authors Michal Trlica (xtrlic02)
 */

#include "tokenvector.h"
#include "../memory.h"
#include "../error.h"

static const int ALLOC_MIN_ELEMENTS_COUNT = 3;

tokenvector_t* tokenvector_ctor(){
   tokenvector_t* vector = safe_alloc(sizeof(tokenvector_t));
   vector->length = 0;
   vector->capacity = 0;
   vector->memory = NULL;
   return vector;
}

void tokenvector_dtor(tokenvector_t* vector){
   guard(vector != NULL);
   if(vector->memory == NULL){
      return;
   }
   
   free(vector->memory);
   vector->length = 0;
   vector->capacity = 0;
   vector->memory = NULL;
   free(vector);
}

token_t* tokenvector_get(tokenvector_t* vector, int index){
   guard(vector != NULL);
   guard(vector->length > index && index >= 0);
   token_t* token = vector->memory[index];

   return token;
}

void tokenvector_push(tokenvector_t* vector, token_t* token){
   guard(token != NULL);
   guard(vector != NULL);
   
   // Allocates additional memory, when capacity is reached
   if (vector->capacity == vector->length) {
      vector->memory = safe_realloc(vector->memory, sizeof(token_t*) * (vector->length + ALLOC_MIN_ELEMENTS_COUNT));
      vector->capacity += ALLOC_MIN_ELEMENTS_COUNT;
   }

   vector->memory[vector->length] = token;
   vector->length++;
}

int tokenvector_get_lenght(tokenvector_t* vector){
   return vector->length;
}

void tokenvector_print(tokenvector_t* vector){
   printf("Tokens: ");
   for(int i = 0; i < vector->length; i++){
      switch(vector->memory[i]->id){
         case TOKENID_IDENTIFIER:
            printf("%s ", vector->memory[i]->value.string_value);
            break;
         case TOKENID_KEYWORD_IF:
            printf("if ");
            break;
         case TOKENID_NUM:
            printf("%ld ", vector->memory[i]->value.int_value);
            break;
         case TOKENID_NUM_DECIMAL:
            printf("%f ", vector->memory[i]->value.decimal_value);
            break;
         case TOKENID_LEFT_PARENTHESES:
            printf("( ");
            break;
         case TOKENID_RIGHT_PARENTHESES:
            printf(") ");
            break;
         case TOKENID_RIGHT_BRACKET:
            printf("} ");
            break;
         case TOKENID_LEFT_BRACKET:
            printf("{ ");
            break;
         case TOKENID_KEYWORD_RETURN:
            printf("return ");
            break;
         case TOKENID_OPERATOR_ASSIGN:
            printf("= ");
            break;
         case TOKENID_OPERATOR_DECLARE:
            printf(":= ");
            break;
         
         default:
         break;
      }
      
   }
   printf("\n");
}

token_t** tokenvector_get_array(tokenvector_t* vector, int* size){
   (*size) = vector->length;
   return vector->memory;
}