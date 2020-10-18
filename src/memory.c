/**
 * IFJ20 Compiler
 * @file memory.c
 * @authors Martin Bednář (xbedna77)
 */

#include "memory.h"
#include "error.h"

void* safe_alloc(size_t size) {
   void* memory = malloc(size);
   if (memory == NULL) {
      exit(ERRCODE_INTERNAL_ERROR);
   }
   return memory;
}

void* safe_realloc(void* ptr, size_t size) {
   void* memory = realloc(ptr, size);
   if (memory == NULL) {
      exit(ERRCODE_INTERNAL_ERROR);
   }
   return memory;
}
