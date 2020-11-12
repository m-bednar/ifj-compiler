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
    NONTERM_PROGRAM,
    NONTERM_PACKAGES,
    NONTERM_PACKAGE,
    NONTERM_FUNCTIONS,
    NONTERM_FUNCTION,
    NONTERM_PARAMETERS,
    NONTERM_PARAMETER,
    NONTERM_PARAMETER_NEXT,
    NONTERM_RETURN_TYPES,
    NONTERM_TYPES,
    NONTERM_TYPE,
    NONTERM_TYPE_NEXT,
    NONTERM_RETURN,
    NONTERM_DEFINITION,
    NONTERM_FOR_DEFINITION,
    NONTERM_ASSIGNMENT,
    NONTERM_FOR_ASSIGNMENT,
    NONTERM_ASSIGNMENT_RIGHT,
    NONTERM_FOR_ASSIGNMENT_RIGHT,
    NONTERM_ID_NEXT,
    NONTERM_COMMANDS,
    NONTERM_COMMAND,
    NONTERM_IF,
    NONTERM_ELSE,
    NONTERM_FOR,
    NONTERM_CALL,
    NONTERM_IDS,
    NONTERM_EXPRESSION,
    NONTERM_EXPRESSIONS,
    NONTERM_EXPRESSION_NEXT
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
