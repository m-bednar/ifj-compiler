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

bool should_use_string_ops(astnode_exp_t* exp, vartable_t* vartable) {
   for (int i = 0; i < exp->tokens_count; i++) {
      if (exp->tokens[i]->id == TOKENID_STRING_LITERAL) {
         return true;
      }
      if (exp->tokens[i]->id == TOKENID_IDENTIFIER) {
         if (vartable_find(vartable, exp->tokens[i]->value.string_value)->type == VT_STRING) {
            return true;
         }
      }
   }
   return false;
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

char* generate_op_str(token_t* op, vartable_t* vartable) {
   if (is_const_tokenid(op->id)) {
      return generate_const_str(op);
   } else {
      guard(op->id == TOKENID_IDENTIFIER);
      return generate_var_str(op->value.string_value, FT_TF, vartable_depth(vartable, op->value.string_value)); 
   }
}

void generate_stack_expression(astnode_exp_t* exp, vartable_t* vartable) {
   for (int i = 0; i < exp->tokens_count; i++) {
      char* var;
      switch (exp->tokens[i]->id) {
         case TOKENID_IDENTIFIER:
         case TOKENID_BOOL_LITERAL:
         case TOKENID_NUM_DECIMAL:
         case TOKENID_NUM:
            var = generate_op_str(exp->tokens[i], vartable);
            printcm("PUSHS %s", var);
            free(var);
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
            printcm("%s %s %s %s", should_use_string_ops(exp, vartable)? "CONCAT" : "ADD", asignee, op1, op2);
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

void generate_const_assign_expression(char* asignee, astnode_exp_t* exp, vartable_t* vartable) {
   char* val = generate_op_str(exp->tokens[0], vartable);
   printcm("MOVE %s %s", asignee, val);
   free(val);
}

bool generate_string_expression(char* asignee, astnode_exp_t* exp, vartable_t* vartable, bool prefer_stack) {
   exp = exp;
   vartable = vartable;
   asignee = asignee;

   if (exp->tokens_count == 1) {
      char* op1 = generate_op_str(exp->tokens[0], vartable);
      if (prefer_stack) {
         printcm("PUSHS %s", op1);
      } else {
         printcm("MOVE %s %s", asignee, op1);
      }
      free(op1);
      return prefer_stack;
   }

   if (exp->tokens_count == 3) {
      bool stack_used = false;
      char* op1 = generate_op_str(exp->tokens[0], vartable);
      char* op2 = generate_op_str(exp->tokens[2], vartable);
      switch (exp->tokens[1]->id) {
         case TOKENID_OPERATOR_ADD:
            printcm("CONCAT %s %s %s", asignee, op1, op2);
            break;
         case TOKENID_OPERATOR_EQUALS:
            if (prefer_stack) {
               printcm("PUSHS %s", op1);
               printcm("PUSHS %s", op2);
               printcm("EQS");
            } else {
               printcm("EQ %s %s %s", asignee, op1, op2);
            }
            stack_used = prefer_stack;
            break;
         case TOKENID_OPERATOR_NOT_EQUAL:
            if (prefer_stack) {
               printcm("PUSHS %s", op1);
               printcm("PUSHS %s", op2);
               printcm("EQS");
               printcm("NOTS");
            } else {
               printcm("EQ %s %s %s", asignee, op1, op2);
               printcm("NOT %s %s", asignee, asignee);
            }
            break;
         default:
            error("Wrong operator used on strings.");
      }
      free(op1);
      free(op2);
      return stack_used;
   }

   for (int i = 0; i < exp->tokens_count; i++) {
      // TODO:
   }
   return false;
}

bool generate_const_expression(astnode_exp_t* exp, vartable_t* vartable, bool prefer_stack) {
   char* var = generate_op_str(exp->tokens[0], vartable);
   if (prefer_stack) {
      printcm("PUSH %s", var);
   } else {
      printcm("MOVE GF@$tmp %s", var);
   }
   free(var);
   return prefer_stack;
}

bool generate_expression(astnode_exp_t* exp, vartable_t* vartable, bool prefer_stack) {
   if (should_use_string_ops(exp, vartable)) {
      return generate_string_expression("GF@$tmp", exp, vartable, prefer_stack);
   } else {
      infix_to_postfix(exp);
      if (exp->tokens_count == 1) { 
         return generate_const_expression(exp, vartable, prefer_stack);
      } else if (exp->tokens_count <= 3 && !prefer_stack) {
         generate_local_expression("GF@$tmp", exp, vartable);
         return false;
      } else {
         generate_stack_expression(exp, vartable);
         return true;
      }
   }
}

void generate_assign_expression(char* asignee, astnode_exp_t* exp, vartable_t* vartable) {
   if (should_use_string_ops(exp, vartable)) {
      generate_string_expression(asignee, exp, vartable, false);
   } else {
      infix_to_postfix(exp);
      if (exp->tokens_count == 1) {
         generate_const_assign_expression(asignee, exp, vartable);
      } else if (exp->tokens_count <= 3) {
         generate_local_expression(asignee, exp, vartable);
      } else {
         char* var = generate_var_str(asignee, FT_TF, vartable_depth(vartable, asignee));
         generate_stack_expression(exp, vartable);
         printcm("POPS %s", var);
         free(var);
      }
   }
   
}
