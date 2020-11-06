/**
 * IFJ20 Compiler
 * @file parser.h
 * @authors Daniel Havranek (xhavra18)
 */

#pragma once

#include "ntsymbol.h"
#include "ntsymstack.h"

/**
 * Parses tokens recieved from scanner.
 * Returns AST.
 */
void parse();