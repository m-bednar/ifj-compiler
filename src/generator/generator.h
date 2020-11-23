/**
 * IFJ20 Compiler
 * @file generator.h
 * @authors Martin Bednář (xbedna77)
 */

#pragma once

#include "../symtable/symtable.h"
#include "../parser/ast.h"

void generate(astnode_global_t* global, bintree_t* fnsymtable);
