/**
 * IFJ20 Compiler
 * @file parser.h
 * @authors Daniel Havranek (xhavra18)
 */

#pragma once

#include "ntsymbol.h"
#include "ntsymstack.h"
#include "ntposymbol.h"
#include "ntposymstack.h"
#include "precedencetable.h"
#include "../scanner/scanner.h"
#include "ast.h"
#include "../symtable/symtable.h"

/**
 * Parses tokens recieved from scanner.
 * Returns AST.
 */
void parse(astnode_generic_t* ast, bintree_t* symtable_global);
