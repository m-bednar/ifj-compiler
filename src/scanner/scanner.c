/**
 * IFJ20 Compiler
 * @file scanner.c
 * @authors František Lukeš (xlukes07)
 */

#include <stdio.h>
#include <stdlib.h>
#include "scanner.h"
#include "../memory.h"

token_t* token_ctor(tokenid id) {
   token_t* token = safe_alloc(sizeof(token_t));
   token->id = id;
   return token;
}

void token_dtor(token_t* token) {
   free(token);
}

token_t* get_next_token() {
   getc(stdin);
}

