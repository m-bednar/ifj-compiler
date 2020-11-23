/**
 * IFJ20 Compiler
 * @file ast.h
 * @authors Michal Trlica (xtrlic02)
 */

#pragma once

#include <stdbool.h>
#include "../scanner/scanner.h"

typedef enum astnode_type_e{
   ANT_IF,
   ANT_FOR,
   ANT_EXP,
   ANT_RET,
   ANT_FUNCDECL,
   ANT_FUNCCALL,
   ANT_ASSIGN,
   ANT_DEFVAR,
   ANT_GLOBAL
} astnode_type_e;

typedef struct astnode_globalval_t {
    struct astnode_codeblock_t* functions;
} astnode_globalval_t;

typedef struct astnode_expval_t {
   token_t** postfix;
} astnode_expval_t;

typedef struct astnode_assignval_t {
   struct astnode_codeblock_t* left_ids;
   astnode_expval_t* right_expresions;
   int expresions_count;
} astnode_assignval_t;

typedef struct astnode_defvarval_t {
   astnode_expval_t expresion;
   token_t* variable;
} astnode_defvarval_t;

typedef struct astnode_retval_t {
   astnode_expval_t* expresion;
} astnode_retval_t;

typedef struct astnode_funcdeclval_t {
   char* name;
   astnode_retval_t* return_val;
   struct astnode_codeblock_t* body;
} astnode_funcdeclval_t;

typedef struct astnode_funccallval_t {
   char* name;
   astnode_expval_t* params;
} astnode_funccallval_t;


typedef struct astnode_ifval_t {
   astnode_expval_t* condition;
   struct astnode_codeblock_t* true_body;
   struct astnode_codeblock_t* else_body;
} astnode_ifval_t;

typedef struct astnode_forval_t {
   astnode_expval_t* condition;
   struct astnode_codeblock_t* body;
} astnode_forval_t;


typedef union astnode_value_u {
   astnode_ifval_t ifval;
   astnode_forval_t forval;
   astnode_expval_t expval;
   astnode_funcdeclval_t funcdeclval;
   astnode_retval_t returnval;
   astnode_globalval_t globalval;
   astnode_funccallval_t funccallval;
   astnode_assignval_t assignval;
   astnode_defvarval_t defvarval;
} astnode_value_u;

typedef struct astnode_t{
   astnode_type_e type;
   astnode_value_u value;
   struct astnode_t* parent;
} astnode_t;

typedef struct astnode_codeblock_t {
   astnode_t** children;
   int children_count;
} astnode_codeblock_t;

/**
 * Allocate new AST
 */
astnode_t* ast_ctor();
