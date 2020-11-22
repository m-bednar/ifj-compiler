/**
 * IFJ20 Compiler
 * @file postfix.h
 * @authors Martin Bednář (xbedna77)
 */

#pragma once

#include "../scanner/token.h"

/**
 * Converts token array in infix notation to postfix notation. 
 * Return new lenght of the array without resizing it and destructs all additional tokens.
 */
int infix_to_postfix(token_t** infix, int length);
