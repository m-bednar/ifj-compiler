/**
 * IFJ20 Compiler
 * @file symbol.c
 * @authors Martin Bednář (xbedna77)
 */

#include "symbol.h"
#include "../error.h"

symbol_t* symbol_ctor(char* identifier) {
   symbol_t* symbol = safe_alloc(sizeof(symbol_t));
   
   symbol->identifier = safe_alloc(strlen(identifier) + 1);
   strcpy(symbol->identifier, identifier);
   
   return symbol;
}

void symbol_dtor(symbol_t* symbol) {
   guard(symbol != NULL);

   if (symbol->identifier != NULL) {
      free(symbol->identifier);
      symbol->identifier = NULL;
   }

   free(symbol);
}
