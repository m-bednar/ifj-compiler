/**
 * IFJ20 Compiler
 * @file symbol.c
 * @authors Martin Bednář (xbedna77)
 */

#include "symbol.h"
#include "../error.h"

symbol_t* symbol_ctor(char* identifier, symboltype_e type, symbolval_u value) {
   symbol_t* symbol = safe_alloc(sizeof(symbol_t));
   
   symbol->identifier = safe_alloc(strlen(identifier) + 1);
   strcpy(symbol->identifier, identifier);

   symbol->type = type;
   symbol->value = value;
   return symbol;
}

void symbol_dtor(symbol_t* symbol) {
   guard(symbol != NULL);

   if (symbol->identifier != NULL) {
      free(symbol->identifier);
      symbol->identifier = NULL;
   }
   if (symbol->type == ST_FUNCTION) {
      symbolval_fn_dtor(symbol->value);
   } else {
      symbolval_var_dtor(symbol->value);
   }

   free(symbol);
}

void symbol_print(symbol_t* symbol) {
   guard(symbol != NULL);
   if (symbol->type == ST_VARIABLE) {
      symbolvalvar_t* val = symbol->value.var;
      fprintf(stderr, "id: %s, type: VARIABLE, value type: %d", symbol->identifier, val->type);
   } else {
      symbolvalfn_t* val = symbol->value.fn;
      fprintf(stderr, "id: %s, type: FUNCTION, args: %d, rets: %d, def: %d\n", symbol->identifier, val->arg_count, val->ret_count, val->defined);
   }
}
