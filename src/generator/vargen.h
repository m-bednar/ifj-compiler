/**
 * IFJ20 Compiler
 * @file vargen.h
 * @authors Martin Bednář (xbedna77)
 */

#pragma once

#include "../scanner/token.h"
#include <stdbool.h>

typedef enum frametype_e {
   FT_TF,
   FT_LF,
   FT_GF
} frametype_e;

char* labelgen_new();

char* labelgen_use();

/**
 * Returns true, if id belongs to token with constant value.
 */
bool is_const_tokenid(tokenid_e id);

/**
 * Creates new string with correct variable signature.
 */
char* generate_var_str(char* identifier, frametype_e frame, int depth);

/**
 * Generates constant variable signature from token.
 */
char* generate_const_str(token_t* token);
