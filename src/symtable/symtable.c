/**
 * IFJ20 Compiler
 * @file symtable.c
 * @authors Martin Bednář (xbedna77)
 */

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

bool bintree_add(bintree_t* tree, symbol_t* value) {
   guard(tree != NULL);
   guard(value != NULL);
   
   if (tree->root == NULL) {
      tree->root = binitem_ctor(value);
      return true;
   } else {
      return binitem_insert_child(tree->root, value);
   }
}

symbol_t* bintree_find(bintree_t* tree, const char* identifier) {
   guard(tree != NULL);
   guard(identifier != NULL);
   
   if (tree->root == NULL) {
      return NULL;
   }

   binitem_t* item = tree->root;
   do {
      switch (binitem_compare(item, identifier)) {
         case IC_EQUAL:
            return item->value;
         case IC_RIGHT:
            item = item->right;
            break;
         case IC_LEFT:
            item = item->left;
            break;
      }
   } while (item != NULL);

   return NULL;
}

symbol_t** bintree_to_array(bintree_t* tree, int* length) {
   guard(tree != NULL);

   symbol_t** array = NULL;
   *length = 0;
   if (tree->root != NULL) {
      binitem_to_array(tree->root, &array, length);
   }
   
   return array;
}

void bintree_print(bintree_t* tree) {
   guard(tree != NULL);
   if (tree->root == NULL) {
      fprintf(stderr, "(empty symtable)");
   } else {
      binitem_print(tree->root);
   }
}
