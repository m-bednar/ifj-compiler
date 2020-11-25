/**
 * IFJ20 Compiler
 * @file expression.h
 * @authors Martin Bednář (xbedna77)
 */

#pragma once

#include "../symtable/bintreestack.h"
#include "../parser/ast.h"


/**
 * Evaluates and generates defvar expression to stdout.
 */
void generate_assign_expression(char* identifier, char* varstr, astnode_exp_t* exp, bintreestack_t* stack, bool can_const);
