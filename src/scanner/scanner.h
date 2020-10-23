/**
 * IFJ20 Compiler
 * @file scanner.h
 * @authors František Lukeš (xlukes07)
 */

#pragma once

typedef enum tokenid {
   TOKENID_SPACE,
   TOKENID_NEWLINE,
   TOKENID_INDENT,
   TOKENID_IDENTIFIER,
   TOKENID_CONTROL_KEYWORD,
   TOKENID_DECLARATION_KEYWORD,
   TOKENID_DATATYPE,
   TOKENID_NUM,
   TOKENID_OPERATOR,
   TOKENID_DECLARATION_OPERATOR,
   TOKENID_ASSIGN_OPERATOR,
   TOKENID_LEFT_PARENTHESES,
   TOKENID_RIGHT_PARENTHESES,
   TOKENID_LEFT_BRACKET,
   ToKENID_RIGHT_BRACKET,
   TOKENID_END_OF_FILE
} tokenid;

typedef union value_t {
   char string_value[256];
   int int_value;
   float float_value;
} value_t;


typedef struct token_t {
   tokenid id;
   value_t value;
} token_t;

/**
 * Deallocates memory used by token.
 */
void token_dtor(token_t* token);


/**
 * Returns next token from stdin.
 */
token_t* get_next_token();
