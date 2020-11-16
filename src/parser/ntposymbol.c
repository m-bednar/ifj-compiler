/**
 * IFJ20 Compiler
 * @file ntposymbol.c
 * @authors Daniel Havranek (xhavra18)
 */

#include "ntposymbol.h"

ntposymbol_t* ntposymbol_ctor(int id, symtype_e type) {
    ntposymbol_t* ntposymbol = safe_alloc(sizeof(ntposymbol_t));

    ntposymbol->type = type;
    ntposymbol->id = id;
    return ntposymbol;
}

void ntposymbol_dtor(ntposymbol_t* ntposymbol) {
    guard(ntposymbol != NULL);

    free(ntposymbol);
    return;
}
