/**
 * IFJ20 Compiler
 * @file utils.h
 * @authors Martin Bednář (xbedna77)
 */

#pragma once
#include <stdio.h>
#include <stdlib.h>

#define dprint(s, ...) fprintf(stderr,"%s:%d: ", __FILE__, __LINE__); fprintf(stderr, s,##__VA_ARGS__); fprintf(stderr, "\n")
#define printcm(s, ...) printf("   "); printf(s,##__VA_ARGS__); printf("\n")  // Print command function
#define printlb(s, ...) printf(s,##__VA_ARGS__); printf("\n")   // Print label function
