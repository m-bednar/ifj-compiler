/**
 * IFJ20 Compiler
 * @file ast.h
 * @authors Michal Trlica (xtrlic02)
 */

#pragma once

#include <stdbool.h>
#include "../scanner/scanner.h"

typedef enum astnode_type_e {
   ANT_IF,
   ANT_FOR,
   ANT_RET,
   ANT_FUNCCALL,
   ANT_ASSIGN,
   ANT_DEFVAR
} astnode_type_e;

typedef struct astnode_codeblock_t {
   struct astnode_generic_t** children;
   int children_count;
} astnode_codeblock_t;

typedef struct astnode_global_t {
   astnode_funcdecl_t** functions;
   int functions_count;
} astnode_global_t;

typedef struct astnode_exp_t {
   token_t** tokens;
   int tokens_count;
} astnode_exp_t;

typedef struct astnode_funccall_t {
   char* name;
   astnode_exp_t** params;
   int params_count;
} astnode_funccall_t;

typedef struct astnode_assign_t {
   token_t** left_ids;
   astnode_exp_t** right_expressions; // NULL if function is there instead
   astnode_funccall_t* right_function;
   int expressions_count;
   int ids_count;
} astnode_assign_t;

typedef struct astnode_defvar_t {
   astnode_exp_t* expression;
   token_t* variable;
} astnode_defvar_t;

typedef struct astnode_ret_t {
   astnode_exp_t** expressions;
   int expressions_count;
} astnode_ret_t;

typedef struct astnode_funcdecl_t {
   char* name;
   astnode_codeblock_t* body;
} astnode_funcdecl_t;

typedef struct astnode_if_t {
   astnode_exp_t* condition;
   astnode_codeblock_t* true_body;
   astnode_codeblock_t* else_body;
} astnode_if_t;

typedef struct astnode_for_t {
   astnode_exp_t* condition;
   astnode_defvar_t* defvar;  // NULL if assign is used or first statement is empty
   astnode_assign_t* assign;  // NULL if defvar is used or first statement is empty
   astnode_exp_t* varchange;  // evaluated after each loop
   astnode_codeblock_t* body;
} astnode_for_t;

typedef union astnode_value_u {
   astnode_if_t* ifval;
   astnode_for_t* forval;
   astnode_exp_t* expval;
   astnode_funcdecl_t* funcdeclval;
   astnode_ret_t* returnval;
   astnode_global_t* globalval;
   astnode_funccall_t* funccallval;
   astnode_assign_t* assignval;
   astnode_defvar_t* defvarval;
} astnode_value_u;

typedef struct astnode_generic_t {
   astnode_type_e type;
   astnode_value_u value;
} astnode_generic_t;

/**
 * Allocate new AST
 */
astnode_generic_t* ast_ctor();
