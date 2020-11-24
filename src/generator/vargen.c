/**
 * IFJ20 Compiler
 * @file vargen.c
 * @authors Martin Bednář (xbedna77)
 */

#include "vargen.h"

static const int BOOL_CONST_PREFIX = 6;
static const int STR_CONST_PREFIX = 8;
static const int INT_CONST_PREFIX = 5;
static const int FLOAT_CONST_MAX_SIZE = 35;

int digits_count(int n) {
   int count = 0;
   do {
      count++;
      n /= 10;
   } while(n != 0);
   return count; 
}

int new_var_depth(bintreestack_t* varstack) {
   return bintreestack_get_length(varstack) - 1;
}

int get_var_depth(char* identifier, bintreestack_t* varstack) {
   int depth;
   bintreestack_find(varstack, identifier, &depth);
   guard(depth != -1);
   return depth;
}

char* generate_const_str(token_t* token) {
   char* var;
   switch (token->id) {
      case TOKENID_BOOL_LITERAL:
         var = safe_alloc(sizeof(char) * (BOOL_CONST_PREFIX + token->value.bool_value ? 4 : 5));
         sprintf(var, "bool@%s", token->value.bool_value ? "true" : "false");
         break;
      case TOKENID_STRING_LITERAL:
         var = safe_alloc(sizeof(char) * (STR_CONST_PREFIX + strlen(token->value.string_value)));
         sprintf(var, "string@%s", token->value.bool_value ? "true" : "false");
         break;
      case TOKENID_NUM:
         var = safe_alloc(sizeof(char) * (INT_CONST_PREFIX + digits_count(token->value.int_value)));
         sprintf(var, "int@%d", token->value.int_value);
         break;
      case TOKENID_NUM_DECIMAL:
         var = safe_alloc(sizeof(char) * FLOAT_CONST_MAX_SIZE); // used space cannot be easily computed
         sprintf(var, "float@%a", token->value.decimal_value);
         break;
      default:
         error("Token cannot be converted to constant.");
   }
   return var;
}

char* generate_var_str(char* identifier, frametype_e frame, int depth) {
   const int suflen = 3; // length of frame suffix
   char* var = safe_alloc(sizeof(char) * (digits_count(depth) + strlen(identifier) + suflen + 2));
   switch (frame) {
      case FT_GF:
         sprintf(var, "GF@%s&%d", identifier, depth);
         break;
      case FT_LF:
         sprintf(var, "LF@%s&%d", identifier, depth);
         break;
      case FT_TF:
         sprintf(var, "TF@%s&%d", identifier, depth);
         break;
   }
   return var;
}