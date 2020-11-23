/**
 * IFJ20 Compiler
 * @file ast.h
 * @authors Michal Trlica (xtrlic02)
 */

#include "tokenstack.h"
#include "../memory.h"
#include "../error.h"

static const float DEALLOC_MIN_UNUSED_PERCENTAGE = 0.5;
static const int ALLOC_MIN_ELEMENTS_COUNT = 3;

tokenstack_t* tokenstack_ctor(){
   tokenstack_t* stack = safe_alloc(sizeof(tokenstack_t));
   stack->length = 0;
   stack->capacity = 0;
   stack->memory = NULL;
   return stack;
}

void tokenstack_dtor(tokenstack_t* stack) {
   guard(stack != NULL);
   if (stack->memory == NULL) {
      return;
   }
   for (int i = 0; i < stack->length; i++) {
      token_dtor(stack->memory[i]);
   }
   free(stack->memory);
   stack->length = 0;
   stack->capacity = 0;
   stack->memory = NULL;
   free(stack);
}

void tokenstack_push(tokenstack_t* stack, token_t* token) {
   guard(stack != NULL);
   guard(token != NULL);
   
   // Allocates additional memory, when capacity is reached
   if (stack->capacity == stack->length) {
      stack->memory = safe_realloc(stack->memory, sizeof(token_t*) * (stack->length + ALLOC_MIN_ELEMENTS_COUNT));
      stack->capacity += ALLOC_MIN_ELEMENTS_COUNT;
   }

   stack->memory[stack->length] = token;
   stack->length++;
}

token_t* tokenstack_pop(tokenstack_t* stack) {
   guard(stack != NULL);
   guard(stack->length != 0);
   token_t* last = stack->memory[stack->length - 1];
   stack->length--;

   // Deallocates excessive memory, when less then DEALLOC_MIN_UNUSED_PERCENTAGE percent is used
   if (stack->length < stack->capacity * DEALLOC_MIN_UNUSED_PERCENTAGE && stack->capacity > ALLOC_MIN_ELEMENTS_COUNT) {
      stack->memory = safe_realloc(stack->memory, sizeof(token_t*) * (stack->length));
      stack->capacity = stack->length;
   }

   return last;
}

int tokenstack_get_lenght(tokenstack_t* stack){
   return stack->length;
}
