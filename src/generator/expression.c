/**
 * IFJ20 Compiler
 * @file expression.c
 * @authors Martin Bednář (xbedna77)
 */

#include "expression.h"
#include "postfix.h"
#include "vargen.h"
#include "utils.h"

bool is_bool_operator(tokenid_e id) {
   return id == TOKENID_OPERATOR_NOT || id == TOKENID_OPERATOR_NOT_EQUAL || id == TOKENID_OPERATOR_EQUALS
      || id == TOKENID_OPERATOR_LESS || id == TOKENID_OPERATOR_LESS_OR_EQUAL || id == TOKENID_OPERATOR_GREATER
      || id == TOKENID_OPERATOR_GREATER_OR_EQUAL;
}

vartype_e determine_token_type(token_t* token, vartable_t* vartable) {
   switch (token->id) {
      case TOKENID_IDENTIFIER:
         return vartable_find(vartable, token->value.string_value)->type;
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

vartype_e determine_expression_type(astnode_exp_t* exp, vartable_t* vartable) {
   for (int i = 0; i < exp->tokens_count; i++) {
      if (is_bool_operator(exp->tokens[i]->id)) {
         return VT_BOOL;
      }
   }
   for (int i = 0; i < exp->tokens_count; i++) {
      vartype_e type = determine_token_type(exp->tokens[i], vartable);
      if (type != VT_UNDEFINED) {
         return type;
      }
   }
   error("Given expression cannot be determined.");
}

void print_pushs_identifier(char* identifier, vartable_t* vartable) {
   char* var = generate_var_str(identifier, FT_TF, vartable_depth(vartable, identifier));
   printcm("PUSHS %s", var);
   free(var);
}

void generate_stack_expression(astnode_exp_t* exp, vartable_t* vartable) {
   for (int i = 0; i < exp->tokens_count; i++) {
      switch (exp->tokens[i]->id) {
         case TOKENID_IDENTIFIER:
            print_pushs_identifier(exp->tokens[i]->value.string_value, vartable);
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
            determine_expression_type(exp, vartable) == VT_FLOAT ? printcm("IDIVS") : printcm("DIVS");
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

void generate_local_expression(char* identifier, astnode_exp_t* exp, vartable_t* vartable) {
   char* var = generate_var_str(identifier, FT_TF, vartable_depth(vartable, identifier));
   if (exp->tokens_count == 2) {
      guard(exp->tokens[0]->id == TOKENID_IDENTIFIER && exp->tokens[1]->id == TOKENID_OPERATOR_NOT);
      char* op1 = generate_var_str(exp->tokens[0]->value.string_value, FT_TF, vartable_depth(vartable, identifier));
      printcm("NOT %s %s", var, op1);
      free(op1);
   } else {
      char* op1; 
      char* op2;
      if (is_const_tokenid(exp->tokens[0]->id)) {
         op1 = generate_const_str(exp->tokens[0]);
      } else {
         op1 = generate_var_str(exp->tokens[0]->value.string_value, FT_TF, vartable_depth(vartable, identifier)); 
      }
      if (is_const_tokenid(exp->tokens[1]->id)) {
         op2 = generate_const_str(exp->tokens[1]);
      } else {
         op2 = generate_var_str(exp->tokens[1]->value.string_value, FT_TF, vartable_depth(vartable, identifier)); 
      }
      switch (exp->tokens[2]->id) {
         case TOKENID_OPERATOR_ADD:
            printcm("ADD %s %s %s", var, op1, op2);
            break;
         case TOKENID_OPERATOR_SUB:
            printcm("SUB %s %s %s", var, op1, op2);
            break;
         case TOKENID_OPERATOR_MUL:
            printcm("MUL %s %s %s", var, op1, op2);
            break;
         case TOKENID_OPERATOR_DIV:
            determine_expression_type(exp, vartable) == VT_FLOAT ? printcm("IDIV") : printcm("DIV");
            break;
         case TOKENID_OPERATOR_AND:
            printcm("AND %s %s %s", var, op1, op2);
            break;
         case TOKENID_OPERATOR_OR:
            printcm("OR %s %s %s", var, op1, op2);
            break;
         case TOKENID_OPERATOR_NOT:
            printcm("NOT %s %s %s", var, op1, op2);
            break;
         case TOKENID_OPERATOR_EQUALS:
            printcm("EQ %s %s %s", var, op1, op2);
            break;
         case TOKENID_OPERATOR_NOT_EQUAL:
            printcm("EQ %s %s %s", var, op1, op2);
            printcm("NOT %s %s", var, var);
            break;
         case TOKENID_OPERATOR_LESS:
            printcm("LT %s %s %s", var, op1, op2);
            break;
         case TOKENID_OPERATOR_GREATER:
            printcm("GT %s %s %s", var, op1, op2);
            break;
         case TOKENID_OPERATOR_LESS_OR_EQUAL:
            printcm("GT %s %s %s", var, op1, op2);
            printcm("NOT %s %s", var, var);
            break;
         case TOKENID_OPERATOR_GREATER_OR_EQUAL:
            printcm("LT %s %s %s", var, op1, op2);
            printcm("NOT %s %s", var, var);
            break;
         default:
            exit(ERRCODE_INTERNAL_ERROR);
      }
      free(op1);
      free(op2);
   }
   free(var);
}

void generate_const_expression(char* identifier, astnode_exp_t* exp, vartable_t* vartable) {
   char* var = generate_var_str(identifier, FT_TF, vartable_depth(vartable, identifier));
   char* val;
   if (is_const_tokenid(exp->tokens[0]->id)) {
      val = generate_const_str(exp->tokens[0]);
   } else {
      val = generate_var_str(exp->tokens[0]->value.string_value, FT_TF, vartable_depth(vartable, exp->tokens[0]->value.string_value));
   }
   printcm("MOVE %s %s", var, val);
   free(var);
   free(val);
}

void generate_assign_expression(char* identifier, astnode_exp_t* exp, vartable_t* vartable) {
   infix_to_postfix(exp);
   
   if (exp->tokens_count == 1) {
      generate_const_expression(identifier, exp, vartable);
   } else if (exp->tokens_count == 2 || exp->tokens_count == 3) {
      generate_local_expression(identifier, exp, vartable);
   } else {
      generate_stack_expression(exp, vartable);
      char* var = generate_var_str(identifier, FT_TF, vartable_depth(vartable, identifier));
      printcm("POPS %s", var);
      free(var);
   }
}
