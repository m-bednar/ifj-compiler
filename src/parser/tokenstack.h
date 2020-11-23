/**
 * IFJ20 Compiler
 * @file ast.h
 * @authors Michal Trlica (xtrlic02)
 */

#pragma once

#include "../scanner/scanner.h"

typedef struct tokenstack_t {
   int length;
   int capacity;
   token_t** memory;
} tokenstack_t;

tokenstack_t* tokenstack_ctor();

void tokenstack_dtor(tokenstack_t* stack);

void tokenstack_push(tokenstack_t* stack, token_t* token);

token_t* tokenstack_pop(tokenstack_t* stack);

int tokenstack_get_lenght(tokenstack_t* stack);
