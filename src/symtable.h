/*
 * IFJ20 Compiler
 * file: symtable.h
 * authors: 
 *    Martin Bednář, xbedna77
 */


#ifndef SYMTABLE_H
#define SYMTABLE_H

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
 * Initializes new item of binary tree.
 */
void binitem_ctor(binitem_t* item);

/**
 * Destructs item of binary tree.
 */
void binitem_dtor(binitem_t* item);

/**
 * Initializes new binary tree structure.
 */
void bintree_ctor(bintree_t* tree);

/**
 * Destructs binary tree structure.
 */
void bintree_dtor(bintree_t* tree);

/**
 * Adds new item to the tree. 
 */
void bintree_add(bintree_t* tree, char* str);

/**
 * Returns true, if tree has item of same value as string.
 */
bool bintree_has(bintree_t* tree, char* str);

#endif
