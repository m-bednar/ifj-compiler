/**
 * IFJ20 Compiler
 * @file expression.c
 * @authors Martin Bednář (xbedna77)
 */

#include "expression.h"
#include "postfix.h"
#include "utils.h"

int digits(int n) {
   int count = 0;
   do {
      count++;
      n /= 10;
   } while(n != 0);
   return count; 
} 

char* str_var(char* identifier, frametype_e frame, bintreestack_t* varstack) {
   const int suflen = 3; // length of frame suffix
   int depth;
   bintreestack_find(varstack, identifier, &depth);
   depth = depth == -1 ? bintreestack_get_length(varstack) - 1 : depth;
   char* var = safe_alloc(sizeof(char) * (digits(depth) + strlen(identifier) + suflen + 2));
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

char* convert_string(char* str) {
   const int csize = 4; // Size of special ascii code
   char* out;
   int len = strlen(str);
   
   for (int i = 0; i < (int)strlen(str); i++) {
      if ((str[i] >= 0 && str[i] <= 32) || str[i] == 35 || str[i] == 92) {
         len += csize;
      }
   }
   
   out = safe_alloc(sizeof(char) * len + 1);
   len = 0;

   for (int i = 0; i < (int)strlen(str); i++) {
      if ((str[i] >= 0 && str[i] <= 32) || str[i] == 35 || str[i] == 92) {
         sprintf(out + len * sizeof(char), "\\%03d", (int)str[i]);
         len += csize;
      } else {
         out[len] = str[i];
         len++;
      }
   }

   return out;
}

void print_pushs_identifier(char* identifier, frametype_e frame, bintreestack_t* varstack) {
   char* var = str_var(identifier, frame, varstack);
   printcm("PUSHS %s", var);
   free(var);
}

void print_pushs_string(char* value) {
   char* str = convert_string(value);
   printcm("PUSHS string@%s", str);
   free(str);
}

void print_pushs_bool(bool value) {
   printcm("PUSHS bool@%s", value ? "true" : "false");
}

void print_pushs_int(int value) {
   printcm("PUSHS int@%d", value);
}

void print_pushs_float(double value) {
   printcm("PUSHS float@%f", value); // FIXME: f should be a, but it is not working
}

void evaluate_stack_expression(astnode_exp_t* exp, bintreestack_t* varstack) {
   for (int i = 0; i < exp->tokens_count; i++) {
      switch (exp->tokens[i]->id) {
         case TOKENID_IDENTIFIER:
            print_pushs_identifier(exp->tokens[i]->value.string_value, FT_TF, varstack);
            break;
         case TOKENID_STRING_LITERAL:
            print_pushs_string(exp->tokens[i]->value.string_value);
            break;
         case TOKENID_BOOL_LITERAL:
            print_pushs_bool(exp->tokens[i]->value.bool_value);
            break;
         case TOKENID_NUM_DECIMAL:
            print_pushs_float(exp->tokens[i]->value.decimal_value);
            break;
         case TOKENID_NUM:
            print_pushs_int(exp->tokens[i]->value.int_value);
            break;
         default:
            exit(ERRCODE_INTERNAL_ERROR);
      }
      if (exp->tokens[i]->id == TOKENID_IDENTIFIER) {
         char* var = str_var(exp->tokens[i]->value.string_value, FT_TF, varstack);
         printcm("PUSHS %s", var);
         free(var);
      }
   }
}

void evaluate_local_expression(astnode_exp_t* exp, bintreestack_t* varstack) {
   exp = exp;
   varstack = varstack;
}

exptype_e evaluate_expression(astnode_exp_t* exp, bintreestack_t* varstack) {
   if (exp->tokens_count == 3) {
      evaluate_local_expression(exp, varstack);
      return ET_LOCAL;
   } else {
      infix_to_postfix(exp);
      evaluate_stack_expression(exp, varstack);
      return ET_STACK;
   }
}
