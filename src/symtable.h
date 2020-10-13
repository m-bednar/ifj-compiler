/**
 * IFJ20 Compiler
 * @file symtable.h
 * @authors Martin Bednář (xbedna77)
 */

#pragma once

#include <stdbool.h>
#include <stdlib.h>

typedef struct {
   const char* value;
   binitem_t* left;
   binitem_t* right;
} binitem_t;

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
void bintree_add(bintree_t* tree, char* value);

/**
 * Returns true, if tree has item of same value as string.
 */
bool bintree_has(bintree_t* tree, char* value);

/**
 * Debug function. Prints bintree on stdout.
 */
void bintree_print(bintree_t* tree);
