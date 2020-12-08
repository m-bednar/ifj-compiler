/**
 * IFJ20 Compiler
 * @file vartable.h
 * @authors Martin Bednář (xbedna77)
 */

#pragma once

#include "../symtable/symbol.h"

typedef struct vardata_t {
   int depth;
   char* identifier;
   vartype_e type;
} vardata_t;

typedef struct vartable_t {
   int depth;
   int lenght;
   vardata_t** data;
} vartable_t;

/**
 * Allocates new vartable.
 */
vartable_t* vartable_ctor();

/**
 * Deallocates vartable.
 */
void vartable_dtor(vartable_t* vartable);

/**
 * Decreases vartable's current depth.
 */
void vartable_ascent(vartable_t* vartable);

/**
 * Increases vartable's current depth.
 */
void vartable_descent(vartable_t* vartable);

/**
 * Add new item to vartable.
 */
void vartable_add(vartable_t* vartable, char* identifier, vartype_e type);

/**
 * Returns found item depth.
 */
int vartable_depth(vartable_t* vartable, char* identifier);

/**
 * Returns true, if variable is not defined in current depth.
 * If variable is defined in current scope, it's type would be replaced in vartable.
 */
bool vartable_should_define(vartable_t* vartable, char* identifier, vartype_e type);

/**
 * Returns variable appropriate to current depth.
 */
vardata_t* vartable_find(vartable_t* vartable, char* identifier);
