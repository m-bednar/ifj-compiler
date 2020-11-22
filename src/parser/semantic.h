/**
 * IFJ20 Compiler
 * @file semantic.h
 * @authors Michal Trlica (xtrlic02)
 */

#pragma once

#include "../scanner/scanner.h"

typedef enum semantic_error_e {
   SERROR_DECLARE_3,
   SERROR_UNKNOWN_TYPE_4,
   SERROR_ASSIGN_TYPE_5,
   SERROR_FUNC_CALL_6,
   SERROR_OTHER_7,
   SERROR_DEVIDE_BY_ZERO_9
}semantic_error_e;

semantic_error_e semantic(token_t* token);
