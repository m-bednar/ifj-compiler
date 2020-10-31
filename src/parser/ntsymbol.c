/**
 * IFJ20 Compiler
 * @file ntsymbol.c
 * @authors Daniel Havranek (xhavra18)
 */

#include "ntsymbol.h"

ntsymbol_t* ntsymbol_ctor(ntsymid_t id) {
    ntsymbol_t* ntsymbol = safe_alloc(sizeof(ntsymbol_t));

    ntsymbol->id = id;
    return ntsymbol;
}

void ntsymbol_dtor(ntsymbol_t* ntsymbol) {
    guard(ntsymbol != NULL);

    free(ntsymbol);
    return;
}
