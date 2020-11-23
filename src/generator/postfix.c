/**
 * IFJ20 Compiler
 * @file postfix.c
 * @authors Martin Bednář (xbedna77)
 */

#include <stdlib.h>
#include <stdbool.h>
#include "postfix.h"
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
}
