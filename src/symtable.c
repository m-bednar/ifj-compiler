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

btreeitem_t* btreeitem_ctor(char* value) {
   btreeitem_t* item = safe_alloc(sizeof(btreeitem_t));
   item->value = safe_alloc(strlen(value) + 1);
   item->right = NULL;
   item->left = NULL;
   strcpy(item->value, value);
   return item;
}

void btreeitem_dtor(btreeitem_t* item) {
   if (item->value != NULL) {
      free(item->value);
   }
   if (item->left != NULL) {
      btreeitem_dtor(item->left);
      free(item->left);
   }
   if (item->right != NULL) {
      btreeitem_dtor(item->right);
      free(item->right);
   }
   item->value = NULL;
   item->right = NULL;
   item->left = NULL;
}

/* --- Binary tree --- */

btree_t* btree_ctor() {
   btree_t* tree = safe_alloc(sizeof(btree_t));
   tree->root = NULL;
   return tree;
}

void btree_dtor(btree_t* tree) {
   if (tree->root != NULL) {
      btreeitem_dtor(tree->root);
      free(tree->root);
   }
   tree->root = NULL;
}

void btree_add(btree_t* tree, char* value) {
   if (btree_has(tree, value)) {
      return;
   }
   
   btreeitem_t* item = btreeitem_ctor(value);
   
   if (tree->root == NULL) {
      tree->root = item;
      return;
   }
   // TODO: implement rest
}

bool btree_has(btree_t* tree, char* value) {
   // TODO: implement
   return true;
}

void btree_print(btree_t* tree) {
   // TODO: implement
}
