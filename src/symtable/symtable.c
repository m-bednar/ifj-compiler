/**
 * IFJ20 Compiler
 * @file symtable.c
 * @authors Martin Bednář (xbedna77)
 */

#include "symtable.h"

bintree_t* bintree_ctor() {
   bintree_t* tree = safe_alloc(sizeof(bintree_t));
   tree->root = NULL;
   return tree;
}

void bintree_dtor(bintree_t* tree) {
   if (tree->root != NULL) {
      binitem_dtor(tree->root);
      tree->root = NULL;
   }
   free(tree);
}

void bintree_add(bintree_t* tree, symbol_t* value) {
   if (bintree_has(tree, value)) {
      return;
   }
   // TODO: implement rest
}

bool bintree_has(bintree_t* tree, char* identifier) {
   // TODO: implement
   return true;
}

symbol_t* bintree_get(bintree_t* tree, char* identifier) {
   // TODO: implement
   return NULL;
}

void bintree_print(bintree_t* tree) {
   // TODO: implement
}
