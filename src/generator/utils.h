/**
 * IFJ20 Compiler
 * @file utils.h
 * @authors Martin Bednář (xbedna77)
 */

#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define dprint(f, ...) fprintf(stderr,"%s:%d: "f"\n", __FILE__, __LINE__,##__VA_ARGS__)
#define pcomment(f, ...) printf("   # "f"\n",##__VA_ARGS__)  // Print comment function
#define printcm(f, ...) printf("   "f"\n",##__VA_ARGS__)  // Print command function
#define printlb(f, ...) printf(f"\n",##__VA_ARGS__)   // Print label function
#define streq(s1, s2) (strcmp(s1, s2) == 0)
