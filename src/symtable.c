/*
 * IFJ20 Compiler
 * file: symtable.c
 * authors: 
 *    Martin Bednář, xbedna77
 */

#include "symtable.h"

/* --- Binary tree item --- */

void binitem_ctor(binitem_t* item, char* value) {
   // TODO: Copy value to item->value with malloc and strcpy
   item->right = NULL;
   item->left = NULL;
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
}

/* --- Binary tree --- */

void bintree_ctor(bintree_t* tree) {
   tree->root = NULL;
}

void bintree_dtor(bintree_t* tree) {
   if (tree->root != NULL) {
      binitem_dtor(tree->root);
      free(tree->root);
   }
}

void bintree_add(bintree_t* tree, char* str) {
   // TODO: implement
}

bool bintree_has(bintree_t* tree, char* str) {
   // TODO: implement
   return true;
}

bool bintree_

