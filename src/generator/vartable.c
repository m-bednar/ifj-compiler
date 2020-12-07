/**
 * IFJ20 Compiler
 * @file vartable.c
 * @authors Martin Bednář (xbedna77)
 */

#include <string.h>
#include "../error.h"
#include "utils.h"
#include "vartable.h"

vartable_t* vartable_ctor() {
   vartable_t* vartable = safe_alloc(sizeof(vartable_t));
   vartable->data = NULL;
   vartable->depth = 0;
   vartable->lenght = 0;
   return vartable;
}

void vartable_dtor(vartable_t* vartable) {
   for (int i = 0; i < vartable->lenght; i++) {
      free(vartable->data[i]->identifier);
      free(vartable->data[i]);
   }
   free(vartable);
}

void vartable_ascent(vartable_t* vartable) {
   vartable->depth--;
}

void vartable_descent(vartable_t* vartable) {
   vartable->depth++;
}

void vartable_add(vartable_t* vartable, char* identifier, vartype_e type) {
   vardata_t* data = safe_alloc(sizeof(vardata_t));
   data->identifier = safe_alloc(strlen(identifier) * sizeof(char));
   strcpy(data->identifier, identifier);
   data->depth = vartable->depth;
   data->type = type;

   vartable->data = safe_realloc(vartable->data, sizeof(vardata_t*) * (vartable->lenght + 1));
   vartable->data[vartable->lenght] = data;
   vartable->lenght++;
}

bool vartable_should_define(vartable_t* vartable, char* identifier, vartype_e type) {
   vardata_t* data = vartable_find(vartable, identifier);
   if (data == NULL) {
      return true;
   } else if (data->depth != vartable->depth) {
      return true;
   } else {
      data->type = type;
      return false;
   }
}

int vartable_depth(vartable_t* vartable, char* identifier) {
   vardata_t* data = vartable_find(vartable, identifier);
   guard(data != NULL);
   return data->depth;
}

vardata_t* vartable_find(vartable_t* vartable, char* identifier) {
   int max = -1;
   vardata_t* item = NULL;
   for (int i = 0; i < vartable->lenght; i++) {
      if (streq(identifier, vartable->data[i]->identifier)) {
         if (vartable->data[i]->depth > max && vartable->data[i]->depth <= vartable->depth) {
            item = vartable->data[i];
         }
      }
   }
   return item;
}
