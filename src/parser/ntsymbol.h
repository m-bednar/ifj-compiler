/**
 * IFJ20 Compiler
 * @file ntsymbol.h
 * @authors Daniel Havranek (xhavra18)
 */

#pragma once

//#include <stdbool.h>
#include "../memory.h"
#include "../scanner/scanner.h"

typedef enum nonterminalid {
    // nonterminals
} nonterminalid;

typedef union ntsymid_t {
    nonterminalid nonterminal;
    tokenid terminal;
} ntsymid_t;

typedef struct ntsymbol_t {
    //bool is_terminal;
    ntsymid_t id;
} ntsymbol_t;


/**
 * Constucts new terminal/nonterminal symbol with given id.
 */
ntsymbol_t* ntsymbol_ctor(ntsymid_t id);

/**
 * Destructs terminal/nonterminal symbol and frees it's resources.
 */
void ntsymbol_dtor(ntsymbol_t* ntsymbol);
