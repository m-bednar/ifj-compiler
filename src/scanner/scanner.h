/**
 * IFJ20 Compiler
 * @file scanner.h
 * @authors František Lukeš (xlukes07)
 */

#pragma once

#include "token.h"

/**
 * Returns next token from stdin.
 */
token_t* get_next_token();
