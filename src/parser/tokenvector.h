/**
 * IFJ20 Compiler
 * @file ast.h
 * @authors Michal Trlica (xtrlic02)
 */

#pragma once

#include "../scanner/scanner.h"

typedef struct tokenvector_t {
   int length;
   int capacity;
   token_t** memory;
} tokenvector_t;

tokenvector_t* tokenvector_ctor();

void tokenvector_dtor(tokenvector_t* vector);

token_t* tokenvector_get(tokenvector_t* vector, int index);

void tokenvector_push(tokenvector_t* vector, token_t* token);

int tokenvector_get_lenght(tokenvector_t* vector);
