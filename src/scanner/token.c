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
   switch (token->id)
   {
      case TOKENID_IDENTIFIER:
         printf("TOKEN ID: TOKENID_IDENTIFIER\n");
         printf("STRING VALUE: %s\n", token->value.string_value);
         break;
      case TOKENID_KEYWORD_IF:
         printf("TOKEN ID: TOKENID_KEYWORD_IF\n");
         break;
      case TOKENID_KEYWORD_FOR:
         printf("TOKEN ID: TOKENID_KEYWORD_FOR\n");
         break;
      case TOKENID_KEYWORD_ELSE:
         printf("TOKEN ID: TOKENID_KEYWORD_ELSE\n");
         break;
      case TOKENID_KEYWORD_RETURN:
         printf("TOKEN ID: TOKENID_KEYWORD_RETURN\n");
         break;
      case TOKENID_KEYWORD_PACKAGE:
         printf("TOKEN ID: TOKENID_KEYWORD_PACKAGE\n");
         break;
      case TOKENID_KEYWORD_FUNC:
         printf("TOKEN ID: TOKENID_KEYWORD_FUNC\n");
         break;
      case TOKENID_KEYWORD_FLOAT64:
         printf("TOKEN ID: TOKENID_KEYWORD_FLOAT64\n");
         break;
      case TOKENID_KEYWORD_INT:
         printf("TOKEN ID: TOKENID_KEYWORD_INT\n");
         break;
      case TOKENID_KEYWORD_STRING:
         printf("TOKEN ID: TOKENID_KEYWORD_STRING\n");
         break;
      case TOKENID_KEYWORD_BOOL:
         printf("TOKEN ID: TOKENID_KEYWORD_BOOL\n");
         break;
      case TOKENID_STRING_LITERAL:
         printf("TOKEN ID: TOKENID_STRING_LITERAL\n");
         printf("STRING VALUE: %s\n", token->value.string_value);
         break;
      case TOKENID_BOOL_LITERAL:
         printf("TOKEN ID: TOKENID_BOOL_LITERAL\n");
         printf("BOOL VALUE: %d\n", token->value.bool_value);
         break;
      case TOKENID_NUM:
         printf("TOKEN ID: TOKENID_NUM\n");
         printf("INT_VALUE: %" PRId64 "\n", token->value.int_value);
         break;
      case TOKENID_NUM_DECIMAL:
         printf("TOKEN ID: TOKENID_NUM_DECIMAL\n");
         printf("DOUBLE VALUE: %lf\n", token->value.decimal_value);
         break;
      case TOKENID_OPERATOR_ADD:
         printf("TOKEN ID: TOKENID_OPERATOR_ADD\n");
         break;
      case TOKENID_OPERATOR_SUB:
         printf("TOKEN ID: TOKENID_OPERATOR_SUB\n");
         break;
      case TOKENID_OPERATOR_MUL:
         printf("TOKEN ID: TOKENID_OPERATOR_MUL\n");
         break;
      case TOKENID_OPERATOR_DIV:
         printf("TOKEN ID: TOKENID_OPERATOR_DIV\n");
         break;
      case TOKENID_OPERATOR_LESS:
         printf("TOKEN ID: TOKENID_OPERATOR_LESS\n");
         break;
      case TOKENID_OPERATOR_LESS_OR_EQUAL:
         printf("TOKEN ID: TOKENID_OPERATOR_LESS_OR_EQUAL\n");
         break;
      case TOKENID_OPERATOR_GREATER:
         printf("TOKEN ID: TOKENID_OPERATOR_GREATER\n");
         break;
      case TOKENID_OPERATOR_GREATER_OR_EQUAL:
         printf("TOKEN ID: TOKENID_OPERATOR_GREATER_OR_EQUAL\n");
         break;
      case TOKENID_OPERATOR_DECLARE:
         printf("TOKEN ID: TOKENID_OPERATOR_DECLARE\n");
         break;
      case TOKENID_OPERATOR_ASSIGN:
         printf("TOKEN ID: TOKENID_OPERATOR_ASSIGN\n");
         break;
      case TOKENID_OPERATOR_EQUALS:
         printf("TOKEN ID: TOKENID_OPERATOR_EQUALS\n");
         break;
      case TOKENID_OPERATOR_NOT:
         printf("TOKEN ID: TOKENID_OPERATOR_NOT\n");
         break;
      case TOKENID_OPERATOR_NOT_EQUAL:
         printf("TOKEN ID: TOKENID_OPERATOR_NOT_EQUAL\n");
         break;
      case TOKENID_OPERATOR_AND:
         printf("TOKEN ID: TOKENID_OPERATOR_AND\n");
         break;
      case TOKENID_OPERATOR_OR:
         printf("TOKEN ID: TOKENID_OPERATOR_OR\n");
         break;
      case TOKENID_LEFT_PARENTHESES:
         printf("TOKEN ID: TOKENID_LEFT_PARENTHESES\n");
         break;
      case TOKENID_RIGHT_PARENTHESES:
         printf("TOKEN ID: TOKENID_RIGHT_PARENTHESES\n");
         break;
      case TOKENID_LEFT_BRACKET:
         printf("TOKEN ID: TOKENID_LEFT_BRACKET\n");
         break;
      case TOKENID_RIGHT_BRACKET:
         printf("TOKEN ID: TOKENID_RIGHT_BRACKET\n");
         break;
      case TOKENID_SEMICOLON:
         printf("TOKEN ID: TOKENID_SEMICOLON\n");
         break;
      case TOKENID_COMMA:
         printf("TOKEN ID: TOKENID_COMMA\n");
         break;
      case TOKENID_NEWLINE:
         printf("TOKEN ID: TOKENID_NEWLINE\n");
         break;
      case TOKENID_END_OF_FILE:
         printf("TOKEN ID: TOKENID_END_OF_FILE\n");
         break;
   default:
      printf("NULL\n");
      break;
   }
}
