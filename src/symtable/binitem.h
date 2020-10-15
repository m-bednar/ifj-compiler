/**
 * IFJ20 Compiler
 * @file binitem.h
 * @authors Martin Bednář (xbedna77)
 */

#pragma once

#include "symbol.h"
#include "../memory.h"
#include "../error.h"

typedef struct binitem_t {
   symbol_t* value;
   struct binitem* left;
   struct binitem* right;
} binitem_t;

/**
 * Constuct new binitem with given value.
 */
binitem_t* binitem_ctor(symbol_t* value);

/**
 * Destruct binitem and free it's resources.
 */
void binitem_dtor(binitem_t* item);

/**
 * Try to insert symbol to the item or one of it's child.
 */
void binitem_insert(binitem_t* item, symbol_t* value);
