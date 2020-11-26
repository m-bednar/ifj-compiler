/**
 * IFJ20 Compiler
 * @file postfix.c
 * @authors Martin Bednář (xbedna77)
 */

#include <stdlib.h>
#include <stdbool.h>
#include "postfix.h"
#include "vargen.h"
#include "utils.h"
#include "../error.h"
#include "../memory.h"

typedef struct tstack_t {
   int length;
   token_t** memory;
} tstack_t;

tstack_t* tstack_ctor(int size) {
   tstack_t* stack = safe_alloc(sizeof(tstack_t));
   stack->memory = safe_alloc(size * sizeof(token_t*));
   stack->length = 0;
   return stack;
}

void tstack_dtor(tstack_t* stack) {
   guard(stack != NULL);
   guard(stack->memory != NULL);
   free(stack->memory);
   free(stack);
}

void tstack_push(tstack_t* stack, token_t* token) {
   guard(stack != NULL);
   stack->memory[stack->length] = token;
   stack->length++;
}

token_t* tstack_pop(tstack_t* stack) {
   guard(stack != NULL);
   guard(stack->length != 0);
   stack->length--;
   return stack->memory[stack->length];
}

token_t* tstack_peek(tstack_t* stack) {
   guard(stack != NULL);
   guard(stack->length != 0);
   return stack->memory[stack->length - 1];
}

bool tstack_isempty(tstack_t* stack) {
   return stack->length == 0;
}

bool is_operand(token_t* token) {
   return token->id == TOKENID_IDENTIFIER || token->id == TOKENID_STRING_LITERAL || token->id == TOKENID_NUM ||
      token->id == TOKENID_NUM_DECIMAL || token->id == TOKENID_BOOL_LITERAL;
}

int op_precedence(token_t* token) { 
   switch (token->id) {
      case TOKENID_OPERATOR_EQUALS:
      case TOKENID_OPERATOR_NOT_EQUAL:
      case TOKENID_OPERATOR_LESS:
      case TOKENID_OPERATOR_LESS_OR_EQUAL:
      case TOKENID_OPERATOR_GREATER:
      case TOKENID_OPERATOR_GREATER_OR_EQUAL:
         return 1;
      case TOKENID_OPERATOR_OR:
         return 2;
      case TOKENID_OPERATOR_AND:
         return 3;
      case TOKENID_OPERATOR_ADD: 
      case TOKENID_OPERATOR_SUB: 
         return 4; 
      case TOKENID_OPERATOR_MUL: 
      case TOKENID_OPERATOR_DIV: 
         return 5; 
      case TOKENID_OPERATOR_NOT: 
         return 6; 
      default:
         return -1;
   }
}

bool are_operand_equal(token_t* operand1, token_t* operand2) {
   switch (operand1->id) {
      case TOKENID_BOOL_LITERAL:
         return operand1->value.bool_value == operand2->value.bool_value;
         break;
      case TOKENID_STRING_LITERAL:
         return strcpy(operand1->value.string_value, operand2->value.string_value) == 0;
         break;
      case TOKENID_NUM_DECIMAL:
         return operand1->value.decimal_value == operand2->value.decimal_value;
         break;
      case TOKENID_NUM:
         return operand1->value.int_value == operand2->value.int_value;
         break;
      default:
         error("First operand had unexpected type");
   }
}

token_t* eval_operation(token_t* operand1, token_t* operand2, token_t* operator) {
   switch (operator->id) {
      case TOKENID_OPERATOR_AND:
         operand1->value.bool_value = operand1->value.bool_value && operand2->value.bool_value;
         break;
      case TOKENID_OPERATOR_OR:
         operand1->value.bool_value = operand1->value.bool_value || operand2->value.bool_value;
         break;
      case TOKENID_OPERATOR_EQUALS:
         operand1->value.bool_value = are_operand_equal(operand1, operand2);
         operand1->id = TOKENID_BOOL_LITERAL;
         break;
      case TOKENID_OPERATOR_NOT_EQUAL:
         operand1->value.bool_value = !are_operand_equal(operand1, operand2);
         operand1->id = TOKENID_BOOL_LITERAL;
         break;
      case TOKENID_OPERATOR_LESS:
         if (operand1->id == TOKENID_NUM) {
            operand1->value.bool_value = operand1->value.int_value < operand2->value.int_value;
         } else {
            operand1->value.bool_value = operand1->value.decimal_value < operand2->value.decimal_value;
         }
         operand1->id = TOKENID_BOOL_LITERAL;
         break;
      case TOKENID_OPERATOR_LESS_OR_EQUAL:
         if (operand1->id == TOKENID_NUM) {
            operand1->value.bool_value = operand1->value.int_value <= operand2->value.int_value;
         } else {
            operand1->value.bool_value = operand1->value.decimal_value <= operand2->value.decimal_value;
         }
         operand1->id = TOKENID_BOOL_LITERAL;
         break;
      case TOKENID_OPERATOR_GREATER:
         if (operand1->id == TOKENID_NUM) {
            operand1->value.bool_value = operand1->value.int_value > operand2->value.int_value;
         } else {
            operand1->value.bool_value = operand1->value.decimal_value > operand2->value.decimal_value;
         }
         operand1->id = TOKENID_BOOL_LITERAL;
         break;
      case TOKENID_OPERATOR_GREATER_OR_EQUAL:
         if (operand1->id == TOKENID_NUM) {
            operand1->value.bool_value = operand1->value.int_value >= operand2->value.int_value;
         } else {
            operand1->value.bool_value = operand1->value.decimal_value >= operand2->value.decimal_value;
         }
         operand1->id = TOKENID_BOOL_LITERAL;
         break;
      case TOKENID_OPERATOR_ADD:
         if (operand1->id == TOKENID_NUM) {
            operand1->value.int_value = operand1->value.int_value + operand2->value.int_value;
         } else if ((operand1->id == TOKENID_NUM_DECIMAL)) {
            operand1->value.decimal_value = operand1->value.decimal_value + operand2->value.decimal_value;
         } else {
            int len = strlen(operand1->value.string_value) + strlen(operand2->value.string_value) + 1;
            operand1->value.string_value = safe_realloc(operand1->value.string_value, sizeof(char) * len);
            strcat(operand1->value.string_value, operand2->value.string_value);
         }
         break;
      case TOKENID_OPERATOR_SUB:
         if (operand1->id == TOKENID_NUM) {
            operand1->value.int_value = operand1->value.int_value - operand2->value.int_value;
         } else {
            operand1->value.decimal_value = operand1->value.decimal_value - operand2->value.decimal_value;
         }
         break;
      case TOKENID_OPERATOR_DIV:
         if (operand1->id == TOKENID_NUM) {
            operand1->value.int_value = operand1->value.int_value / operand2->value.int_value;
         } else {
            operand1->value.decimal_value = operand1->value.decimal_value / operand2->value.decimal_value;
         }
         break;
      case TOKENID_OPERATOR_MUL:
         if (operand1->id == TOKENID_NUM) {
            operand1->value.int_value = operand1->value.int_value * operand2->value.int_value;
         } else {
            operand1->value.decimal_value = operand1->value.decimal_value * operand2->value.decimal_value;
         }
         break;
      default:
         error("Given token is not operator");
   }
   return operand1;
}

void replace_three_tokens(astnode_exp_t* exp, token_t* token, int index) {
   token_t* new = token_ctor(token->id, token->value);
   if (token->id == TOKENID_IDENTIFIER || token->id == TOKENID_STRING_LITERAL) {
      new->value.string_value = safe_alloc(strlen(token->value.string_value) + 1);
      strcpy(new->value.string_value, token->value.string_value);
   }

   token_dtor(exp->tokens[index]);
   token_dtor(exp->tokens[index + 1]);
   token_dtor(exp->tokens[index + 2]);

   exp->tokens[index] = new;

   for (int i = index + 3; i < exp->tokens_count; i++) {
      exp->tokens[i - 2] = exp->tokens[i];
   }
   
   exp->tokens_count -= 2;
}

bool optimize_by_evaluation(astnode_exp_t* exp) {
   bool flag = false; 
   bool optimized;
   do {
      optimized = false;
      for (int i = 0; i < exp->tokens_count - 2 && !optimized; i++) {
         token_t* t1 = exp->tokens[i];
         token_t* t2 = exp->tokens[i + 1];
         token_t* t3 = exp->tokens[i + 2];
         if (is_const_tokenid(t1->id) && is_const_tokenid(t2->id) && !is_operand(t3)) {
            replace_three_tokens(exp, eval_operation(t1, t2, t3), i);
            optimized = true;
         }
      }
      if (optimized) {
         flag = true;
      }
   } while (optimized);
   return flag;
}

bool is_zero_value(token_t* t) {
   return (t->id == TOKENID_NUM && t->value.int_value == 0) ||
      (t->id == TOKENID_NUM_DECIMAL && t->value.decimal_value == 0.0);
}

bool is_unary_value(token_t* t) {
   return (t->id == TOKENID_NUM && t->value.int_value == 1) ||
      (t->id == TOKENID_NUM_DECIMAL && t->value.decimal_value == 1.0);
}

bool optimize_by_arithmetics(astnode_exp_t* exp) {
   bool flag = false; 
   bool optimized;
   do {
      optimized = false;
      for (int i = 0; i < exp->tokens_count - 2 && !optimized; i++) {
         token_t* t1 = exp->tokens[i];
         token_t* t2 = exp->tokens[i + 1];
         token_t* t3 = exp->tokens[i + 2];
         if (is_operand(t1) && is_operand(t2) && !is_operand(t3)) {
            switch (t3->id) {
               case TOKENID_OPERATOR_MUL:
                  if (is_zero_value(t1)) {
                     replace_three_tokens(exp, t1, i);
                     optimized = true;
                  } else if (is_zero_value(t2)) {
                     replace_three_tokens(exp, t2, i);
                     optimized = true;
                  } else if (is_unary_value(t1)) {
                     replace_three_tokens(exp, t2, i);
                     optimized = true;
                  } else if (is_unary_value(t2)) {
                     replace_three_tokens(exp, t1, i);
                     optimized = true;
                  }
                  break;
               case TOKENID_OPERATOR_DIV:
                  if (is_zero_value(t1)) {
                     replace_three_tokens(exp, t1, i);
                     optimized = true;
                  } else if (is_zero_value(t2)) {
                     exit(ERRCODE_ZERO_DIVISION_ERROR);
                  } else if (is_unary_value(t2)) {
                     replace_three_tokens(exp, t1, i);
                     optimized = true;
                  }
                  break;
               case TOKENID_OPERATOR_ADD:
                  if (is_zero_value(t1)) {
                     replace_three_tokens(exp, t2, i);
                     optimized = true;
                  } else if (is_zero_value(t2)) {
                     replace_three_tokens(exp, t1, i);
                     optimized = true;
                  }
                  break;
               case TOKENID_OPERATOR_SUB:
                  if (is_zero_value(t2)) {
                     replace_three_tokens(exp, t1, i);
                     optimized = true;
                  }
                  break;
               default:
                  break;
            }
         }
      }
      if (optimized) {
         flag = true;
      }
   } while (optimized);
   return flag;
}

void optimize_postfix(astnode_exp_t* exp) {
   while (optimize_by_arithmetics(exp) || optimize_by_evaluation(exp));
}

void infix_to_postfix(astnode_exp_t* exp) {
   tstack_t* stack = tstack_ctor(exp->tokens_count);
   int j = 0;  // postfix length

   for (int i = 0; i < exp->tokens_count; i++) {
      if (is_operand(exp->tokens[i])) {
         exp->tokens[j++] = exp->tokens[i];
      } else if (exp->tokens[i]->id == TOKENID_LEFT_PARENTHESES) {
         tstack_push(stack, exp->tokens[i]); 
      } else if (exp->tokens[i]->id == TOKENID_RIGHT_PARENTHESES) { 
         while (!tstack_isempty(stack) && tstack_peek(stack)->id != TOKENID_LEFT_PARENTHESES) {
            exp->tokens[j++] = tstack_pop(stack);
         }
         token_dtor(tstack_pop(stack)); 
      } else { 
         // operator is encountered
         while (!tstack_isempty(stack) && op_precedence(exp->tokens[i]) <= op_precedence(tstack_peek(stack))) {
            exp->tokens[j++] = tstack_pop(stack);
         }
         tstack_push(stack, exp->tokens[i]); 
      } 
   }

   // pop rest of the operators on stack
   while (!tstack_isempty(stack)) {
      exp->tokens[j++] = tstack_pop(stack); 
   }

   exp->tokens_count = j;
   exp->tokens = safe_realloc(exp->tokens, j * sizeof(token_t*));

   tstack_dtor(stack);
   optimize_postfix(exp);
}
