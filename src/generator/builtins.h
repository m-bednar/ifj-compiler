/**
 * IFJ20 Compiler
 * @file builtins.h
 * @authors Martin Bednář (xbedna77)
 */

#pragma once

#include "vartable.h"
#include "../scanner/token.h"

/**
 * Generates built-in function call 'inputs'.
 */
void builtin_inputs();

/**
 * Generates built-in function call 'inputi'.
 */
void builtin_inputi();

/**
 * Generates built-in function call 'inputf'.
 */
void builtin_inputf();

/**
 * Generates built-in function call 'print'.
 */
void builtin_print(vartable_t* vartable, token_t** params, int param_count);

/**
 * Generates built-in function call 'int2float'.
 */
void builtin_int2float(vartable_t* vartable, token_t* param);

/**
 * Generates built-in function call 'float2int'.
 */
void builtin_float2int(vartable_t* vartable, token_t* param);

/**
 * Generates built-in function call 'len'.
 */
void builtin_len(vartable_t* vartable, token_t* param);
