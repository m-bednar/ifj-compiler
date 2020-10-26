/**
 * IFJ20 Compiler
 * @file bintreestack.h
 * @authors Martin Bednář (xbedna77)
 */

#include "symtable.h"

typedef struct bintreestack_t {
   int length;
   int capacity;
   bintree_t** memory;
} bintreestack_t;

/**
 * Creates new stack.
 */
bintreestack_t* bintreestack_ctor();

/**
 * Deallocates stack and all it's elements. 
 */
void bintreestack_dtor(bintreestack_t* stack);

/**
 * Pushes new element onto top of the stack. 
 */
void bintreestack_push(bintreestack_t* stack, bintree_t* tree);

/**
 * Removes and returns top element. Does not deallocate the element.
 */
bintree_t* bintreestack_pop(bintreestack_t* stack);

/**
 * Returns element on specified index without removing it.
 */
bintree_t* bintreestack_get(bintreestack_t* stack, int index);
