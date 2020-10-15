/**
 * IFJ20 Compiler
 * @file binitem.h
 * @authors Martin Bednář (xbedna77)
 */

#pragma once

#include "symbol.h"
#include "../memory.h"

typedef struct {
   symbol_t* value;
   binitem_t* left;
   binitem_t* right;
} binitem_t;

/**
 * Constucts new binitem with given value.
 */
binitem_t* binitem_ctor(symbol_t* value);

/**
 * Destructs binitem and frees it's resources.
 */
void binitem_dtor(binitem_t* item);
