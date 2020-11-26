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
   vector->length--;

   return token;
}

void tokenvector_push(tokenvector_t* vector, token_t* token){
   guard(token != NULL);
   guard(token != NULL);
   
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
   for(int i = 0; i < vector->length; i++){
      printf("Tokens: %s", vector->memory[i]->value.string_value);
   }
   printf("\n");
}
