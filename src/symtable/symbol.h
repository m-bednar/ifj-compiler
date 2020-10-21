/**
 * IFJ20 Compiler
 * @file symbol.h
 * @authors Martin Bednář (xbedna77)
 */

#pragma once

#include <string.h>
#include "../memory.h"

typedef enum symboltype {
   ST_FUNCTION,
   ST_VARIABLE
} symboltype;

typedef enum vartype {
   VT_VOID,
   VT_BOOL,
   VT_INT,
   VT_FLOAT,
   VT_STRING
} vartype;

typedef union symbolval {
   vartype var_type;
   vartype fn_return;
   symbol_t* fn_args;
} symbolval;

typedef struct symbol_t {
   char* identifier;
   symboltype type;
   symbolval value;
} symbol_t;

/**
 * Constucts new symbol with given values.
 */
symbol_t* symbol_ctor(char* identifier, symboltype type, symbolval value);

/**
 * Destructs symbol and frees it's resources.
 */
void symbol_dtor(symbol_t* symbol);
