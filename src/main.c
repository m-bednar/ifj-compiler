/**
 * IFJ20 Compiler
 * @file main.c
 * @authors Martin Bednář (xbedna77)
 */

#include <stdio.h>
#include <stdlib.h>
#include "scanner/scanner.h"

int main() {
   
   token_t* token = get_next_token();
   printf("%d", token);
   token_dtor(token);

   return EXIT_SUCCESS;
}
