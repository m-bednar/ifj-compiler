/**
 * IFJ20 Compiler
 * @file utils.h
 * @authors Martin Bednář (xbedna77)
 */

#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/**
 * Debug print. (TODO: remove)
 */
#define dprint(f, ...) fprintf(stderr,"%s:%d: "f"\n", __FILE__, __LINE__,##__VA_ARGS__)

/**
 * Print comment macro.
 */
#define pcomment(f, ...) printf("   # "f"\n",##__VA_ARGS__)

/** 
 * Print command macro.
 */
#define printcm(f, ...) printf("   "f"\n",##__VA_ARGS__)  

/** 
 * Print label macro.
 */
#define printlb(f, ...) printf(f"\n",##__VA_ARGS__)  

/** 
 * Print "jump to" label macro.
 */
#define printjl(f, ...) printf(" "f"\n",##__VA_ARGS__)  

/** 
 * Returns true if strings are equal.
 */
#define streq(s1, s2) (strcmp(s1, s2) == 0)
