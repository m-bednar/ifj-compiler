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

int binitem_compare(binitem_t* item, const char* identifier) {
   guard(item != NULL);
   guard(identifier != NULL);

   int cmp = strcmp(item->value->identifier, identifier);
   if (cmp == 0) {   // identifiers are equal
      return ID_EQUAL;
   }
   if (cmp > 0) {
      return ID_RIGHT;
   } else {
      return ID_LEFT;
   }
}

void binitem_insert_child(binitem_t* item, symbol_t* value) {
   guard(item != NULL);
   guard(value != NULL);

   switch (binitem_compare(item, value->identifier)) {
      case ID_EQUAL:
         return;
      case ID_RIGHT:
         if (item->right == NULL) {
            item->right = binitem_ctor(value);
         } else {
            binitem_insert_child(item->right, value);
         }
         break;
      case ID_LEFT:
         if (item->left == NULL) {
            item->left = binitem_ctor(value);
         } else {
            binitem_insert_child(item->left, value);
         }
         break;
   }
}
