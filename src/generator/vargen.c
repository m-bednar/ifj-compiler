/**
 * IFJ20 Compiler
 * @file vargen.c
 * @authors Martin Bednář (xbedna77)
 */

#include "vargen.h"
#include "utils.h"
#include "../error.h"
#include "../memory.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>

static const int BOOL_CONST_PREFIX = 6;
static const int STR_CONST_PREFIX = 8;
static const int INT_CONST_PREFIX = 5;
static const int FLOAT_CONST_MAX_SIZE = 35;

/**
 * Returns count of digits of given int.
 */
int digits_count(int64_t n) {
   int count = 0;
   do {
      count++;
      n /= 10;
   } while(n != 0);
   return count; 
}

bool is_const_tokenid(tokenid_e id) {
   return id == TOKENID_NUM_DECIMAL || id == TOKENID_NUM || id == TOKENID_STRING_LITERAL || id == TOKENID_BOOL_LITERAL;
}

char* labelgen_new() {
   static int lastid = 0;
   char* label = safe_alloc(2 + digits_count(lastid));
   sprintf(label, "$L%d", lastid);
   lastid++;
   return label;
}

char* convert_string(char* str) {
   const int csize = 3; // Size of escaped ascii code minus one char
   char* out;
   int len = strlen(str);
   for (int i = 0; i < (int)strlen(str); i++) {
      if ((str[i] >= 0 && str[i] <= 32) || str[i] == 35 || str[i] == 92) {
         len += csize;
      }
   }
   out = safe_alloc(sizeof(char) * (len + 1));
   out[0] = '\0';
   for (int i = 0; i < (int)strlen(str); i++) {
      if ((str[i] >= 0 && str[i] <= 32) || str[i] == 35 || str[i] == 92) {
         char buffer[3];
         sprintf(buffer, "%d", str[i]);
         if (digits_count((int)str[i]) == 2) {
            strcat(out, "\\0");
         } else { 
            strcat(out, "\\00");
         }
         strcat(out, buffer);
      } else {
         strncat(out, &str[i], 1);
      }
   }
   return out;
}

char* generate_const_str(token_t* token) {
   char* var;
   char* str;
   switch (token->id) {
      case TOKENID_BOOL_LITERAL:
         var = safe_alloc(sizeof(char) * (BOOL_CONST_PREFIX + token->value.bool_value ? 4 : 5));
         sprintf(var, "bool@%s", token->value.bool_value ? "true" : "false");
         break;
      case TOKENID_STRING_LITERAL:
         str = convert_string(token->value.string_value);
         var = safe_alloc(sizeof(char) * (STR_CONST_PREFIX + strlen(str)));
         sprintf(var, "string@%s", str);
         free(str);
         break;
      case TOKENID_NUM:
         var = safe_alloc(sizeof(char) * (INT_CONST_PREFIX + digits_count(token->value.int_value)));
         sprintf(var, "int@%"PRId64, token->value.int_value);
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
