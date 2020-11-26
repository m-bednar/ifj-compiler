
/**
 * IFJ20 Compiler
 * @file ntposymstack.c
 * @authors Daniel Havranek (xhavra18)
 */

#include "ntposymstack.h"

static const float DEALLOC_MIN_UNUSED_PERCENTAGE = 0.5;
static const int ALLOC_MIN_ELEMENTS_COUNT = 4;

ntposymstack_t* ntposymstack_ctor() {
   ntposymstack_t* stack = safe_alloc(sizeof(ntposymstack_t));

   stack->length = 0;
   stack->capacity = 0;
   stack->memory = NULL;
   return stack;
}

void ntposymstack_dtor(ntposymstack_t* stack) {
   guard(stack != NULL);

   if (stack->memory != NULL) {
      for (int i = 0; i < stack->length; i++) {
         ntposymbol_dtor(stack->memory[i]);
      }
      free(stack->memory);
      stack->length = 0;
      stack->capacity = 0;
      stack->memory = NULL;
      free(stack);
   }
   return;
}

void ntposymstack_push(ntposymstack_t* stack, ntposymbol_t* ntposymbol) {
   guard(stack != NULL);
   guard(ntposymbol != NULL);

   // Allocates additional memory, when capacity is reached
   if (stack->capacity == stack->length) {
      stack->memory = safe_realloc(
            stack->memory,
            sizeof(ntposymbol_t*) * (stack->length + ALLOC_MIN_ELEMENTS_COUNT));
      stack->capacity += ALLOC_MIN_ELEMENTS_COUNT;
   }

   stack->memory[stack->length] = ntposymbol;
   stack->length++;
   return;
}

ntposymbol_t* ntposymstack_pop(ntposymstack_t* stack) {
   guard(stack != NULL);
   guard(stack->length != 0);

   ntposymbol_t* top = stack->memory[stack->length - 1];
   stack->length--;

   // Deallocates excessive memory, when less than DEALLOC_MIN_UNUSED_PERCENTAGE
   // percent is used
   if (stack->length < stack->capacity * DEALLOC_MIN_UNUSED_PERCENTAGE &&
         stack->capacity > ALLOC_MIN_ELEMENTS_COUNT) {
      stack->memory =
            safe_realloc(stack->memory, sizeof(ntposymbol_t*) * (stack->length));
      stack->capacity = stack->length;
   }

   return top;
}

ntposymbol_t* ntposymstack_top(ntposymstack_t* stack) {
   guard(stack != NULL);
   guard(stack->length != 0);

   ntposymbol_t* top = stack->memory[stack->length - 1];
   return top;
}

ntposymbol_t* ntposymstack_top_terminal(ntposymstack_t* stack) {
   guard(stack != NULL);
   guard(stack->length != 0);

   for (int i = stack->length - 1; i >= 0; i--) {
      if (stack->memory[i]->type == TERMINAL) {
         return stack->memory[i];
      }
   }

   return NULL;
}

int ntposymstack_get_length(ntposymstack_t* stack) {
   return stack->length;
}
