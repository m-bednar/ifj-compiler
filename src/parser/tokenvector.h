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

tokenvector_t* tokenvector_t_ctor();

void tokenvector_dtor(tokenvector_t* vector);

tokenvector_t* tokenvector_get(int index);

void tokenvector_push(token_t* token);

int tokenstack_get_lenght(tokenvector_t* vector);
