/**
 * IFJ20 Compiler
 * @file ntsymstack.h
 * @authors Daniel Havranek (xhavra18)
 */

#pragma once

#include "../memory.h"
#include "ntsymbol.h"

typedef struct ntsymstack_t {
   int length;
   int capacity;
   ntsymbol_t** memory;
} ntsymstack_t;

/**
 * Creates new stack.
 */
ntsymstack_t* ntsymstack_ctor();

/**
 * Deallocates stack and all its elements. 
 */
void ntsymstack_dtor(ntsymstack_t* stack);

/**
 * Pushes new element on top of the stack. 
 */
void ntsymstack_push(ntsymstack_t* stack, ntsymbol_t* ntsymbol);

/**
 * Removes and returns top element. Does not deallocate the element.
 * Throws error, when no element is found.
 */
ntsymbol_t* ntsymstack_pop(ntsymstack_t* stack);

/**
 * Returns element on top of the stack without removing it.
 * Throws error, when no element is found.
 */
ntsymbol_t* ntsymstack_top(ntsymstack_t* stack);

/**
 * Returns length (number of elements) of the stack.
 */
int ntsymstack_get_length(ntsymstack_t* stack);
