/**
 * IFJ20 Compiler
 * @file semantic.h
 * @authors Michal Trlica (xtrlic02)
 */

#pragma once

#include "../scanner/scanner.h"
#include "ast.h"
#include "ntsymbol.h"
#include "../symtable/bintreestack.h"

/*
 * returns ERROR code or -1 if no error occured 
 */
int semantic(token_t* token, nonterminalid_e flag, int eol_flag, astnode_generic_t* ast, bintree_t* symtable_global, bool free_static_variables);

int semantic_check_undeclared_func(bintree_t* symtable_global);

int semantic_return_in_func(astnode_generic_t* ast, bintree_t* symtable_global);
