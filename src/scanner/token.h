/**
 * IFJ20 Compiler
 * @file token.h
 * @authors Martin Bednář (xbedna77), František Lukeš (xlukes07)
 */

#pragma once

#include <stdint.h>
#include <stdbool.h>

typedef enum tokenid_e {
   TOKENID_IDENTIFIER,
   TOKENID_KEYWORD_IF,                 // if
   TOKENID_KEYWORD_FOR,                // for
   TOKENID_KEYWORD_ELSE,               // else
   TOKENID_KEYWORD_RETURN,             // return
   TOKENID_KEYWORD_PACKAGE,            // package
   TOKENID_KEYWORD_FUNC,               // func
   TOKENID_KEYWORD_FLOAT64,            // flaot64
   TOKENID_KEYWORD_INT,                // int
   TOKENID_KEYWORD_STRING,             // string
   TOKENID_KEYWORD_BOOL,               // bool
   TOKENID_STRING_LITERAL,             // "string literal"
   TOKENID_BOOL_LITERAL,               // true / false
   TOKENID_NUM,                        // integer number
   TOKENID_NUM_DECIMAL,                // decimal number
   TOKENID_OPERATOR_ADD,               // +
   TOKENID_OPERATOR_SUB,               // -
   TOKENID_OPERATOR_MUL,               // *
   TOKENID_OPERATOR_DIV,               // /
   TOKENID_OPERATOR_LESS,              // <
   TOKENID_OPERATOR_LESS_OR_EQUAL,     // <=
   TOKENID_OPERATOR_GREATER,           // >
   TOKENID_OPERATOR_GREATER_OR_EQUAL,  // >=
   TOKENID_OPERATOR_DECLARE,           // :=
   TOKENID_OPERATOR_ASSIGN,            // =
   TOKENID_OPERATOR_EQUALS,            // ==
   TOKENID_OPERATOR_NOT,               // !
   TOKENID_OPERATOR_NOT_EQUAL,         // !=
   TOKENID_OPERATOR_AND,               // &&
   TOKENID_OPERATOR_OR,                // ||
   TOKENID_LEFT_PARENTHESES,           // (
   TOKENID_RIGHT_PARENTHESES,          // )
   TOKENID_LEFT_BRACKET,               // {
   TOKENID_RIGHT_BRACKET,              // }
   TOKENID_SEMICOLON,                  // ;
   TOKENID_COMMA,                      // ,
   TOKENID_NEWLINE,                    // \n
   TOKENID_END_OF_FILE                 // EOF
} tokenid_e;

typedef union token_value_u {
   char* string_value;
   int64_t int_value;
   double decimal_value;
   bool bool_value;
} token_value_u;


typedef struct token_t {
   tokenid_e id;
   token_value_u value;
} token_t;

/**
 * Allocates and creates new token.
 */
token_t* token_ctor(tokenid_e id, token_value_u value);

/**
 * Deallocates memory used by token.
 */
void token_dtor(token_t* token);
