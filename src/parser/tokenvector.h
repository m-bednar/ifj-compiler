/**
 * IFJ20 Compiler
 * @file ast.h
 * @authors Michal Trlica (xtrlic02)
 */

#pragma once

#include "../scanner/scanner.h"

typedef struct tokenvector_t {
   int length;          // count of tokens in memory
   int capacity;        // size of memory
   token_t** memory;
} tokenvector_t;

/**
 * Allocates new vector
 */
tokenvector_t* tokenvector_ctor();

/**
 * Deallocates tokens and memory
 */
void tokenvector_dtor(tokenvector_t* vector);

token_t* tokenvector_get(tokenvector_t* vector, int index);

/**
 * Add token at the end of the array
 */
void tokenvector_push(tokenvector_t* vector, token_t* token);

/**
 * Return lenght of vector
 */
int tokenvector_get_length(tokenvector_t* vector);

/**
 * Makes copy of memory and returns as array
 */
token_t** tokenvector_get_array(tokenvector_t* vector, int* size);
