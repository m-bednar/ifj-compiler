/**
 * IFJ20 Compiler
 * @file symtable.c
 * @authors Martin Bednář (xbedna77)
 */

#include <string.h>
#include "errcodes.h"
#include "symtable.h"

void* safe_alloc(size_t size) {
   void* memory = malloc(size);
   if (memory == NULL) {
      exit(ERRCODE_INTERNAL_ERROR);
   }
   return memory;
}

/* --- Binary tree item --- */

binitem_t* binitem_ctor(char* value) {
   binitem_t* item = safe_alloc(sizeof(binitem_t));
   item->value = safe_alloc(strlen(value) + 1);
   item->right = NULL;
   item->left = NULL;
   strcpy(item->value, value);
   return item;
}

void binitem_dtor(binitem_t* item) {
   if (item->value != NULL) {
      free(item->value);
   }
   if (item->left != NULL) {
      binitem_dtor(item->left);
      free(item->left);
   }
   if (item->right != NULL) {
      binitem_dtor(item->right);
      free(item->right);
   }
   item->value = NULL;
   item->right = NULL;
   item->left = NULL;
}

/* --- Binary tree --- */

bintree_t* bintree_ctor() {
   bintree_t* tree = safe_alloc(sizeof(bintree_t));
   tree->root = NULL;
   return tree;
}

void bintree_dtor(bintree_t* tree) {
   if (tree->root != NULL) {
      binitem_dtor(tree->root);
      free(tree->root);
   }
   tree->root = NULL;
}

void bintree_add(bintree_t* tree, char* value) {
   if (bintree_has(tree, value)) {
      return;
   }
   
   binitem_t* item = binitem_ctor(value);
   
   if (tree->root == NULL) {
      tree->root = item;
      return;
   }
   // TODO: implement rest
}

bool bintree_has(bintree_t* tree, char* value) {
   // TODO: implement
   return true;
}
