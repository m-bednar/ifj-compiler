/**
 * IFJ20 Compiler
 * @file binitem.h
 * @authors Martin Bednář (xbedna77)
 */

#pragma once

#include "symbol.h"
#include "../memory.h"
#include "../error.h"

enum idcompare {
   ID_EQUAL,
   ID_RIGHT,
   ID_LEFT
};

typedef struct binitem_t {
   symbol_t* value;
   struct binitem_t* left;
   struct binitem_t* right;
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
 * Insert symbol to the one of item's child.
 */
void binitem_insert_child(binitem_t* item, symbol_t* value);

/**
 * Compare identifier with item's identifier, returns ID_EQUAL when equal, ID_LEFT value when left item's
 * child should be next to check, ID_RIGHT value when right item should be next to check.
 */
int binitem_compare(binitem_t* item, const char* identifier);
