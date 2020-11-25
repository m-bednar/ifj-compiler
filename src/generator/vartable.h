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

vartable_t* vartable_ctor();

void vartable_dtor(vartable_t* vartable);

void vartable_ascent(vartable_t* vartable);

void vartable_descent(vartable_t* vartable);

void vartable_add(vartable_t* vartable, char* identifier, vartype_e type);

int vartable_depth(vartable_t* vartable, char* identifier);

bool vartable_should_define(vartable_t* vartable, char* identifier, vartype_e type);

vardata_t* vartable_find(vartable_t* vartable, char* identifier);
