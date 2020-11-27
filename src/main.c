/**
 * IFJ20 Compiler
 * @file main.c
 * @authors Martin Bednář (xbedna77)
 */

#include <stdio.h>
#include <stdlib.h>
#include "parser/parser.h"
#include "parser/ast.h"
#include "symtable/symtable.h"


int main() {
   astnode_generic_t* ast = ast_ctor();
   bintree_t* symtable_global = bintree_ctor();
   parse(ast, symtable_global);
   return EXIT_SUCCESS;
}
