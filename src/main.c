/**
 * IFJ20 Compiler
 * @file main.c
 * @authors Martin Bednář (xbedna77)
 */

#include <stdio.h>
#include <stdlib.h>
#include "symtable/symtable.h"

int main() {
   bintree_t* tree = bintree_ctor();

   bintree_dtor(tree);
   return EXIT_SUCCESS;
}
