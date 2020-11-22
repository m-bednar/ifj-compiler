/**
 * IFJ20 Compiler
 * @file postfix.h
 * @authors Martin Bednář (xbedna77)
 */

#pragma once

#include "../scanner/token.h"

int infix_to_postfix(token_t** infix, int length);
