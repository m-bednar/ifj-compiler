/**
 * IFJ20 Compiler
 * @file binitem.c
 * @authors Martin Bednář (xbedna77)
 */

#include "binitem.h"

/**
 * Constucts new binitem with given value.
 */
binitem_t* binitem_ctor(symbol_t* value) {
   binitem_t* item = safe_alloc(sizeof(binitem_t));

   item->value = value;
   item->left = NULL;
   item->right = NULL;

   return item;
}

/**
 * Destructs binitem and frees it's resources.
 */
void binitem_dtor(binitem_t* item) {
   if (item == NULL) {
      return;
   }
   if (item->value != NULL) {
      symbol_dtor(item->value);
      item->value = NULL;
   }
   if (item->left != NULL) {
      binitem_dtor(item->left);
      item->left = NULL;
   }
   if (item->right != NULL) {
      binitem_dtor(item->right);
      item->right = NULL;
   }
   free(item);
}
