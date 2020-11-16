/**
 * IFJ20 Compiler
 * @file ntposymbol.h
 * @authors Daniel Havranek (xhavra18)
 */

#pragma once

#include <stdbool.h>
#include "../memory.h"
#include "../error.h"
#include "../scanner/token.h"
#include "ntsymbol.h"

typedef enum symtype_e {
    NONTERMINAL,
    TERMINAL,
    PRECEDENCE_OPERATOR
} symtype_e;

typedef struct ntposymbol_t {
    symtype_e type;
    int id;
} ntposymbol_t;


/**
 * Constucts new terminal/nonterminal/precedence_operator symbol with given id.
 */
ntposymbol_t* ntposymbol_ctor(int id, symtype_e type);

/**
 * Destructs terminal/nonterminal/precedence_operator symbol and frees it's resources.
 */
void ntposymbol_dtor(ntposymbol_t* ntposymbol);
