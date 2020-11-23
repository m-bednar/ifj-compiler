/**
 * IFJ20 Compiler
 * @file expression.h
 * @authors Martin Bednář (xbedna77)
 */

#pragma once

#include "../symtable/bintreestack.h"
#include "../parser/ast.h"

typedef enum frametype_e {
   FT_TF,
   FT_LF,
   FT_GF
} frametype_e;

typedef enum exptype_e {
   ET_STACK,
   ET_LOCAL
} exptype_e;

/**
 * Evaluates and generates expression to stdout.
 * Returning expression type, that was used.
 */
exptype_e evaluate_expression(astnode_exp_t* exp, bintreestack_t* varstack);

/**
 * Returns string equvalent to the var used in given scope.
 */
char* str_var(char* identifier, frametype_e frame, bintreestack_t* varstack);
