/**
 * IFJ20 Compiler
 * @file precedencetable.h
 * @authors Daniel Havranek (xhavra18)
 */

#pragma once

#include "../scanner/token.h"

#define TABLE_SIZE 21

typedef enum ptablesymbol_e {
    PT_G,  // >
    PT_L,  // <
    PT_E,  // =
    PT_N   // none
} ptablesymbol_e;

typedef enum terminalid_e {
   TERMINAL_OPERATOR_OR,            // ||
   TERMINAL_OPERATOR_AND,               // &&
   TERMINAL_OPERATOR_EQUALS,            // ==
   TERMINAL_OPERATOR_NOT_EQUAL,         // !=
   TERMINAL_OPERATOR_LESS,              // <
   TERMINAL_OPERATOR_LESS_OR_EQUAL,     // <=
   TERMINAL_OPERATOR_GREATER,           // >
   TERMINAL_OPERATOR_GREATER_OR_EQUAL,  // >=
   TERMINAL_OPERATOR_ADD,               // +
   TERMINAL_OPERATOR_SUB,               // -
   TERMINAL_OPERATOR_MUL,               // *
   TERMINAL_OPERATOR_DIV,               // /
   TERMINAL_OPERATOR_NOT,               // !
   TERMINAL_LEFT_PARENTHESES,           // (
   TERMINAL_RIGHT_PARENTHESES,          // )
   TERMINAL_IDENTIFIER,
   TERMINAL_NUM,                        // integer number
   TERMINAL_NUM_DECIMAL,                // decimal number
   TERMINAL_BOOL_LITERAL,               // true / false
   TERMINAL_STRING_LITERAL,             // "string literal"
   TERMINAL_END_OF_FILE                 // EOF
} terminalid_e;

extern const int precedence_table[TABLE_SIZE][TABLE_SIZE];

/**
 * Returns terminal ID of given token ID.
 */
terminalid_e get_precedence_terminal_id(int token_id);
