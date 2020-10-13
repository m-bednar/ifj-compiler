/**
 * IFJ20 Compiler
 * @file main.c
 * @authors Martin Bednář (xbedna77)
 */

#include <stdlib.h>
#include <stdio.h>
#include "symtable.h"

int main() {
   btree_t* tree = btree_item_ctor();

   btree_add(tree, "hello world");
   btree_add(tree, "hello0");
   btree_add(tree, "world0");
   btree_add(tree, "hello1");
   
   printf("Has \"hello world\": %d\n", btree_has(tree, "hello world"));
   printf("Has \"world0\": %d\n", btree_has(tree, "world0"));
   
   btree_print(tree);
   
   btree_dtor(tree);
   return EXIT_SUCCESS;
}
