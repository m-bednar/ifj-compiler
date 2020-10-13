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
   btree_item_t* left;
   btree_item_t* right;
} btree_item_t;

typedef struct {
   btree_item_t* root;
} btree_t;

/**
 * Initializes new binary tree structure.
 */
btree_t* btree_ctor();

/**
 * Destructs binary tree structure.
 */
void btree_dtor(btree_t* tree);

/**
 * Adds item to the tree.
 */
void btree_add(btree_t* tree, char* value);

/**
 * Returns true, if tree has item of same value as string.
 */
bool btree_has(btree_t* tree, char* value);

/**
 * Debug function. Prints btree on stdout.
 */
void btree_print(btree_t* tree);
