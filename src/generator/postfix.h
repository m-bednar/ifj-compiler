/**
 * IFJ20 Compiler
 * @file postfix.h
 * @authors Martin Bednář (xbedna77)
 */

#pragma once

#include "../parser/ast.h"

/**
 * Converts expression in infix notation to postfix notation.
 */
void infix_to_postfix(astnode_exp_t* exp);