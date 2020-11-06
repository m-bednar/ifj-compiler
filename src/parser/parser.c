/**
 * IFJ20 Compiler
 * @file parser.c
 * @authors Daniel Havranek (xhavra18)
 */

#include "parser.h"

void parse() {
    ntsymstack_t *stack = ntsymstack_ctor();

    ntsymbol_t *end_of_file = ntsymbol_ctor(TOKENID_END_OF_FILE, true);
    ntsymbol_t *program = ntsymbol_ctor(NONTERM_PROGRAM, false);

    ntsymstack_push(stack, end_of_file);
    ntsymstack_push(stack, program);

    // parsing cycle

    ntsymbol_dtor(program);
    ntsymbol_dtor(end_of_file);
   
    return;
}
