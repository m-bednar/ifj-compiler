/**
 * IFJ20 Compiler
 * @file symbol.h
 * @authors Martin Bednář (xbedna77)
 */

#pragma once

#include <string.h>
#include "symbolval.h"
#include "../memory.h"

typedef enum symboltype {
   ST_FUNCTION,
   ST_VARIABLE
} symboltype;

typedef struct symbol_t {
   char* identifier;
   symboltype type;
   symbolval_u value;
} symbol_t;

/**
 * Constucts new symbol with given values.
 */
symbol_t* symbol_ctor(char* identifier, symboltype type, symbolval_u value);

/**
 * Destructs symbol and frees it's resources.
 */
void symbol_dtor(symbol_t* symbol);

/**
 * Prints symbol to the stderr.
 */
void symbol_print(symbol_t* symbol);
