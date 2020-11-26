/**
 * IFJ20 Compiler
 * @file builtins.h
 * @authors Martin Bednář (xbedna77)
 */

#pragma once

#include "vartable.h"
#include "../scanner/token.h"

/**
 * Returns true if given function identifier is name of built-in function.
 */
bool is_builtin(char* identifier);

/**
 * Generates built-in function call.
 */
bool generate_builtin(char* identifier, token_t** params, int param_count, vartable_t* vartable);
