/**
 * IFJ20 Compiler
 * @file expression.c
 * @authors Martin Bednář (xbedna77)
 */

#include "expression.h"
#include "postfix.h"
#include "vargen.h"
#include "utils.h"

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

void print_pushs_identifier(char* identifier, bintreestack_t* varstack) {
   int depth = get_var_depth(identifier, varstack);
   char* var = generate_var_str(identifier, FT_TF, depth);
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
   printcm("PUSHS float@%a", value);
}

void generate_stack_expression(astnode_exp_t* exp, bintreestack_t* varstack) {
   for (int i = 0; i < exp->tokens_count; i++) {
      switch (exp->tokens[i]->id) {
         case TOKENID_IDENTIFIER:
            print_pushs_identifier(exp->tokens[i]->value.string_value, varstack);
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
         case TOKENID_OPERATOR_ADD:
            printcm("ADDS");
            break;
         case TOKENID_OPERATOR_SUB:
            printcm("SUBS");
            break;
         case TOKENID_OPERATOR_MUL:
            printcm("MULS");
            break;
         case TOKENID_OPERATOR_DIV:
            printcm("DIV");   // FIXME: IDIV version required
            break;
         case TOKENID_OPERATOR_AND:
            printcm("ANDS");
            break;
         case TOKENID_OPERATOR_OR:
            printcm("ORS");
            break;
         case TOKENID_OPERATOR_NOT:
            printcm("NOTS");
            break;
         case TOKENID_OPERATOR_EQUALS:
            printcm("EQS");
            break;
         case TOKENID_OPERATOR_NOT_EQUAL:
            printcm("EQS");
            printcm("NOTS");
            break;
         case TOKENID_OPERATOR_LESS:
            printcm("LTS");
            break;
         case TOKENID_OPERATOR_GREATER:
            printcm("GTS");
            break;
         case TOKENID_OPERATOR_LESS_OR_EQUAL:
            printcm("GTS");
            printcm("NOTS");
            break;
         case TOKENID_OPERATOR_GREATER_OR_EQUAL:
            printcm("LTS");
            printcm("NOTS");
            break;
         default:
            exit(ERRCODE_INTERNAL_ERROR);
      }
   }
}

void generate_local_expression(astnode_exp_t* exp, bintreestack_t* varstack) {
   exp = exp;
   varstack = varstack;
}

void generate_const_expression(char* varstr, astnode_exp_t* exp) {
   char* val = generate_const_str(exp->tokens[0]);
   printcm("MOVE %s %s", varstr, val);
   free(val);
}

void generate_assign_expression(char* identifier, char* varstr, astnode_exp_t* exp, bintreestack_t* varstack) {
   // TODO: check for string concat
   identifier = identifier;
   if (exp->tokens_count == 1) {
      generate_const_expression(varstr, exp);
   } else if (exp->tokens_count <= 3) {
      generate_local_expression(exp, varstack);
   } else {
      infix_to_postfix(exp);
      // optimize postfix
      if (exp->tokens_count == 1) {
         generate_const_expression(varstr, exp);
      } else {
         generate_stack_expression(exp, varstack);
      }
   }
}
