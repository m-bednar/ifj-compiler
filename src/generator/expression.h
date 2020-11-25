/**
 * IFJ20 Compiler
 * @file expression.h
 * @authors Martin Bednář (xbedna77)
 */

#pragma once

#include "vartable.h"
#include "../parser/ast.h"

/**
 * Determines resulting type of an expression.
 */
vartype_e determine_expression_type(astnode_exp_t* exp, vartable_t* vartable);

/**
 * Evaluates and generates defvar expression to stdout.
 * Returns vartype of expression.
 */
void generate_assign_expression(char* identifier, astnode_exp_t* exp, vartable_t* vartable);
