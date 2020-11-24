/**
 * IFJ20 Compiler
 * @file symbolval.h
 * @authors Martin Bednář (xbedna77)
 */

#pragma once
#include <stdbool.h>
#include "../scanner/token.h"

typedef enum vartype_e {
   VT_UNDEFINED,
   VT_BOOL,
   VT_INT,
   VT_FLOAT,
   VT_STRING
} vartype_e;

typedef struct symbolvalfn_t {
   int arg_count;          // Count of arguments
   int ret_count;          // Count of return values
   vartype_e* arg_types;   // Types of arguments (NULL if no arguments)
   vartype_e* ret_types;   // Types of return values (NULL if void)
   bool defined;           // Whatever function was already defined
} symbolvalfn_t;

typedef struct symbolvalvar_t {
   vartype_e type;         // Type of the variable
   bool is_const;
   token_value_u const_val;
} symbolvalvar_t;

typedef union symbolval_u {
   symbolvalfn_t* fn;
   symbolvalvar_t* var;
} symbolval_u;


/**
 * Allocates and creates new symbol value for function.
 */
symbolval_u symbolval_fn_ctor(int arg_count, int ret_count, vartype_e* arg_types, vartype_e* ret_types, bool defined);

/**
 * Allocates and creates new symbol value for variable.
 */
symbolval_u symbolval_var_ctor(vartype_e type);

/**
 * Deallocates symbol function value.
 */
void symbolval_fn_dtor(symbolval_u symbolval);

/**
 * Deallocates symbol variable value.
 */
void symbolval_var_dtor(symbolval_u symbolval);
