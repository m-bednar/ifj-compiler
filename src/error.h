/**
 * IFJ20 Compiler
 * @file error.h
 * @authors Martin Bednář (xbedna77)
 */

#pragma once

#include <stdio.h>
#include "errcodes.h"

#define error(msg) fprintf(stderr, "Error at %s:%d: %s\n", __FILE__, __LINE__, msg); exit(ERRCODE_INTERNAL_ERROR)
#define guard(cond) if (!(cond)) { error(#cond); }
