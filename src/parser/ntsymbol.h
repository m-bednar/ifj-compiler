/**
 * IFJ20 Compiler
 * @file ntsymbol.h
 * @authors Daniel Havranek (xhavra18)
 */

#pragma once

#include <stdbool.h>
#include "../memory.h"
#include "../error.h"
#include "../scanner/token.h"

typedef enum nonterminalid_e {
    NONTERMINAL_PROGRAM,
    NONTERMINAL_PACKAGES,
    NONTERMINAL_PACKAGE,
    NONTERMINAL_FUNCTIONS,
    NONTERMINAL_FUNCTION,
    NONTERMINAL_PARAMETERS,
    NONTERMINAL_PARAMETER,
    NONTERMINAL_PARAMETER_NEXT,
    NONTERMINAL_RETURN_TYPES,
    NONTERMINAL_TYPES,
    NONTERMINAL_TYPE,
    NONTERMINAL_TYPE_NEXT,
    NONTERMINAL_RETURN,
    NONTERMINAL_DEFINITION,
    NONTERMINAL_FOR_DEFINITION,
    NONTERMINAL_ASSIGNMENT,
    NONTERMINAL_FOR_ASSIGNMENT,
    NONTERMINAL_ASSIGNMENT_RIGHT,
    NONTERMINAL_FOR_ASSIGNMENT_RIGHT,
    NONTERMINAL_ID_NEXT,
    NONTERMINAL_COMMANDS,
    NONTERMINAL_COMMAND,
    NONTERMINAL_IF,
    NONTERMINAL_ELSE_IF,
    NONTERMINAL_ELSE,
    NONTERMINAL_FOR,
    NONTERMINAL_CALL,
    NONTERMINAL_IDS,
    NONTERMINAL_EXPRESSION,
    NONTERMINAL_EXPRESSIONS,
    NONTERMINAL_EXPRESSION_NEXT
} nonterminalid_e;

typedef struct ntsymbol_t {
    bool is_terminal;
    int id;
} ntsymbol_t;


/**
 * Constucts new terminal/nonterminal symbol with given id.
 */
ntsymbol_t* ntsymbol_ctor(int id, bool is_terminal);

/**
 * Destructs terminal/nonterminal symbol and frees it's resources.
 */
void ntsymbol_dtor(ntsymbol_t* ntsymbol);
