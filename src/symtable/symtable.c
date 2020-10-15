/**
 * IFJ20 Compiler
 * @file symtable.c
 * @authors Martin Bednář (xbedna77)
 */

#include <string.h>
#include <stdlib.h>
#include "symtable.h"
#include "../error.h"
#include "../memory.h"

bintree_t* bintree_ctor() {
   bintree_t* tree = safe_alloc(sizeof(bintree_t));
   tree->root = NULL;
   return tree;
}

void bintree_dtor(bintree_t* tree) {
   guard(tree);
   if (tree->root != NULL) {
      binitem_dtor(tree->root);
      tree->root = NULL;
   }
   free(tree);
}

void bintree_add(bintree_t* tree, symbol_t* value) {
   guard(tree != NULL);
   guard(value != NULL);
   
   if (bintree_has(tree, value->identifier)) {
      return;
   }
   if (tree->root == NULL) {
      tree->root = binitem_ctor(value);
   } else {
      binitem_insert_child(tree->root, value);
   }
}

bool bintree_has(bintree_t* tree, const char* identifier) {
   guard(tree != NULL);
   guard(identifier != NULL);
   return bintree_get(tree, identifier) != NULL;
}

symbol_t* bintree_get(bintree_t* tree, const char* identifier) {
   guard(tree != NULL);
   guard(identifier != NULL);
   if (tree->root == NULL) {
      return NULL;
   }
   // TODO: implement rest
   return NULL;
}

void bintree_print(bintree_t* tree) {
   // TODO: implement
}
