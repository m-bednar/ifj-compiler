/**
 * IFJ20 Compiler
 * @file ast.h
 * @authors Michal Trlica (xtrlic02)
 */

#include <stdbool.h>
#include "../scanner/scanner.h"

typedef enum astnodetype_e{
   TYPE_IF_VAL,
   TYPE_FOR_VAL,
   TYPE_EXP_VAL,
   TYPE_RETURN_VAL,
   TYPE_FUNC_VAL,
   TYPE_GLOBAL
} astnode_type_e;

typedef struct astnode_globalval_t {
   astnode_codeblock_t* functions;
} astnode_ifval_t;

typedef struct astnode_expval_t {
   token_t** postfix;
} astnode_ifval_t;

typedef struct astnode_retval_t {
   astnode_expval_t* expresion;
} astnode_ifval_t;

typedef struct astnode_funcval_t {
   astnode_retval_t* return_val;
   astnode_codeblock_t* body;
} astnode_ifval_t;

typedef struct astnode_ifval_t {
   astnode_expval_t* condition;
   astnode_codeblock_t* true_body;
   astnode_codeblock_t* else_body;
} astnode_ifval_t;

typedef struct astnode_forval_t {
   astnode_expval_t* condition;
   astnode_codeblock_t* body;
} astnode_ifval_t;


typedef union astnode_value_u {
   astnode_ifval_t ifval;
   astnode_forval_t forval;
   astnode_expval_t expval;
   astnode_funcval_t funcval;
   astnode_retval_t returnval;
   astnode_globalval_t globalval;
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
