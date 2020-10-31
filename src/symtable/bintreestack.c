/**
 * IFJ20 Compiler
 * @file bintreestack.c
 * @authors Martin Bednář (xbedna77)
 */

#include "bintreestack.h"
#include "../memory.h"
#include "../error.h"

static const float DEALLOC_MIN_UNUSED_PERCENTAGE = 0.5;
static const int ALLOC_MIN_ELEMENTS_COUNT = 3;

bintreestack_t* bintreestack_ctor() {
   bintreestack_t* stack = safe_alloc(sizeof(bintreestack_t));
   stack->length = 0;
   stack->capacity = 0;
   stack->memory = NULL;
   return stack;
}

void bintreestack_dtor(bintreestack_t* stack) {
   guard(stack != NULL);
   if (stack->memory == NULL) {
      return;
   }
   for (int i = 0; i < stack->length; i++) {
      bintree_dtor(stack->memory[i]);
   }
   free(stack->memory);
   stack->length = 0;
   stack->capacity = 0;
   stack->memory = NULL;
   free(stack);
}

void bintreestack_push(bintreestack_t* stack, bintree_t* tree) {
   guard(stack != NULL);
   guard(tree != NULL);
   
   // Allocates additional memory, when capacity is reached
   if (stack->capacity == stack->length) {
      stack->memory = safe_realloc(stack->memory, sizeof(bintree_t*) * (stack->length + ALLOC_MIN_ELEMENTS_COUNT));
      stack->capacity += ALLOC_MIN_ELEMENTS_COUNT;
   }

   stack->memory[stack->length] = tree;
   stack->length++;
}

bintree_t* bintreestack_pop(bintreestack_t* stack) {
   guard(stack != NULL);
   guard(stack->length != 0);
   bintree_t* last = stack->memory[stack->length - 1];
   stack->length--;

   // Deallocates excessive memory, when 1/3 or less than 1/3 of the capacity is used
   if (stack->length < stack->capacity * DEALLOC_MIN_UNUSED_PERCENTAGE) {
      stack->memory = safe_realloc(stack->memory, sizeof(bintree_t*) * (stack->length));
      stack->capacity = stack->length;
   }

   return last;
}

bintree_t* bintreestack_get(bintreestack_t* stack, int index) {
   guard(stack != NULL);
   guard(stack->length != 0);
   guard(index < stack->length);
   guard(index > 0);

   return stack->memory[index];
}

int bintreestack_get_length(bintreestack_t* stack) {
   return stack->length;
}
