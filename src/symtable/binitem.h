/**
 * IFJ20 Compiler
 * @file binitem.h
 * @authors Martin Bednář (xbedna77)
 */

#pragma once

#include <stdbool.h>
#include "symbol.h"
#include "../memory.h"
#include "../error.h"

typedef enum idcompare_e {
   IC_EQUAL,
   IC_RIGHT,
   IC_LEFT
} idcompare_e;

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
 * Returns false, when symbol with same identifier already exists.
 */
bool binitem_insert_child(binitem_t* item, symbol_t* value);

/**
 * Prints item and it's children to stderr.
 */
void binitem_print(binitem_t* item);

/**
 * Adds binitem content and it's children content to array, increasing length.
 */
void binitem_to_array(binitem_t* item, symbol_t** array, int* length);

/**
 * Compare identifier with item's identifier, returns ID_EQUAL when equal, ID_LEFT value when left item's
 * child should be next to check, ID_RIGHT value when right item should be next to check.
 */
idcompare_e binitem_compare(binitem_t* item, const char* identifier);
