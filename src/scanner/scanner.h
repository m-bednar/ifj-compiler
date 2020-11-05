/**
 * IFJ20 Compiler
 * @file scanner.h
 * @authors František Lukeš (xlukes07)
 */

#include <stdint.h>
#pragma once

typedef enum tokenid {
   //TOKENID_SPACE,
   //TOKENID_INDENT,               // \t
   TOKENID_IDENTIFIER,
   TOKENID_CONTROL_KEYWORD,         // if, for, else, return
   TOKENID_DECLARATION_KEYWORD,     // package, func
   TOKENID_DATATYPE,                // float64, int, string, bool
   TOKENID_NUM,
   TOKENID_NUM_DECIMAL,
   TOKENID_OPERATOR_PLUS,           // +
   TOKENID_OPERATOR_MINUS,          // -
   TOKENID_OPERATOR_MULT,           // *
   TOKENID_OPERATOR_DIV,            // /
   TOKENID_OPERATOR_LESS,           // <
   TOKENID_OPERATOR_LESS_OR_EQUAL,  // <=
   TOKENID_OPERATOR_MORE,           // >
   TOKENID_OPERATOR_MORE_OR_EQUAL,  // >=
   TOKENID_DECLARATION_OPERATOR,    // :=
   TOKENID_ASSIGN_OPERATOR,         // =
   TOKENID_LEFT_PARENTHESES,        // (
   TOKENID_RIGHT_PARENTHESES,       // )
   TOKENID_LEFT_BRACKET,            // {
   TOKENID_RIGHT_BRACKET,           // }
   TOKENID_SEMICOLON,               // ;
   TOKENID_COMMA,                   // ,
   TOKENID_NEWLINE,                 // \n
   TOKENID_END_OF_FILE              // EOF
} tokenid;

typedef union token_value_u {
   char* string_value;
   int64_t int_value;
   double decimal_value;
} token_value_u;


typedef struct token_t {
   tokenid id;
   token_value_u value;
} token_t;

/**
 * Deallocates memory used by token.
 */
void token_dtor(token_t* token);


/**
 * Returns next token from stdin.
 */
token_t* get_next_token();
