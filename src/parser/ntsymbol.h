/**
 * IFJ20 Compiler
 * @file ntsymbol.h
 * @authors Daniel Havranek (xhavra18)
 */

#pragma once

#include <stdbool.h>
#include "../memory.h"
#include "../error.h"
#include "../scanner/scanner.h"

typedef enum nonterminalid_e {
    TEST_NONTERM1,
    TEST_NONTERM2,
    TEST_NONTERM3
} nonterminalid_e;

typedef union ntsymid_u {
    nonterminalid_e nonterminal;
    tokenid terminal;
} ntsymid_u;

typedef struct ntsymbol_t {
    bool is_terminal;
    ntsymid_u id;
} ntsymbol_t;


/**
 * Constucts new terminal/nonterminal symbol with given id.
 */
ntsymbol_t* ntsymbol_ctor(ntsymid_u id, bool is_terminal);

/**
 * Destructs terminal/nonterminal symbol and frees it's resources.
 */
void ntsymbol_dtor(ntsymbol_t* ntsymbol);
