/**
 * IFJ20 Compiler
 * @file symtable.h
 * @authors Martin Bednář (xbedna77)
 */

#pragma once

#include <stdbool.h>
#include <string.h>
#include <stdlib.h>
#include "symbol.h"
#include "binitem.h"
#include "../memory.h"

typedef struct {
   binitem_t* root;
} bintree_t;

/**
 * Initializes new binary tree structure.
 */
bintree_t* bintree_ctor();

/**
 * Destructs binary tree structure.
 */
void bintree_dtor(bintree_t* tree);

/**
 * Adds item to the tree.
 */
void bintree_add(bintree_t* tree, symbol_t* value);

/**
 * Returns true, if tree has item of given identifier.
 */
bool bintree_has(bintree_t* tree, char* identifier);

/**
 * Returns symbol specified by identifier if found, else returns NULL.
 */
symbol_t* bintree_get(bintree_t* tree, char* identifier);

/**
 * Debug function. Prints bintree on stdout.
 */
void bintree_print(bintree_t* tree);
