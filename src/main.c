/**
 * IFJ20 Compiler
 * @file main.c
 * @authors Martin Bednář (xbedna77)
 */

#include <stdlib.h>
#include <stdio.h>
#include "symtable.h"

int main() {
   bintree_t* tree = binitem_ctor();

   bintree_add(tree, "hello world");
   bintree_add(tree, "hello0");
   bintree_add(tree, "world0");
   bintree_add(tree, "hello1");
   
   printf("Has \"hello world\": %d\n", bintree_has(tree, "hello world"));
   printf("Has \"world0\": %d\n", bintree_has(tree, "world0"));
   
   bintree_print(tree);
   
   bintree_dtor(tree);
   return EXIT_SUCCESS;
}
