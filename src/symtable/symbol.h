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

typedef struct symbolvalfn_t {
   int arg_count;
   vartype* arg_types;
   vartype return_type;
} symbolvalfn_t;

typedef struct symbolvalvar_t {
   vartype type;
} symbolvalvar_t;

typedef union symbolval_u {
   symbolvalfn_t fn;
   symbolvalvar_t var;
} symbolval_u;

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
