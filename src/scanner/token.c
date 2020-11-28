/**
 * IFJ20 Compiler
 * @file token.c
 * @authors Martin Bednář (xbedna77)
 */

#include "token.h"
#include "../error.h"
#include "../memory.h"
#include <string.h>
#include <inttypes.h>

bool is_memory_allocated(tokenid_e id) {
   const tokenid_e ids[] = {
      TOKENID_IDENTIFIER,
      TOKENID_STRING_LITERAL
   }; // Array of all ids that use memory
   int size = sizeof(ids) / sizeof(ids[0]);
   for (int i = 0; i < size; i++) {
      if (ids[i] == id) {
         return true;
      }
   }
   return false;
}

token_t* token_ctor(tokenid_e id, token_value_u value) {
   token_t* token = safe_alloc(sizeof(token_t));
   token->id = id;
   token->value = value;
   return token;
}

void token_dtor(token_t* token) {
   guard(token != NULL);
   if (is_memory_allocated(token->id)) {
      free(token->value.string_value);
   }
   free(token);
}

token_t* token_copy(token_t* token) {
   token_value_u value;
   if (token->id == TOKENID_IDENTIFIER || token->id == TOKENID_STRING_LITERAL) {
      value.string_value = safe_alloc((strlen(token->value.string_value)+1)*sizeof(char));
      strcpy(value.string_value, token->value.string_value);
   } else if (token->id == TOKENID_NUM) {
      value.int_value = token->value.int_value;
   } else if (token->id == TOKENID_NUM_DECIMAL) {
      value.decimal_value = token->value.decimal_value;
   } else if (token->id == TOKENID_BOOL_LITERAL) {
      value.bool_value = token->value.bool_value;
   }
   token_t* copy = token_ctor(token->id, value);
   return copy;
}

void token_print(token_t* token) {
   if (token == NULL) {
      fprintf(stderr, "NULL\n");
      return;
   }
   switch (token->id) {
      case TOKENID_IDENTIFIER:
         fprintf(stderr,"TOKEN ID: TOKENID_IDENTIFIER\n");
         fprintf(stderr,"STRING VALUE: %s\n", token->value.string_value);
         break;
      case TOKENID_KEYWORD_IF:
         fprintf(stderr,"TOKEN ID: TOKENID_KEYWORD_IF\n");
         break;
      case TOKENID_KEYWORD_FOR:
         fprintf(stderr,"TOKEN ID: TOKENID_KEYWORD_FOR\n");
         break;
      case TOKENID_KEYWORD_ELSE:
         fprintf(stderr,"TOKEN ID: TOKENID_KEYWORD_ELSE\n");
         break;
      case TOKENID_KEYWORD_RETURN:
         fprintf(stderr,"TOKEN ID: TOKENID_KEYWORD_RETURN\n");
         break;
      case TOKENID_KEYWORD_PACKAGE:
         fprintf(stderr,"TOKEN ID: TOKENID_KEYWORD_PACKAGE\n");
         break;
      case TOKENID_KEYWORD_FUNC:
         fprintf(stderr,"TOKEN ID: TOKENID_KEYWORD_FUNC\n");
         break;
      case TOKENID_KEYWORD_FLOAT64:
         fprintf(stderr,"TOKEN ID: TOKENID_KEYWORD_FLOAT64\n");
         break;
      case TOKENID_KEYWORD_INT:
         fprintf(stderr,"TOKEN ID: TOKENID_KEYWORD_INT\n");
         break;
      case TOKENID_KEYWORD_STRING:
         fprintf(stderr,"TOKEN ID: TOKENID_KEYWORD_STRING\n");
         break;
      case TOKENID_KEYWORD_BOOL:
         fprintf(stderr,"TOKEN ID: TOKENID_KEYWORD_BOOL\n");
         break;
      case TOKENID_STRING_LITERAL:
         fprintf(stderr,"TOKEN ID: TOKENID_STRING_LITERAL\n");
         fprintf(stderr,"STRING VALUE: %s\n", token->value.string_value);
         break;
      case TOKENID_BOOL_LITERAL:
         fprintf(stderr,"TOKEN ID: TOKENID_BOOL_LITERAL\n");
         fprintf(stderr,"%s", token->value.bool_value ? "true" : "false");
         break;
      case TOKENID_NUM:
         fprintf(stderr,"TOKEN ID: TOKENID_NUM\n");
         fprintf(stderr,"INT_VALUE: %lld\n", token->value.int_value);
         break;
      case TOKENID_NUM_DECIMAL:
         fprintf(stderr,"TOKEN ID: TOKENID_NUM_DECIMAL\n");
         fprintf(stderr,"DOUBLE VALUE: %lf\n", token->value.decimal_value);
         break;
      case TOKENID_OPERATOR_ADD:
         fprintf(stderr,"TOKEN ID: TOKENID_OPERATOR_ADD\n");
         break;
      case TOKENID_OPERATOR_SUB:
         fprintf(stderr,"TOKEN ID: TOKENID_OPERATOR_SUB\n");
         break;
      case TOKENID_OPERATOR_MUL:
         fprintf(stderr,"TOKEN ID: TOKENID_OPERATOR_MUL\n");
         break;
      case TOKENID_OPERATOR_DIV:
         fprintf(stderr,"TOKEN ID: TOKENID_OPERATOR_DIV\n");
         break;
      case TOKENID_OPERATOR_LESS:
         fprintf(stderr,"TOKEN ID: TOKENID_OPERATOR_LESS\n");
         break;
      case TOKENID_OPERATOR_LESS_OR_EQUAL:
         fprintf(stderr,"TOKEN ID: TOKENID_OPERATOR_LESS_OR_EQUAL\n");
         break;
      case TOKENID_OPERATOR_GREATER:
         fprintf(stderr,"TOKEN ID: TOKENID_OPERATOR_GREATER\n");
         break;
      case TOKENID_OPERATOR_GREATER_OR_EQUAL:
         fprintf(stderr,"TOKEN ID: TOKENID_OPERATOR_GREATER_OR_EQUAL\n");
         break;
      case TOKENID_OPERATOR_DECLARE:
         fprintf(stderr,"TOKEN ID: TOKENID_OPERATOR_DECLARE\n");
         break;
      case TOKENID_OPERATOR_ASSIGN:
         fprintf(stderr,"TOKEN ID: TOKENID_OPERATOR_ASSIGN\n");
         break;
      case TOKENID_OPERATOR_EQUALS:
         fprintf(stderr,"TOKEN ID: TOKENID_OPERATOR_EQUALS\n");
         break;
      case TOKENID_OPERATOR_NOT:
         fprintf(stderr,"TOKEN ID: TOKENID_OPERATOR_NOT\n");
         break;
      case TOKENID_OPERATOR_NOT_EQUAL:
         fprintf(stderr,"TOKEN ID: TOKENID_OPERATOR_NOT_EQUAL\n");
         break;
      case TOKENID_OPERATOR_AND:
         fprintf(stderr,"TOKEN ID: TOKENID_OPERATOR_AND\n");
         break;
      case TOKENID_OPERATOR_OR:
         fprintf(stderr,"TOKEN ID: TOKENID_OPERATOR_OR\n");
         break;
      case TOKENID_LEFT_PARENTHESES:
         fprintf(stderr,"TOKEN ID: TOKENID_LEFT_PARENTHESES\n");
         break;
      case TOKENID_RIGHT_PARENTHESES:
         fprintf(stderr,"TOKEN ID: TOKENID_RIGHT_PARENTHESES\n");
         break;
      case TOKENID_LEFT_BRACKET:
         fprintf(stderr,"TOKEN ID: TOKENID_LEFT_BRACKET\n");
         break;
      case TOKENID_RIGHT_BRACKET:
         fprintf(stderr,"TOKEN ID: TOKENID_RIGHT_BRACKET\n");
         break;
      case TOKENID_SEMICOLON:
         fprintf(stderr,"TOKEN ID: TOKENID_SEMICOLON\n");
         break;
      case TOKENID_COMMA:
         fprintf(stderr,"TOKEN ID: TOKENID_COMMA\n");
         break;
      case TOKENID_NEWLINE:
         fprintf(stderr,"TOKEN ID: TOKENID_NEWLINE\n");
         break;
      case TOKENID_END_OF_FILE:
         fprintf(stderr,"TOKEN ID: TOKENID_END_OF_FILE\n");
         break;
      default:
         error("Invalid token ID");
         break;
   }
}
