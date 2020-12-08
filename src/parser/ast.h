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
   struct astnode_funcdecl_t** functions;
   int functions_count;
} astnode_global_t;

typedef struct astnode_exp_t {
   token_t** tokens;
   int tokens_count;
} astnode_exp_t;

typedef struct astnode_funccall_t {
   char* name;
   token_t** params;
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
   astnode_defvar_t* defvar;
   astnode_assign_t* assign;  // evaluated after each loop
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

void ast_global_add_func(astnode_generic_t* global, astnode_funcdecl_t* funcdecl);

astnode_funcdecl_t* astnode_funcdecl_ctor(char* name);

astnode_exp_t* astnode_exp_ctor(token_t** tokens, int token_count);

astnode_defvar_t* astnode_defvar_ctor(token_t* variable, astnode_exp_t* expression);

astnode_generic_t* astnode_generic_defvar_ctor(astnode_defvar_t* def);

void astnode_funcdecl_add(astnode_funcdecl_t* func, astnode_generic_t* child);

astnode_generic_t* astnode_ret_ctor();

void astnode_ret_add_exp(astnode_generic_t* ret, astnode_exp_t* exp);

astnode_exp_t* astnode_exp_ctor(token_t** tokens, int token_count);

astnode_generic_t* astnode_if_ctor(astnode_exp_t* condition);

void astnode_if_add_truebody(astnode_generic_t* ast_node, astnode_generic_t* new_node);

void astnode_if_add_elsebody(astnode_generic_t* ast_node, astnode_generic_t* new_node);

astnode_generic_t* astnode_for_ctor(astnode_exp_t* condition,  astnode_defvar_t* defvar, astnode_assign_t* assign);

void astnode_for_add_body(astnode_generic_t* ast_node, astnode_generic_t* new_node);

astnode_assign_t* astnode_assign_ctor();

astnode_generic_t* astnode_generic_assign_ctor(astnode_assign_t* assign);

void astnode_assign_add_ids(astnode_assign_t* ast_node, token_t** ids, int ids_count);

void astnode_assign_add_exp(astnode_assign_t* ast_node, astnode_exp_t* exp);

void astnode_assign_add_funccall(astnode_assign_t* ast_node, astnode_funccall_t* funccall);

astnode_funccall_t* astnode_funccall_ctor(char* name, token_t** params, int params_count);

astnode_generic_t* astnode_generic_funccall_ctor(astnode_funccall_t* funccall);

void ast_dtor(astnode_generic_t* root_global);

void astnode_exp_dtor(astnode_exp_t* ast_node);

void astnode_defvar_dtor(astnode_defvar_t* ast_node);

void astnode_assign_dtor(astnode_assign_t* ast_node);

void astnode_funccall_dtor(astnode_funccall_t* ast_node);

void astnode_ret_dtor(astnode_ret_t* ast_node);

void astnode_for_dtor(astnode_for_t* ast_node);

void astnode_if_dtor(astnode_if_t* ast_node);

void astnode_codeblock_dtor(astnode_codeblock_t* ast_node);

void astnode_funcdecl_dtor(astnode_funcdecl_t* ast_node);
