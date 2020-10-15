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

void binitem_insert(binitem_t* item, symbol_t* value) {
   guard(item != NULL);
   guard(value != NULL);
   guard(item->value == NULL);
   item->value = value;
}

void binitem_insert_child(binitem_t* item, symbol_t* value) {
   guard(item != NULL);
   guard(value != NULL);
   /*if (item->value == NULL) {
      item->value = value;
   } else {
      int cmp = strcmp(item->value->identifier, value->identifier);
      if (cmp == 0) {   // identifiers are equal
         return;
      }
      if (cmp > 0) {
         if (item->right == NULL) {
            item->right = binitem_ctor(value);
         } else {
            binitem_insert(item->right, value);
         }
         
      } else {
         binitem_try_insert(item->left, value);
      }
   }*/
}
