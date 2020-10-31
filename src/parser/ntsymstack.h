/**
 * IFJ20 Compiler
 * @file ntsymstack.h
 * @authors Daniel Havranek (xhavra18)
 */

#pragma once

typedef struct ntsymstack {
   int length;
   int capacity;
   // ntsymbol_t** memory;
} ntsymstack;
