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

   // Deallocates excessive memory, when less then DEALLOC_MIN_UNUSED_PERCENTAGE percent is used
   if (stack->length < stack->capacity * DEALLOC_MIN_UNUSED_PERCENTAGE && stack->capacity > ALLOC_MIN_ELEMENTS_COUNT) {
      stack->memory = safe_realloc(stack->memory, sizeof(bintree_t*) * (stack->length));
      stack->capacity = stack->length;
   }

   return last;
}

bintree_t* bintreestack_peek(bintreestack_t* stack) {
   guard(stack != NULL);
   guard(stack->length != 0);
   return stack->memory[stack->length - 1];
}


symbol_t* bintreestack_find(bintreestack_t* stack, char* identifier, int* level) {
   guard(stack != NULL);
   guard(identifier != NULL);

   for (int i = stack->length - 1; i >= 0; i--) {
      symbol_t* found = bintree_find(stack->memory[i], identifier);
      if (found != NULL) {
         if (level != NULL) {
            *level = i;
         }
         return found;
      }
   }

   return NULL;
}

int bintreestack_get_length(bintreestack_t* stack) {
   return stack->length;
}

void bintreestack_print(bintreestack_t* stack) {
   for (int i = stack->length - 1; i >= 0; i--) {
      fprintf(stderr, "----- stack %d -----\n", i);
      bintree_print(stack->memory[i]);
      fprintf(stderr, "-------------------\n");
   }
}
