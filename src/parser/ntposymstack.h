/**
 * IFJ20 Compiler
 * @file ntposymstack.h
 * @authors Daniel Havranek (xhavra18)
 */

#pragma once

#include "../memory.h"
#include "../error.h"
#include "ntposymbol.h"

typedef struct ntposymstack_t {
   int length;
   int capacity;
   ntposymbol_t** memory;
} ntposymstack_t;

/**
 * Creates new stack.
 */
ntposymstack_t* ntposymstack_ctor();

/**
 * Deallocates stack and all its elements. 
 */
void ntposymstack_dtor(ntposymstack_t* stack);

/**
 * Pushes new element on top of the stack. 
 */
void ntposymstack_push(ntposymstack_t* stack, ntposymbol_t* ntposymbol);

/**
 * Removes and returns top element. Does not deallocate the element.
 * Throws error, when no element is found.
 */
ntposymbol_t* ntposymstack_pop(ntposymstack_t* stack);

/**
 * Returns element on top of the stack without removing it.
 * Throws error, when no element is found.
 */
ntposymbol_t* ntposymstack_top(ntposymstack_t* stack);

/**
 * Returns length (number of elements) of the stack.
 */
int ntposymstack_get_length(ntposymstack_t* stack);
