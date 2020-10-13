/**
 * IFJ20 Compiler
 * @file main.c
 * @authors Martin Bednář (xbedna77)
 */

#include <stdlib.h>
#include <stdio.h>
#include "symtable.h"

int main() {
   bintree_t *tree = binitem_ctor();

   bintree_add(tree, "hello world");
   
   bintree_dtor(tree);
   return EXIT_SUCCESS;
}
