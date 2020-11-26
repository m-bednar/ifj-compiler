/**
 * IFJ20 Compiler
 * @file ast.c
 * @authors Michal Trlica (xtrlic02)
 */

#include "astnodestack.h"
#include "../memory.h"
#include "../error.h"

static const float DEALLOC_MIN_UNUSED_PERCENTAGE = 0.5;
static const int ALLOC_MIN_ELEMENTS_COUNT = 3;

astnodestack_t* astnodestack_ctor(){
   astnodestack_t* stack = safe_alloc(sizeof(astnodestack_t));
   stack->length = 0;
   stack->capacity = 0;
   stack->memory = NULL;
   return stack;
}

void astnodestack_dtor(astnodestack_t* stack){
   guard(stack != NULL);
   if (stack->memory == NULL) {
      return;
   }
   free(stack->memory);
   stack->length = 0;
   stack->capacity = 0;
   stack->memory = NULL;
   free(stack);
}

astnode_generic_t* astnodestack_peek(astnodestack_t* stack){
   guard(stack != NULL);
   guard(stack->length != 0);
   return stack->memory[stack->length - 1];
}

astnode_generic_t* astnodestack_pop(astnodestack_t* stack){
   guard(stack != NULL);
   guard(stack->length != 0);
   astnode_generic_t* last = stack->memory[stack->length - 1];
   stack->length--;

   // Deallocates excessive memory, when less then DEALLOC_MIN_UNUSED_PERCENTAGE percent is used
   if (stack->length < stack->capacity * DEALLOC_MIN_UNUSED_PERCENTAGE && stack->capacity > ALLOC_MIN_ELEMENTS_COUNT) {
      stack->memory = safe_realloc(stack->memory, sizeof(astnode_generic_t*) * (stack->length));
      stack->capacity = stack->length;
   }

   return last;
}

void astnodestack_push(astnodestack_t* stack, astnode_generic_t* astnode){
   guard(stack != NULL);
   guard(astnode != NULL);
   
   // Allocates additional memory, when capacity is reached
   if (stack->capacity == stack->length) {
      stack->memory = safe_realloc(stack->memory, sizeof(astnode_generic_t*) * (stack->length + ALLOC_MIN_ELEMENTS_COUNT));
      stack->capacity += ALLOC_MIN_ELEMENTS_COUNT;
   }

   stack->memory[stack->length] = astnode;
   stack->length++;   
}

