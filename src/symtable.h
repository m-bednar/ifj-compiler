#ifndef SYMTABLE_H
#define SYMTABLE_H

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
void bintree_ctor(bintree_t* tree);

/**
 * Destructs binary tree structure.
 */
void bintree_dtor(bintree_t* tree);

/**
 * Initializes new item of binary tree.
 */
void binitem_ctor(binitem_t* item);

/**
 * Destructs item of binary tree.
 */
void binitem_dtor(binitem_t* item);

#endif
