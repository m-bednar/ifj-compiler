/**
 * IFJ20 Compiler
 * @file main.c
 * @authors Martin Bednář (xbedna77)
 */

#include <stdlib.h>
#include <stdio.h>
#include "symtable.h"

int main() {
   binitem_t *item = malloc(sizeof(binitem_t));
   bintree_t *tree = malloc(sizeof(bintree_t));;

   bintree_ctor(tree);
   binitem_ctor(item, "hello world");

   bintree_add(tree, item);
   
   bintree_dtor(tree);

   return EXIT_SUCCESS;
}
