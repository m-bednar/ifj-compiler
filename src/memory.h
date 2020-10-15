/**
 * IFJ20 Compiler
 * @file memory.h
 * @authors Martin Bednář (xbedna77)
 */

#pragma once

#include <stdlib.h>
#include "errcodes.h"

/**
 * Allocates new memory block, or exits with non-zero value when allocation fails.
 */
void* safe_alloc(size_t size);

/**
 * Reallocates memory block, or exits with non-zero value when allocation fails.
 */
void* safe_realloc(void* ptr, size_t size);
