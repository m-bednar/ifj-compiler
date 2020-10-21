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
