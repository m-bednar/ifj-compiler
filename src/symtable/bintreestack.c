/**
 * IFJ20 Compiler
 * @file bintreestack.c
 * @authors Martin Bednář (xbedna77)
 */

#include "bintreestack.h"
#include "../memory.h"
#include "../error.h"


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
      stack->memory = safe_realloc(stack->memory, sizeof(bintree_t*) * (stack->length + 1));
      stack->capacity++;
   }

   stack->memory[stack->length] = tree;
   stack->length++;
}

bintree_t* bintreestack_pop(bintreestack_t* stack) {
   guard(stack != NULL);
   guard(stack->length != 0);
   bintree_t* last = stack->memory[stack->length - 1];
   stack->length--;

   // Deallocates excessive memory, when half or less than half of the capacity is used
   if (stack->length <= stack->capacity / 2) {
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
