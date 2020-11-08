/**
 * IFJ20 Compiler
 * @file error.h
 * @authors Martin Bednář (xbedna77)
 */

#pragma once

#include <stdio.h>

// Error codes
#define ERRCODE_LEXICAL_ERROR 1           /** Lexical error. */
#define ERRCODE_SYNTAX_ERROR 2            /** Syntax error. */
#define ERRCODE_VAR_UNDEFINED_ERROR 3     /** Undefined variable/function or function redefinition. */
#define ERRCODE_TYPE_DERIVATION_ERROR 4   /** Type cannot be derivated or infered. */
#define ERRCODE_TYPE_INCOMPATIBLE_ERROR 5 /** Incompatible types in binary expression. */
#define ERRCODE_ARGS_OR_RETURN_ERROR 6    /** Wrong number of params is passed or wrong number of values returned. */
#define ERRCODE_GENERAL_SEMANTIC_ERROR 7  /** Other semantic errors. */
#define ERRCODE_ZERO_DIVISION_ERROR 9     /** Zero division error. */
#define ERRCODE_INTERNAL_ERROR 99         /** Internal error of compiler. */

#define error(msg) fprintf(stderr, "Error at %s:%d: %s\n", __FILE__, __LINE__, msg); exit(ERRCODE_INTERNAL_ERROR)
#define guard(cond) if (!(cond)) { error("Guard failed."); }
