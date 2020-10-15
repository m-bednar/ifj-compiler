/**
 * IFJ20 Compiler
 * @file symbol.h
 * @authors Martin Bednář (xbedna77)
 */

#pragma once

#include <string.h>
#include "../memory.h"

typedef struct {
   const char* identifier;
} symbol_t;

/**
 * Constucts new symbol with given values.
 */
symbol_t* symbol_ctor(char* identifier);

/**
 * Destructs symbol and frees it's resources.
 */
void symbol_dtor(symbol_t* symbol);
