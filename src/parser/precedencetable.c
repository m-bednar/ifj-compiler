/**
 * IFJ20 Compiler
 * @file precedencetable.c
 * @authors Daniel Havranek (xhavra18)
 */

#include "precedencetable.h"

const int precedence_table[TABLE_SIZE][TABLE_SIZE] = {
    {PT_G, PT_L, PT_L, PT_L, PT_L, PT_L, PT_L, PT_L, PT_L, PT_L, PT_L, PT_L, PT_L, PT_L, PT_G, PT_L, PT_L, PT_L, PT_L, PT_L, PT_G},
    {PT_G, PT_G, PT_L, PT_L, PT_L, PT_L, PT_L, PT_L, PT_L, PT_L, PT_L, PT_L, PT_L, PT_L, PT_G, PT_L, PT_L, PT_L, PT_L, PT_L, PT_G},
    {PT_G, PT_G, PT_G, PT_G, PT_L, PT_L, PT_L, PT_L, PT_L, PT_L, PT_L, PT_L, PT_L, PT_L, PT_G, PT_L, PT_L, PT_L, PT_L, PT_L, PT_G},
    {PT_G, PT_G, PT_G, PT_G, PT_L, PT_L, PT_L, PT_L, PT_L, PT_L, PT_L, PT_L, PT_L, PT_L, PT_G, PT_L, PT_L, PT_L, PT_L, PT_L, PT_G},
    {PT_G, PT_G, PT_G, PT_G, PT_G, PT_G, PT_G, PT_G, PT_L, PT_L, PT_L, PT_L, PT_L, PT_L, PT_G, PT_L, PT_L, PT_L, PT_N, PT_L, PT_G},
    {PT_G, PT_G, PT_G, PT_G, PT_G, PT_G, PT_G, PT_G, PT_L, PT_L, PT_L, PT_L, PT_L, PT_L, PT_G, PT_L, PT_L, PT_L, PT_N, PT_L, PT_G},
    {PT_G, PT_G, PT_G, PT_G, PT_G, PT_G, PT_G, PT_G, PT_L, PT_L, PT_L, PT_L, PT_L, PT_L, PT_G, PT_L, PT_L, PT_L, PT_N, PT_L, PT_G},
    {PT_G, PT_G, PT_G, PT_G, PT_G, PT_G, PT_G, PT_G, PT_L, PT_L, PT_L, PT_L, PT_L, PT_L, PT_G, PT_L, PT_L, PT_L, PT_N, PT_L, PT_G},
    {PT_G, PT_G, PT_G, PT_G, PT_G, PT_G, PT_G, PT_G, PT_G, PT_G, PT_L, PT_L, PT_L, PT_L, PT_G, PT_L, PT_L, PT_L, PT_N, PT_L, PT_G},
    {PT_G, PT_G, PT_G, PT_G, PT_G, PT_G, PT_G, PT_G, PT_G, PT_G, PT_L, PT_L, PT_L, PT_L, PT_G, PT_L, PT_L, PT_L, PT_N, PT_N, PT_G},
    {PT_G, PT_G, PT_G, PT_G, PT_G, PT_G, PT_G, PT_G, PT_G, PT_G, PT_G, PT_G, PT_L, PT_L, PT_G, PT_L, PT_L, PT_L, PT_N, PT_N, PT_G},
    {PT_G, PT_G, PT_G, PT_G, PT_G, PT_G, PT_G, PT_G, PT_G, PT_G, PT_G, PT_G, PT_L, PT_L, PT_G, PT_L, PT_L, PT_L, PT_N, PT_N, PT_G},
    {PT_G, PT_G, PT_G, PT_G, PT_G, PT_G, PT_G, PT_G, PT_G, PT_G, PT_G, PT_G, PT_L, PT_L, PT_G, PT_L, PT_L, PT_L, PT_L, PT_N, PT_G},
    {PT_L, PT_L, PT_L, PT_L, PT_L, PT_L, PT_L, PT_L, PT_L, PT_L, PT_L, PT_L, PT_L, PT_L, PT_E, PT_L, PT_L, PT_L, PT_L, PT_L, PT_N},
    {PT_G, PT_G, PT_G, PT_G, PT_G, PT_G, PT_G, PT_G, PT_G, PT_G, PT_G, PT_G, PT_N, PT_N, PT_G, PT_N, PT_N, PT_N, PT_N, PT_N, PT_G},
    {PT_G, PT_G, PT_G, PT_G, PT_G, PT_G, PT_G, PT_G, PT_G, PT_G, PT_G, PT_G, PT_N, PT_N, PT_G, PT_N, PT_N, PT_N, PT_N, PT_N, PT_G},
    {PT_G, PT_G, PT_G, PT_G, PT_G, PT_G, PT_G, PT_G, PT_G, PT_G, PT_G, PT_G, PT_N, PT_N, PT_G, PT_N, PT_N, PT_N, PT_N, PT_N, PT_G},
    {PT_G, PT_G, PT_G, PT_G, PT_G, PT_G, PT_G, PT_G, PT_G, PT_G, PT_G, PT_G, PT_N, PT_N, PT_G, PT_N, PT_N, PT_N, PT_N, PT_N, PT_G},
    {PT_G, PT_G, PT_G, PT_G, PT_N, PT_N, PT_N, PT_N, PT_N, PT_N, PT_N, PT_N, PT_N, PT_N, PT_G, PT_N, PT_N, PT_N, PT_N, PT_N, PT_G},
    {PT_G, PT_G, PT_G, PT_G, PT_G, PT_G, PT_G, PT_G, PT_G, PT_N, PT_N, PT_N, PT_N, PT_N, PT_G, PT_N, PT_N, PT_N, PT_N, PT_N, PT_G},
    {PT_L, PT_L, PT_L, PT_L, PT_L, PT_L, PT_L, PT_L, PT_L, PT_L, PT_L, PT_L, PT_L, PT_L, PT_N, PT_L, PT_L, PT_L, PT_L, PT_L, PT_N},
};

terminalid_e get_precedence_terminal_id(int token_id) {
    switch(token_id) {
        case TOKENID_OPERATOR_OR:
            return TERMINAL_OPERATOR_OR;
        case TOKENID_OPERATOR_AND:
            return TERMINAL_OPERATOR_AND;
        case TOKENID_OPERATOR_EQUALS:
            return TERMINAL_OPERATOR_EQUALS;
        case TOKENID_OPERATOR_NOT_EQUAL:
            return TERMINAL_OPERATOR_NOT_EQUAL;
        case TOKENID_OPERATOR_LESS:
            return TERMINAL_OPERATOR_LESS;
        case TOKENID_OPERATOR_LESS_OR_EQUAL:
            return TERMINAL_OPERATOR_LESS_OR_EQUAL;
        case TOKENID_OPERATOR_GREATER:
            return TERMINAL_OPERATOR_GREATER;
        case TOKENID_OPERATOR_GREATER_OR_EQUAL:
            return TERMINAL_OPERATOR_GREATER_OR_EQUAL;
        case TOKENID_OPERATOR_ADD:
            return TERMINAL_OPERATOR_ADD;
        case TOKENID_OPERATOR_SUB:
            return TERMINAL_OPERATOR_SUB;
        case TOKENID_OPERATOR_MUL:
            return TERMINAL_OPERATOR_MUL;
        case TOKENID_OPERATOR_DIV:
            return TERMINAL_OPERATOR_DIV;
        case TOKENID_OPERATOR_NOT:
            return TERMINAL_OPERATOR_NOT;
        case TOKENID_LEFT_PARENTHESES:
            return TERMINAL_LEFT_PARENTHESES;
        case TOKENID_RIGHT_PARENTHESES:
            return TERMINAL_RIGHT_PARENTHESES;
        case TOKENID_IDENTIFIER:
            return TERMINAL_IDENTIFIER;
        case TOKENID_NUM:
            return TERMINAL_NUM;
        case TOKENID_NUM_DECIMAL:
            return TERMINAL_NUM_DECIMAL;
        case TOKENID_BOOL_LITERAL:
            return TERMINAL_BOOL_LITERAL;
        case TOKENID_STRING_LITERAL:
            return TERMINAL_STRING_LITERAL;
        case TOKENID_END_OF_FILE:
            return TERMINAL_END_OF_FILE;
        default:
            return TERMINAL_END_OF_FILE;
    }
}
