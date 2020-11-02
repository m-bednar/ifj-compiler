/**
 * IFJ20 Compiler
 * @file binitem.c
 * @authors Martin Bednář (xbedna77)
 */

#include "binitem.h"

binitem_t* binitem_ctor(symbol_t* value) {
   binitem_t* item = safe_alloc(sizeof(binitem_t));

   item->value = value;
   item->left = NULL;
   item->right = NULL;

   return item;
}

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

idcompare_e binitem_compare(binitem_t* item, const char* identifier) {
   guard(item != NULL);
   guard(identifier != NULL);

   int cmp = strcmp(item->value->identifier, identifier);
   if (cmp == 0) {   // identifiers are equal
      return IC_EQUAL;
   }
   if (cmp > 0) {
      return IC_RIGHT;
   } else {
      return IC_LEFT;
   }
}

bool binitem_insert_child(binitem_t* item, symbol_t* value) {
   guard(item != NULL);
   guard(value != NULL);

   switch (binitem_compare(item, value->identifier)) {
      case IC_EQUAL:
         return false;
      case IC_RIGHT:
         if (item->right == NULL) {
            item->right = binitem_ctor(value);
            return true;
         } else {
            return binitem_insert_child(item->right, value);
         }
         break;
      case IC_LEFT:
         if (item->left != NULL) {
            item->left = binitem_ctor(value);
            return true;
         } else {
            return binitem_insert_child(item->left, value);
         }
         break;
   }
   return true;
}

void binitem_print(binitem_t* item) {
   guard(item != NULL);

   if (item->value != NULL) {
      symbol_print(item->value);
   }
   if (item->left != NULL) {
      binitem_print(item->left);
   }
   if (item->right != NULL) {
      binitem_print(item->right);
   }
}
