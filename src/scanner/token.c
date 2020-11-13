/**
 * IFJ20 Compiler
 * @file token.c
 * @authors Martin Bednář (xbedna77)
 */

#include "token.h"
#include "../error.h"
#include "../memory.h"

bool is_memory_allocated(tokenid_e id) {
   const tokenid_e ids[] = {
      TOKENID_IDENTIFIER,
      TOKENID_STRING_LITERAL
   }; // Array of all ids that use memory
   int size = sizeof(ids) / sizeof(ids[0]);
   for (int i = 0; i < size; i++) {
      if (ids[i] == id) {
         return true;
      }
   }
   return false;
}

token_t* token_ctor(tokenid_e id, token_value_u value) {
   token_t* token = safe_alloc(sizeof(token_t));
   token->id = id;
   token->value = value;
   return token;
}

void token_dtor(token_t* token) {
   guard(token != NULL);
   if (is_memory_allocated(token->id)) {
      free(token->value.string_value);
   }
   free(token);
}

