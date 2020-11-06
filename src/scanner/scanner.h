/**
 * IFJ20 Compiler
 * @file scanner.h
 * @authors František Lukeš (xlukes07)
 */

#include <stdint.h>
#pragma once

typedef enum tokenid_e {
   //TOKENID_SPACE,
   //TOKENID_INDENT,               // \t
   TOKENID_IDENTIFIER,
   TOKENID_KEYWORD_IF,              // if
   TOKENID_KEYWORD_FOR,             // for
   TOKENID_KEYWORD_ELSE,            //else
   TOKENID_KEYWORD_RETURN,          //return
   TOKENID_KEYWORD_PACKAGE,         //package
   TOKENID_KEYWORD_FUNC,            // func
   TOKENID_KEYWORD_FLOAT64,         // flaot64
   TOKENID_KEYWORD_INT,             // int
   TOKENID_KEYWORD_STRING,          //string
   TOKENID_KEYWORD_BOOL,            //bool
   TOKENID_STRING_LITERAL,          // "string literal"
   TOKENID_NUM,
   TOKENID_NUM_DECIMAL,
   TOKENID_OPERATOR_PLUS,           // +
   TOKENID_OPERATOR_MINUS,          // -
   TOKENID_OPERATOR_MUL,           // *
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
} tokenid_e;

typedef union token_value_u {
   char* string_value;
   int64_t int_value;
   double decimal_value;
} token_value_u;


typedef struct token_t {
   tokenid_e id;
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
