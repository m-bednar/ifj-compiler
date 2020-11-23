/**
 * IFJ20 Compiler
 * @file semantic.h
 * @authors Michal Trlica (xtrlic02)
 */

#pragma once

#include "../scanner/scanner.h"
#include "ast.h"

/*
 * returns ERROR code or -1 if no error occured 
 */
int semantic(token_t* token);
