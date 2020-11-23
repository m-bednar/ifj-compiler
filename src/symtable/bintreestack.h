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
 * Throws error, when no element is found.
 */
bintree_t* bintreestack_pop(bintreestack_t* stack);

/**
 * Returns top element without removing it.
 */
bintree_t* bintreestack_peek(bintreestack_t* stack);

/**
 * Returns first found symbol in stack with given identifier or NULL when not found.
 */
symbol_t* bintreestack_find(bintreestack_t* stack, char* identifier, int* level);

/**
 * Returns length (number of elements) of the stack.
 */
int bintreestack_get_length(bintreestack_t* stack);

/**
 * Prints whole stack and it's content to stderr.
 */
void bintreestack_print(bintreestack_t* stack);
