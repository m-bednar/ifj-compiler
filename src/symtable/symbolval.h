/**
 * IFJ20 Compiler
 * @file symbolval.h
 * @authors Martin Bednář (xbedna77)
 */

#pragma once

typedef enum vartype {
   VT_VOID,
   VT_BOOL,
   VT_INT,
   VT_FLOAT,
   VT_STRING
} vartype;

typedef struct symbolvalfn_t {
   int arg_count;
   int ret_count;
   vartype* arg_types;
   vartype* ret_types;
} symbolvalfn_t;

typedef struct symbolvalvar_t {
   vartype type;
} symbolvalvar_t;

typedef union symbolval_u {
   symbolvalfn_t* fn;
   symbolvalvar_t* var;
} symbolval_u;


/**
 * Allocates and creates new symbol value for function.
 */
symbolval_u symbolval_fn_ctor(int arg_count, int ret_count, vartype* arg_types, vartype* ret_types);

/**
 * Allocates and creates new symbol value for variable.
 */
symbolval_u symbolval_var_ctor(vartype type);

/**
 * Deallocates symbol function value.
 */
void symbolval_fn_dtor(symbolval_u symbolval);

/**
 * Deallocates symbol variable value.
 */
void symbolval_var_dtor(symbolval_u symbolval);
