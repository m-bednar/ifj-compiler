#include "symtable.h"
#include <stdlib.h>

void bintree_ctor(bintree_t* tree) {
   tree->root = NULL;
}

void bintree_dtor(bintree_t* tree) {
   if (tree->root != NULL) {
      binitem_dtor(tree->root);
      free(tree->root);
   }
}

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
