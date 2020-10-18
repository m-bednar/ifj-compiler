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
   TOKENID_KEYWORD
} tokenid;

typedef struct token_t {
   tokenid id;
} token_t;

/**
 * Deallocates memory used by token.
 */
void token_dtor(token_t* token);


/**
 * Returns next token from stdin.
 */
token_t* get_next_token();
