/**
 * IFJ20 Compiler
 * @file symtable.h
 * @authors Martin Bednář (xbedna77)
 */

#pragma once

#include <stdbool.h>
#include "symbol.h"
#include "binitem.h"

typedef struct bintree_t {
   binitem_t* root;
} bintree_t;

/**
 * Allocate new binary tree structure.
 */
bintree_t* bintree_ctor();

/**
 * Destruct binary tree structure and free it's resources.
 */
void bintree_dtor(bintree_t* tree);

/**
 * Add item to the tree.
 */
void bintree_add(bintree_t* tree, symbol_t* value);

/**
 * Returns true, if tree has item of the given identifier.
 */
bool bintree_has(bintree_t* tree, const char* identifier);

/**
 * Returns symbol, specified by identifier, if found, NULL if not found.
 */
symbol_t* bintree_get(bintree_t* tree, const char* identifier);
