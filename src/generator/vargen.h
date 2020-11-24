/**
 * IFJ20 Compiler
 * @file vargen.h
 * @authors Martin Bednář (xbedna77)
 */

#pragma once

#include "../symtable/bintreestack.h"
#include <stdbool.h>

typedef enum frametype_e {
   FT_TF,
   FT_LF,
   FT_GF
} frametype_e;

/**
 * Returns depth that should be used for newly defined variable.
 */
int new_var_depth(bintreestack_t* varstack);

/**
 * Returns depth of existing variable.
 */
int get_var_depth(char* identifier, bintreestack_t* varstack);

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
