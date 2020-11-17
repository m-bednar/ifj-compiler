/**
 * IFJ20 Compiler
 * @file precedencetable.c
 * @authors Daniel Havranek (xhavra18)
 */

#include "precedencetable.h"

#define TABLE_SIZE 21

const int precedence_table[TABLE_SIZE][TABLE_SIZE] = {
    {G, L, L, L, L, L, L, L, L, L, L, L, L, L, G, L, L, L, L, L, G},
    {G, G, L, L, L, L, L, L, L, L, L, L, L, L, G, L, L, L, L, L, G},
    {G, G, G, G, L, L, L, L, L, L, L, L, L, L, G, L, L, L, L, L, G},
    {G, G, G, G, L, L, L, L, L, L, L, L, L, L, G, L, L, L, L, L, G},
    {G, G, G, G, G, G, G, G, L, L, L, L, L, L, G, L, L, L, N, L, G},
    {G, G, G, G, G, G, G, G, L, L, L, L, L, L, G, L, L, L, N, L, G},
    {G, G, G, G, G, G, G, G, L, L, L, L, L, L, G, L, L, L, N, L, G},
    {G, G, G, G, G, G, G, G, L, L, L, L, L, L, G, L, L, L, N, L, G},
    {G, G, G, G, G, G, G, G, G, G, L, L, L, L, G, L, L, L, N, L, G},
    {G, G, G, G, G, G, G, G, G, G, L, L, L, L, G, L, L, L, N, N, G},
    {G, G, G, G, G, G, G, G, G, G, G, G, L, L, G, L, L, L, N, N, G},
    {G, G, G, G, G, G, G, G, G, G, G, G, L, L, G, L, L, L, N, N, G},
    {G, G, G, G, G, G, G, G, G, G, G, G, L, L, G, L, L, L, L, N, G},
    {L, L, L, L, L, L, L, L, L, L, L, L, L, L, E, L, L, L, L, L, N},
    {G, G, G, G, G, G, G, G, G, G, G, G, N, N, G, N, N, N, N, N, G},
    {G, G, G, G, G, G, G, G, G, G, G, G, N, N, G, N, N, N, N, N, G},
    {G, G, G, G, G, G, G, G, G, G, G, G, N, N, G, N, N, N, N, N, G},
    {G, G, G, G, G, G, G, G, G, G, G, G, N, N, G, N, N, N, N, N, G},
    {G, G, G, G, N, N, N, N, N, N, N, N, N, N, G, N, N, N, N, N, G},
    {G, G, G, G, G, G, G, G, G, N, N, N, N, N, G, N, N, N, N, N, G},
    {L, L, L, L, L, L, L, L, L, L, L, L, L, L, N, L, L, L, L, L, N},
};
