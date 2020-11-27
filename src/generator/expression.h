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
 * Evaluates and generates expression to stdout.
 * Returns true, if expression result ends up on stack, false when in $tmp variable.
 */
bool generate_expression(astnode_exp_t* exp, vartable_t* vartable, bool tostack);

/**
 * Evaluates and generates defvar expression to stdout.
 */
void generate_assign_expression(char* identifier, astnode_exp_t* exp, vartable_t* vartable);
