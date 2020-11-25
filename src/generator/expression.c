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

bool is_bool_operator(tokenid_e id) {
   return id == TOKENID_OPERATOR_NOT || id == TOKENID_OPERATOR_NOT_EQUAL || id == TOKENID_OPERATOR_EQUALS
      || id == TOKENID_OPERATOR_LESS || id == TOKENID_OPERATOR_LESS_OR_EQUAL || id == TOKENID_OPERATOR_GREATER
      || id == TOKENID_OPERATOR_GREATER_OR_EQUAL;
}

vartype_e determine_token_type(token_t* token, bintreestack_t* varstack) {
   switch (token->id) {
      case TOKENID_IDENTIFIER:
         return bintreestack_find(varstack, token->value.string_value, NULL)->value.var->type;
      case TOKENID_STRING_LITERAL:
         return VT_STRING;
      case TOKENID_BOOL_LITERAL:
         return VT_BOOL;
      case TOKENID_NUM_DECIMAL:
         return VT_FLOAT;
      case TOKENID_NUM:
         return VT_INT;
      default:
         return VT_UNDEFINED;
   }
}

vartype_e determine_expression_type(astnode_exp_t* exp, bintreestack_t* varstack) {
   token_t* last = exp->tokens[exp->tokens_count - 1];
   token_t* first = exp->tokens[0];
   varstack = varstack;
   if (exp->tokens_count == 1) {
      return determine_token_type(first, varstack);
   } 
   if (is_bool_operator(last->id)) {
      return VT_BOOL;
   }
   for (int i = 0; i < exp->tokens_count; i++) {
      vartype_e type = determine_token_type(exp->tokens[i], varstack);
      if (type != VT_UNDEFINED) {
         return type;
      }
   }
   error("Given expression cannot be determined.");
}

void print_pushs_identifier(char* identifier, bintreestack_t* varstack) {
   int depth = get_var_depth(identifier, varstack);
   char* var = generate_var_str(identifier, FT_TF, depth);
   printcm("PUSHS %s", var);
   free(var);
}

void generate_stack_expression(astnode_exp_t* exp, bintreestack_t* varstack) {
   for (int i = 0; i < exp->tokens_count; i++) {
      switch (exp->tokens[i]->id) {
         case TOKENID_IDENTIFIER:
            print_pushs_identifier(exp->tokens[i]->value.string_value, varstack);
            break;
         case TOKENID_BOOL_LITERAL:
            printcm("PUSHS bool@%s", exp->tokens[i]->value.bool_value ? "true" : "false");
            break;
         case TOKENID_NUM_DECIMAL:
            printcm("PUSHS float@%a", exp->tokens[i]->value.decimal_value);
            break;
         case TOKENID_NUM:
            printcm("PUSHS int@%d", exp->tokens[i]->value.int_value);
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
            printcm(determine_expression_type(exp, varstack) == VT_FLOAT ? "IDIVS" : "DIVS");
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

void generate_local_expression(char* var, astnode_exp_t* exp, bintreestack_t* varstack) {
   exp = exp;
   varstack = varstack;
   var = var;
   /*
   if (exp->tokens_count == 2) {
      guard(exp->tokens[0] == TOKENID_IDENTIFIER && exp->tokens[1] == TOKENID_OPERATOR_NOT);
      int depth = get_var_depth(exp->tokens[0]->value.string_value, varstack);
      char* var = generate_var_str(exp->tokens[0]->value.string_value, FT_TF, depth);
      printcm("NOT %s %s", var, var);
   } else {
      int depth = get_var_depth(exp->tokens[0]->value.string_value, varstack);
      char* var = generate_var_str(exp->tokens[0]->value.string_value, FT_TF, depth);
   }
   */
}

void generate_const_expression(char* varstr, astnode_exp_t* exp) {
   char* val = generate_const_str(exp->tokens[0]);
   printcm("MOVE %s %s", varstr, val);
   free(val);
}

vartype_e generate_assign_expression(char* varstr, astnode_exp_t* exp, bintreestack_t* varstack) {
   infix_to_postfix(exp);
   printcm("DEFVAR %s", varstr);
   if (exp->tokens_count == 1) {
      generate_const_expression(varstr, exp);
   } else if (exp->tokens_count == 2 || exp->tokens_count == 3) {
      generate_local_expression(varstr, exp, varstack);
   } else {
      generate_stack_expression(exp, varstack);
      printcm("POPS %s", varstr);
   }
   return determine_expression_type(exp, varstack);
}
