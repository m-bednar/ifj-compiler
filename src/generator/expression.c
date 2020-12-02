/**
 * IFJ20 Compiler
 * @file expression.c
 * @authors Martin Bednář (xbedna77)
 */

#include "expression.h"
#include "postfix.h"
#include "vargen.h"
#include "utils.h"

/**
 * Returns true, if given operator id is resulting in boolean.
 */
bool is_bool_operator(tokenid_e id) {
   return id == TOKENID_OPERATOR_NOT || id == TOKENID_OPERATOR_NOT_EQUAL || id == TOKENID_OPERATOR_EQUALS
      || id == TOKENID_OPERATOR_LESS || id == TOKENID_OPERATOR_LESS_OR_EQUAL || id == TOKENID_OPERATOR_GREATER
      || id == TOKENID_OPERATOR_GREATER_OR_EQUAL;
}

/**
 * Whenever to use float operators insead of int operators.
 */
bool should_use_float_ops(astnode_exp_t* exp, vartable_t* vartable) {
   for (int i = 0; i < exp->tokens_count; i++) {
      if (exp->tokens[i]->id == TOKENID_NUM_DECIMAL) {
         return true;
      }
      if (exp->tokens[i]->id == TOKENID_IDENTIFIER) {
         if (vartable_find(vartable, exp->tokens[i]->value.string_value)->type == VT_FLOAT) {
            return true;
         }
      }
   }
   return false;
}

/**
 * Determines value type of given token.
 */
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

/**
 * Creates relevant var/const string depending on given token.
 */
char* generate_op_str(token_t* token, vartable_t* vartable) {
   if (is_const_tokenid(token->id)) {
      return generate_const_str(token);
   } else {
      guard(token->id == TOKENID_IDENTIFIER);
      return generate_var_str(token->value.string_value, FT_TF, vartable_depth(vartable, token->value.string_value)); 
   }
}

/**
 * Determines whenever is given token string value.
 */
bool is_string_value(token_t* token, vartable_t* vartable) {
   if (token->id == TOKENID_STRING_LITERAL) {
      return true;
   }
   if (token->id == TOKENID_IDENTIFIER) {
      return vartable_find(vartable, token->value.string_value)->type == VT_STRING;
   }
   return false;
}

/**
 * Generates PUSHS instruction with given operand.
 */
void pushs_operand(token_t* token, vartable_t* vartable) {
   if (!is_string_value(token, vartable)) {
      char* var = generate_op_str(token, vartable);
      printcm("PUSHS %s", var);
      free(var);
   }
}

/**
 * Generates expression with stack-only operations. 
 */
void generate_stack_expression(astnode_exp_t* exp, vartable_t* vartable) {
   for (int i = 0; i < exp->tokens_count; i++) {
      switch (exp->tokens[i]->id) {
         case TOKENID_IDENTIFIER:
         case TOKENID_BOOL_LITERAL:
         case TOKENID_NUM_DECIMAL:
         case TOKENID_STRING_LITERAL:
         case TOKENID_NUM:
            pushs_operand(exp->tokens[i], vartable);
            break;
         case TOKENID_OPERATOR_ADD:
            if (!is_string_value(exp->tokens[i - 1], vartable)) {
               printcm("ADDS");
            } else {
               if (i > 2 && is_string_value(exp->tokens[i - 2], vartable)) {
                  char* op1 = generate_op_str(exp->tokens[i - 2], vartable);
                  char* op2 = generate_op_str(exp->tokens[i - 1], vartable);
                  printcm("CONCAT GF@&tmp %s %s", op1, op2);
                  free(op1);
                  free(op2);
               } else {
                  char* op1 = generate_op_str(exp->tokens[i - 1], vartable);
                  printcm("POPS GF@&tmp");
                  printcm("CONCAT GF@&tmp GF@&tmp %s", op1);
                  free(op1);
               }
               printcm("PUSHS GF@&tmp");
            }
            break;
         case TOKENID_OPERATOR_SUB:
            printcm("SUBS");
            break;
         case TOKENID_OPERATOR_MUL:
            printcm("MULS");
            break;
         case TOKENID_OPERATOR_DIV:
            printcm("%s", should_use_float_ops(exp, vartable) ? "IDIVS" : "DIVS");
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

/**
 * For simple expressions, containing 2-3 tokens, this will generate operation(s) with
 * ouput to given asignee.
 */
void generate_local_expression(char* asignee, astnode_exp_t* exp, vartable_t* vartable) {
   if (exp->tokens_count == 2) {
      guard(exp->tokens[0]->id == TOKENID_IDENTIFIER && exp->tokens[1]->id == TOKENID_OPERATOR_NOT);
      char* op1 = generate_op_str(exp->tokens[0], vartable);
      printcm("NOT %s %s", asignee, op1);
      free(op1);
   } else {
      char* op1 = generate_op_str(exp->tokens[0], vartable); 
      char* op2 = generate_op_str(exp->tokens[1], vartable); 
      switch (exp->tokens[2]->id) {
         case TOKENID_OPERATOR_ADD:
            printcm("%s %s %s %s", is_string_value(exp->tokens[0], vartable)? "CONCAT" : "ADD", asignee, op1, op2);
            break;
         case TOKENID_OPERATOR_SUB:
            printcm("SUB %s %s %s", asignee, op1, op2);
            break;
         case TOKENID_OPERATOR_MUL:
            printcm("MUL %s %s %s", asignee, op1, op2);
            break;
         case TOKENID_OPERATOR_DIV:
            printcm("%s %s %s %s", should_use_float_ops(exp, vartable) ? "IDIV" : "DIV", asignee, op1, op2);
            break;
         case TOKENID_OPERATOR_AND:
            printcm("AND %s %s %s", asignee, op1, op2);
            break;
         case TOKENID_OPERATOR_OR:
            printcm("OR %s %s %s", asignee, op1, op2);
            break;
         case TOKENID_OPERATOR_NOT:
            printcm("NOT %s %s %s", asignee, op1, op2);
            break;
         case TOKENID_OPERATOR_EQUALS:
            printcm("EQ %s %s %s", asignee, op1, op2);
            break;
         case TOKENID_OPERATOR_NOT_EQUAL:
            printcm("EQ %s %s %s", asignee, op1, op2);
            printcm("NOT %s %s", asignee, asignee);
            break;
         case TOKENID_OPERATOR_LESS:
            printcm("LT %s %s %s", asignee, op1, op2);
            break;
         case TOKENID_OPERATOR_GREATER:
            printcm("GT %s %s %s", asignee, op1, op2);
            break;
         case TOKENID_OPERATOR_LESS_OR_EQUAL:
            printcm("GT %s %s %s", asignee, op1, op2);
            printcm("NOT %s %s", asignee, asignee);
            break;
         case TOKENID_OPERATOR_GREATER_OR_EQUAL:
            printcm("LT %s %s %s", asignee, op1, op2);
            printcm("NOT %s %s", asignee, asignee);
            break;
         default:
            exit(ERRCODE_INTERNAL_ERROR);
      }
      free(op1);
      free(op2);
   }
}

/**
 * For expression that is single variable/const, this will generate assign
 * to appropriate variable.
 */
void generate_const_assign_expression(char* asignee, astnode_exp_t* exp, vartable_t* vartable) {
   char* val = generate_op_str(exp->tokens[0], vartable);
   printcm("MOVE %s %s", asignee, val);
   free(val);
}

/**
 * For expression that is single variable/const, this will generate assign
 * that will move this value in main global variable or on stack.
 */
bool generate_const_expression(astnode_exp_t* exp, vartable_t* vartable, bool prefer_stack) {
   char* var = generate_op_str(exp->tokens[0], vartable);
   if (prefer_stack) {
      printcm("PUSH %s", var);
   } else {
      printcm("MOVE GF@$tmp0 %s", var);
   }
   free(var);
   return prefer_stack;
}

bool generate_expression(astnode_exp_t* exp, vartable_t* vartable, bool prefer_stack) {
   infix_to_postfix(exp);
   if (exp->tokens_count == 1) { 
      return generate_const_expression(exp, vartable, prefer_stack);
   } else if (exp->tokens_count <= 3 && !prefer_stack) {
      generate_local_expression("GF@$tmp0", exp, vartable);
      return false;
   } else {
      generate_stack_expression(exp, vartable);
      return true;
   }
}

void generate_assign_expression(char* asignee, astnode_exp_t* exp, vartable_t* vartable) {
   infix_to_postfix(exp);
   if (exp->tokens_count == 1) {
      generate_const_assign_expression(asignee, exp, vartable);
   } else if (exp->tokens_count <= 3) {
      generate_local_expression(asignee, exp, vartable);
   } else {
      generate_stack_expression(exp, vartable);
      printcm("POPS %s", asignee);
   }
}
