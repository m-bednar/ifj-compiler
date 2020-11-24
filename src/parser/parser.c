/**
 * IFJ20 Compiler
 * @file parser.c
 * @authors Daniel Havranek (xhavra18)
 */

#include "parser.h"


/**
 * Nonterminal derivations for predictive parsing.
 * Derivates nonterminals based on LL grammar rules.
 * Returns true if any error occurs.
 */
bool nonterminal_expression_next_derivation(ntsymstack_t* stack, int token_id) {
   if (token_id == TOKENID_NEWLINE || token_id == TOKENID_LEFT_BRACKET) {
      ntsymbol_dtor(ntsymstack_pop(stack));
      return false;
   } else if (token_id == TOKENID_COMMA) {
      ntsymstack_push(stack, ntsymbol_ctor(NONTERMINAL_EXPRESSION, false));
      ntsymstack_push(stack, ntsymbol_ctor(TOKENID_COMMA, true));
      return false;
   } else {
      return true;
   }
}

bool nonterminal_expressions_derivation(ntsymstack_t* stack, int token_id) {
   if (token_id == TOKENID_IDENTIFIER || token_id == TOKENID_NUM || token_id == TOKENID_NUM_DECIMAL ||
         token_id == TOKENID_STRING_LITERAL || token_id == TOKENID_BOOL_LITERAL || token_id == TOKENID_OPERATOR_NOT ||
         token_id == TOKENID_LEFT_PARENTHESES) {
      ntsymbol_dtor(ntsymstack_pop(stack));
      ntsymstack_push(stack, ntsymbol_ctor(NONTERMINAL_EXPRESSION_NEXT, false));
      ntsymstack_push(stack, ntsymbol_ctor(NONTERMINAL_EXPRESSION, false));
      return false;
   } else if (token_id == TOKENID_NEWLINE) {
      ntsymbol_dtor(ntsymstack_pop(stack));
      return false;
   } else {
      return true;
   }
}

bool nonterminal_term_next_derivation(ntsymstack_t* stack, int token_id) {
   if (token_id == TOKENID_COMMA) {
      ntsymstack_push(stack, ntsymbol_ctor(NONTERMINAL_TERM, false));
      ntsymstack_push(stack, ntsymbol_ctor(TOKENID_COMMA, true));
      return false;
   } else if (token_id == TOKENID_RIGHT_PARENTHESES) {
      ntsymbol_dtor(ntsymstack_pop(stack));
      return false;
   } else {
      return true;
   }
}

bool nonterminal_term_derivation(ntsymstack_t* stack, int token_id) {
   if (token_id == TOKENID_IDENTIFIER) {
      ntsymbol_dtor(ntsymstack_pop(stack));
      ntsymstack_push(stack, ntsymbol_ctor(TOKENID_IDENTIFIER, true));
      return false;
   } else if (token_id == TOKENID_NUM) {
      ntsymbol_dtor(ntsymstack_pop(stack));
      ntsymstack_push(stack, ntsymbol_ctor(TOKENID_NUM, true));
      return false;
   } else if (token_id == TOKENID_NUM_DECIMAL) {
      ntsymbol_dtor(ntsymstack_pop(stack));
      ntsymstack_push(stack, ntsymbol_ctor(TOKENID_NUM_DECIMAL, true));
      return false;
   } else if (token_id == TOKENID_STRING_LITERAL) {
      ntsymbol_dtor(ntsymstack_pop(stack));
      ntsymstack_push(stack, ntsymbol_ctor(TOKENID_STRING_LITERAL, true));
      return false;
   } else if (token_id == TOKENID_BOOL_LITERAL) {
      ntsymbol_dtor(ntsymstack_pop(stack));
      ntsymstack_push(stack, ntsymbol_ctor(TOKENID_BOOL_LITERAL, true));
      return false;
   } else {
      return true;
   }
}

bool nonterminal_terms_derivation(ntsymstack_t* stack, int token_id) {
   if (token_id == TOKENID_IDENTIFIER || token_id == TOKENID_NUM || token_id == TOKENID_NUM_DECIMAL || 
         token_id == TOKENID_STRING_LITERAL || token_id == TOKENID_BOOL_LITERAL) {
      ntsymbol_dtor(ntsymstack_pop(stack));
      ntsymstack_push(stack, ntsymbol_ctor(NONTERMINAL_TERM_NEXT, false));
      ntsymstack_push(stack, ntsymbol_ctor(NONTERMINAL_TERM, false));
      return false;
   } else if (token_id == TOKENID_RIGHT_PARENTHESES) {
      ntsymbol_dtor(ntsymstack_pop(stack));
      return false;
   } else {
      return true;
   }
}

bool nonterminal_call_derivation(ntsymstack_t* stack, int token_id) {
   if (token_id == TOKENID_IDENTIFIER) {
      ntsymbol_dtor(ntsymstack_pop(stack));
      ntsymstack_push(stack, ntsymbol_ctor(TOKENID_RIGHT_PARENTHESES, true));
      ntsymstack_push(stack, ntsymbol_ctor(NONTERMINAL_TERMS, false));
      ntsymstack_push(stack, ntsymbol_ctor(TOKENID_LEFT_PARENTHESES, true));
      ntsymstack_push(stack, ntsymbol_ctor(TOKENID_IDENTIFIER, true));
      return false;
   } else {
      return true;
   }
}

bool nonterminal_for_derivation(ntsymstack_t* stack, int token_id) {
   if (token_id == TOKENID_KEYWORD_FOR) {
      ntsymbol_dtor(ntsymstack_pop(stack));
      ntsymstack_push(stack, ntsymbol_ctor(TOKENID_RIGHT_BRACKET, true));
      ntsymstack_push(stack, ntsymbol_ctor(NONTERMINAL_COMMANDS, false));
      ntsymstack_push(stack, ntsymbol_ctor(TOKENID_NEWLINE, true));
      ntsymstack_push(stack, ntsymbol_ctor(TOKENID_LEFT_BRACKET, true));
      ntsymstack_push(stack, ntsymbol_ctor(NONTERMINAL_FOR_ASSIGNMENT, false));
      ntsymstack_push(stack, ntsymbol_ctor(TOKENID_SEMICOLON, true));
      ntsymstack_push(stack, ntsymbol_ctor(NONTERMINAL_EXPRESSION, false));
      ntsymstack_push(stack, ntsymbol_ctor(TOKENID_SEMICOLON, true));
      ntsymstack_push(stack, ntsymbol_ctor(NONTERMINAL_FOR_DEFINITION, false));
      ntsymstack_push(stack, ntsymbol_ctor(TOKENID_KEYWORD_FOR, true));
      return false;
   } else {
      return true;
   }
}

bool nonterminal_else_if_derivation(ntsymstack_t* stack, int token_id, int token_next_id) {
   if (token_id == TOKENID_NEWLINE) {
      ntsymbol_dtor(ntsymstack_pop(stack));
      return false;
   } else if (token_id == TOKENID_KEYWORD_ELSE) {
      if (token_next_id == TOKENID_KEYWORD_IF) {
         ntsymstack_push(stack, ntsymbol_ctor(TOKENID_RIGHT_BRACKET, true));
         ntsymstack_push(stack, ntsymbol_ctor(NONTERMINAL_COMMANDS, false));
         ntsymstack_push(stack, ntsymbol_ctor(TOKENID_NEWLINE, true));
         ntsymstack_push(stack, ntsymbol_ctor(TOKENID_LEFT_BRACKET, true));
         ntsymstack_push(stack, ntsymbol_ctor(NONTERMINAL_EXPRESSION, false));
         ntsymstack_push(stack, ntsymbol_ctor(TOKENID_KEYWORD_IF, true));
         ntsymstack_push(stack, ntsymbol_ctor(TOKENID_KEYWORD_ELSE, true));
         return false;
      } else if (token_next_id == TOKENID_LEFT_BRACKET) {
         ntsymbol_dtor(ntsymstack_pop(stack));
         ntsymstack_push(stack, ntsymbol_ctor(TOKENID_RIGHT_BRACKET, true));
         ntsymstack_push(stack, ntsymbol_ctor(NONTERMINAL_COMMANDS, false));
         ntsymstack_push(stack, ntsymbol_ctor(TOKENID_NEWLINE, true));
         ntsymstack_push(stack, ntsymbol_ctor(TOKENID_LEFT_BRACKET, true));
         ntsymstack_push(stack, ntsymbol_ctor(TOKENID_KEYWORD_ELSE, true));
         return false;
      } else {
         return true;
      }
   } else {
      return true;
   }
}

bool nonterminal_if_derivation(ntsymstack_t* stack, int token_id) {
   if (token_id == TOKENID_KEYWORD_IF) {
      ntsymbol_dtor(ntsymstack_pop(stack));
      ntsymstack_push(stack, ntsymbol_ctor(NONTERMINAL_ELSE_IF, false));
      ntsymstack_push(stack, ntsymbol_ctor(TOKENID_RIGHT_BRACKET, true));
      ntsymstack_push(stack, ntsymbol_ctor(NONTERMINAL_COMMANDS, false));
      ntsymstack_push(stack, ntsymbol_ctor(TOKENID_NEWLINE, true));
      ntsymstack_push(stack, ntsymbol_ctor(TOKENID_LEFT_BRACKET, true));
      ntsymstack_push(stack, ntsymbol_ctor(NONTERMINAL_EXPRESSION, false));
      ntsymstack_push(stack, ntsymbol_ctor(TOKENID_KEYWORD_IF, true));
      return false;
   } else {
      return true;
   }
}

bool nonterminal_command_derivation(ntsymstack_t* stack, int token_id, int token_next_id) {
   if (token_id == TOKENID_IDENTIFIER) {
      if (token_next_id == TOKENID_OPERATOR_DECLARE) {
         ntsymbol_dtor(ntsymstack_pop(stack));
         ntsymstack_push(stack, ntsymbol_ctor(NONTERMINAL_DEFINITION, false));
         return false;
      } else if (token_next_id == TOKENID_COMMA ||
                      token_next_id == TOKENID_OPERATOR_ASSIGN) {
         ntsymbol_dtor(ntsymstack_pop(stack));
         ntsymstack_push(stack, ntsymbol_ctor(NONTERMINAL_ASSIGNMENT, false));
         return false;
      } else if (token_next_id == TOKENID_LEFT_PARENTHESES) {
         ntsymbol_dtor(ntsymstack_pop(stack));
         ntsymstack_push(stack, ntsymbol_ctor(NONTERMINAL_CALL, false));
         return false;
      } else {
         return true;
      }
   } else if (token_id == TOKENID_KEYWORD_IF) {
      ntsymbol_dtor(ntsymstack_pop(stack));
      ntsymstack_push(stack, ntsymbol_ctor(NONTERMINAL_IF, false));
      return false;
   } else if (token_id == TOKENID_KEYWORD_FOR) {
      ntsymbol_dtor(ntsymstack_pop(stack));
      ntsymstack_push(stack, ntsymbol_ctor(NONTERMINAL_FOR, false));
      return false;
   } else if (token_id == TOKENID_KEYWORD_RETURN) {
      ntsymbol_dtor(ntsymstack_pop(stack));
      ntsymstack_push(stack, ntsymbol_ctor(NONTERMINAL_RETURN, false));
      return false;
   } else {
      return true;
   }
}

bool nonterminal_commands_derivation(ntsymstack_t* stack, int token_id) {
   if (token_id == TOKENID_IDENTIFIER || token_id == TOKENID_KEYWORD_IF || token_id == TOKENID_KEYWORD_FOR || 
         token_id == TOKENID_KEYWORD_RETURN) {
      ntsymstack_push(stack, ntsymbol_ctor(TOKENID_NEWLINE, true));
      ntsymstack_push(stack, ntsymbol_ctor(NONTERMINAL_COMMAND, false));
      return false;
   } else if (token_id == TOKENID_KEYWORD_RETURN || token_id == TOKENID_RIGHT_BRACKET) {
      ntsymbol_dtor(ntsymstack_pop(stack));
      return false;
   } else {
      return true;
   }
}

bool nonterminal_id_next_derivation(ntsymstack_t* stack, int token_id) {
   if (token_id == TOKENID_OPERATOR_ASSIGN || token_id == TOKENID_RIGHT_PARENTHESES) {
      ntsymbol_dtor(ntsymstack_pop(stack));
      return false;
   } else if (token_id == TOKENID_COMMA) {
      ntsymstack_push(stack, ntsymbol_ctor(TOKENID_IDENTIFIER, true));
      ntsymstack_push(stack, ntsymbol_ctor(TOKENID_COMMA, true));
      return false;
   } else {
      return true;
   }
}

bool nonterminal_for_assignment_right_derivation(ntsymstack_t* stack, int token_id, int token_next_id) {
   if (token_id == TOKENID_IDENTIFIER) {
      if (token_next_id == TOKENID_LEFT_PARENTHESES) {
         ntsymbol_dtor(ntsymstack_pop(stack));
         ntsymstack_push(stack, ntsymbol_ctor(NONTERMINAL_CALL, false));
         return false;
      } else if (token_next_id == TOKENID_LEFT_BRACKET || token_next_id == TOKENID_COMMA || token_next_id == TOKENID_OPERATOR_OR ||
                     token_next_id == TOKENID_OPERATOR_AND || token_next_id == TOKENID_OPERATOR_NOT_EQUAL || 
                     token_next_id == TOKENID_OPERATOR_EQUALS || token_next_id == TOKENID_OPERATOR_LESS ||
                     token_next_id == TOKENID_OPERATOR_LESS_OR_EQUAL || token_next_id == TOKENID_OPERATOR_GREATER ||
                     token_next_id == TOKENID_OPERATOR_GREATER_OR_EQUAL || token_next_id == TOKENID_OPERATOR_ADD ||
                     token_next_id == TOKENID_OPERATOR_SUB || token_next_id == TOKENID_OPERATOR_MUL ||
                     token_next_id == TOKENID_OPERATOR_DIV) {
         ntsymbol_dtor(ntsymstack_pop(stack));
         ntsymstack_push(stack, ntsymbol_ctor(NONTERMINAL_EXPRESSION_NEXT, false));
         ntsymstack_push(stack, ntsymbol_ctor(NONTERMINAL_EXPRESSION, false));
         return false;
      } else {
         return true;
      }
   } else if (token_id == TOKENID_LEFT_PARENTHESES || token_id == TOKENID_OPERATOR_NOT || token_id == TOKENID_NUM ||
                  token_id == TOKENID_NUM_DECIMAL || token_id == TOKENID_STRING_LITERAL || token_id == TOKENID_BOOL_LITERAL) {
      ntsymbol_dtor(ntsymstack_pop(stack));
      ntsymstack_push(stack, ntsymbol_ctor(NONTERMINAL_EXPRESSION_NEXT, false));
      ntsymstack_push(stack, ntsymbol_ctor(NONTERMINAL_EXPRESSION, false));
      return false;
   } else {
      return true;
   }
}

bool nonterminal_assignment_right_derivation(ntsymstack_t* stack, int token_id, int token_next_id) {
   if (token_id == TOKENID_IDENTIFIER) {
      if (token_next_id == TOKENID_LEFT_PARENTHESES) {
         ntsymbol_dtor(ntsymstack_pop(stack));
         ntsymstack_push(stack, ntsymbol_ctor(NONTERMINAL_CALL, false));
         return false;
      } else if (token_next_id == TOKENID_NEWLINE || token_next_id == TOKENID_COMMA || token_next_id == TOKENID_OPERATOR_OR ||
                     token_next_id == TOKENID_OPERATOR_AND || token_next_id == TOKENID_OPERATOR_NOT_EQUAL ||
                     token_next_id == TOKENID_OPERATOR_EQUALS || token_next_id == TOKENID_OPERATOR_LESS ||
                     token_next_id == TOKENID_OPERATOR_LESS_OR_EQUAL || token_next_id == TOKENID_OPERATOR_GREATER ||
                     token_next_id == TOKENID_OPERATOR_GREATER_OR_EQUAL || token_next_id == TOKENID_OPERATOR_ADD ||
                     token_next_id == TOKENID_OPERATOR_SUB || token_next_id == TOKENID_OPERATOR_MUL ||
                     token_next_id == TOKENID_OPERATOR_DIV) {
         ntsymbol_dtor(ntsymstack_pop(stack));
         ntsymstack_push(stack, ntsymbol_ctor(NONTERMINAL_EXPRESSION_NEXT, false));
         ntsymstack_push(stack, ntsymbol_ctor(NONTERMINAL_EXPRESSION, false));
         return false;
      } else {
         return true;
      }
   } else if (token_id == TOKENID_LEFT_PARENTHESES || token_id == TOKENID_OPERATOR_NOT || token_id == TOKENID_NUM ||
                  token_id == TOKENID_NUM_DECIMAL || token_id == TOKENID_STRING_LITERAL || token_id == TOKENID_BOOL_LITERAL) {
      ntsymbol_dtor(ntsymstack_pop(stack));
      ntsymstack_push(stack, ntsymbol_ctor(NONTERMINAL_EXPRESSION_NEXT, false));
      ntsymstack_push(stack, ntsymbol_ctor(NONTERMINAL_EXPRESSION, false));
      return false;
   } else {
      return true;
   }
}

bool nonterminal_for_assignment_derivation(ntsymstack_t* stack, int token_id) {
   if (token_id == TOKENID_IDENTIFIER) {
      ntsymbol_dtor(ntsymstack_pop(stack));
      ntsymstack_push(stack, ntsymbol_ctor(NONTERMINAL_FOR_ASSIGNMENT_RIGHT, false));
      ntsymstack_push(stack, ntsymbol_ctor(TOKENID_OPERATOR_ASSIGN, true));
      ntsymstack_push(stack, ntsymbol_ctor(NONTERMINAL_ID_NEXT, false));
      ntsymstack_push(stack, ntsymbol_ctor(TOKENID_IDENTIFIER, true));
      return false;
   } else if (token_id == TOKENID_LEFT_BRACKET) {
      ntsymbol_dtor(ntsymstack_pop(stack));
      return false;
   } else {
      return true;
   }
}

bool nonterminal_assignment_derivation(ntsymstack_t* stack, int token_id) {
   if (token_id == TOKENID_IDENTIFIER) {
      ntsymbol_dtor(ntsymstack_pop(stack));
      ntsymstack_push(stack, ntsymbol_ctor(NONTERMINAL_ASSIGNMENT_RIGHT, false));
      ntsymstack_push(stack, ntsymbol_ctor(TOKENID_OPERATOR_ASSIGN, true));
      ntsymstack_push(stack, ntsymbol_ctor(NONTERMINAL_ID_NEXT, false));
      ntsymstack_push(stack, ntsymbol_ctor(TOKENID_IDENTIFIER, true));
      return false;
   } else {
      return true;
   }
}

bool nonterminal_for_definition_derivation(ntsymstack_t* stack, int token_id) {
   if (token_id == TOKENID_IDENTIFIER) {
      ntsymbol_dtor(ntsymstack_pop(stack));
      ntsymstack_push(stack, ntsymbol_ctor(NONTERMINAL_EXPRESSION, false));
      ntsymstack_push(stack, ntsymbol_ctor(TOKENID_OPERATOR_DECLARE, true));
      ntsymstack_push(stack, ntsymbol_ctor(TOKENID_IDENTIFIER, true));
      return false;
   } else if (token_id == TOKENID_SEMICOLON) {
      ntsymbol_dtor(ntsymstack_pop(stack));
      return false;
   } else {
      return true;
   }
}

bool nonterminal_definition_derivation(ntsymstack_t* stack, int token_id) {
   if (token_id == TOKENID_IDENTIFIER) {
      ntsymbol_dtor(ntsymstack_pop(stack));
      ntsymstack_push(stack, ntsymbol_ctor(NONTERMINAL_EXPRESSION, false));
      ntsymstack_push(stack, ntsymbol_ctor(TOKENID_OPERATOR_DECLARE, true));
      ntsymstack_push(stack, ntsymbol_ctor(TOKENID_IDENTIFIER, true));
      return false;
   } else {
      return true;
   }
}

bool nonterminal_return_derivation(ntsymstack_t* stack, int token_id) {
   if (token_id == TOKENID_KEYWORD_RETURN) {
      ntsymbol_dtor(ntsymstack_pop(stack));
      ntsymstack_push(stack, ntsymbol_ctor(NONTERMINAL_EXPRESSIONS, false));
      ntsymstack_push(stack, ntsymbol_ctor(TOKENID_KEYWORD_RETURN, true));
      return false;
   } else {
      return true;
   }
}

bool nonterminal_type_next_derivation(ntsymstack_t* stack, int token_id) {
   if (token_id == TOKENID_RIGHT_PARENTHESES) {
      ntsymbol_dtor(ntsymstack_pop(stack));
      return false;
   } else if (token_id == TOKENID_COMMA) {
      ntsymstack_push(stack, ntsymbol_ctor(NONTERMINAL_TYPE, false));
      ntsymstack_push(stack, ntsymbol_ctor(TOKENID_COMMA, true));
      return false;
   } else {
      return true;
   }
}

bool nonterminal_types_derivation(ntsymstack_t* stack, int token_id) {
   if (token_id == TOKENID_KEYWORD_INT || token_id == TOKENID_KEYWORD_FLOAT64 || token_id == TOKENID_KEYWORD_STRING ||
         token_id == TOKENID_KEYWORD_BOOL) {
      ntsymbol_dtor(ntsymstack_pop(stack));
      ntsymstack_push(stack, ntsymbol_ctor(NONTERMINAL_TYPE_NEXT, false));
      ntsymstack_push(stack, ntsymbol_ctor(NONTERMINAL_TYPE, false));
      return false;
   } else if (token_id == TOKENID_RIGHT_PARENTHESES) {
      ntsymbol_dtor(ntsymstack_pop(stack));
      return false;
   } else {
      return true;
   }
}

bool nonterminal_return_types_derivation(ntsymstack_t* stack, int token_id) {
   if (token_id == TOKENID_LEFT_PARENTHESES) {
      ntsymbol_dtor(ntsymstack_pop(stack));
      ntsymstack_push(stack, ntsymbol_ctor(TOKENID_RIGHT_PARENTHESES, true));
      ntsymstack_push(stack, ntsymbol_ctor(NONTERMINAL_TYPES, false));
      ntsymstack_push(stack, ntsymbol_ctor(TOKENID_LEFT_PARENTHESES, true));
      return false;
   } else if (token_id == TOKENID_LEFT_BRACKET) {
      ntsymbol_dtor(ntsymstack_pop(stack));
      return false;
   } else {
      return true;
   }
}

bool nonterminal_type_derivation(ntsymstack_t* stack, int token_id) {
   if (token_id == TOKENID_KEYWORD_INT) {
      ntsymbol_dtor(ntsymstack_pop(stack));
      ntsymstack_push(stack, ntsymbol_ctor(TOKENID_KEYWORD_INT, true));
      return false;
   } else if (token_id == TOKENID_KEYWORD_FLOAT64) {
      ntsymbol_dtor(ntsymstack_pop(stack));
      ntsymstack_push(stack, ntsymbol_ctor(TOKENID_KEYWORD_FLOAT64, true));
      return false;
   } else if (token_id == TOKENID_KEYWORD_STRING) {
      ntsymbol_dtor(ntsymstack_pop(stack));
      ntsymstack_push(stack, ntsymbol_ctor(TOKENID_KEYWORD_STRING, true));
      return false;
   } else if (token_id == TOKENID_KEYWORD_BOOL) {
      ntsymbol_dtor(ntsymstack_pop(stack));
      ntsymstack_push(stack, ntsymbol_ctor(TOKENID_KEYWORD_BOOL, true));
      return false;
   } else {
      return true;
   }
}

bool nonterminal_parameter_next_derivation(ntsymstack_t* stack, int token_id) {
   if (token_id == TOKENID_RIGHT_PARENTHESES) {
      ntsymbol_dtor(ntsymstack_pop(stack));
      return false;
   } else if (token_id == TOKENID_COMMA) {
      ntsymstack_push(stack, ntsymbol_ctor(NONTERMINAL_PARAMETER, false));
      ntsymstack_push(stack, ntsymbol_ctor(TOKENID_COMMA, true));
      return false;
   } else {
      return true;
   }
}

bool nonterminal_parameter_derivation(ntsymstack_t* stack, int token_id) {
   if (token_id == TOKENID_IDENTIFIER) {
      ntsymbol_dtor(ntsymstack_pop(stack));
      ntsymstack_push(stack, ntsymbol_ctor(NONTERMINAL_TYPE, false));
      ntsymstack_push(stack, ntsymbol_ctor(TOKENID_IDENTIFIER, true));
      return false;
   } else {
      return true;
   }
}

bool nonterminal_parameters_derivation(ntsymstack_t* stack, int token_id) {
   if (token_id == TOKENID_IDENTIFIER) {
      ntsymbol_dtor(ntsymstack_pop(stack));
      ntsymstack_push(stack, ntsymbol_ctor(NONTERMINAL_PARAMETER_NEXT, false));
      ntsymstack_push(stack, ntsymbol_ctor(NONTERMINAL_PARAMETER, false));
      return false;
   } else if (token_id == TOKENID_RIGHT_PARENTHESES) {
      ntsymbol_dtor(ntsymstack_pop(stack));
      return false;
   } else {
      return true;
   }
}

bool nonterminal_function_derivation(ntsymstack_t* stack, int token_id) {
   if (token_id == TOKENID_KEYWORD_FUNC) {
      ntsymbol_dtor(ntsymstack_pop(stack));
      ntsymstack_push(stack, ntsymbol_ctor(TOKENID_RIGHT_BRACKET, true));
      ntsymstack_push(stack, ntsymbol_ctor(NONTERMINAL_COMMANDS, false));
      ntsymstack_push(stack, ntsymbol_ctor(TOKENID_NEWLINE, true));
      ntsymstack_push(stack, ntsymbol_ctor(TOKENID_LEFT_BRACKET, true));
      ntsymstack_push(stack, ntsymbol_ctor(NONTERMINAL_RETURN_TYPES, false));
      ntsymstack_push(stack, ntsymbol_ctor(TOKENID_RIGHT_PARENTHESES, true));
      ntsymstack_push(stack, ntsymbol_ctor(NONTERMINAL_PARAMETERS, false));
      ntsymstack_push(stack, ntsymbol_ctor(TOKENID_LEFT_PARENTHESES, true));
      ntsymstack_push(stack, ntsymbol_ctor(TOKENID_IDENTIFIER, true));
      ntsymstack_push(stack, ntsymbol_ctor(TOKENID_KEYWORD_FUNC, true));
      return false;
   } else {
      return true;
   }
}

bool nonterminal_functions_derivation(ntsymstack_t* stack, int token_id) {
   if (token_id == TOKENID_KEYWORD_FUNC) {
      ntsymstack_push(stack, ntsymbol_ctor(TOKENID_NEWLINE, true));
      ntsymstack_push(stack, ntsymbol_ctor(NONTERMINAL_FUNCTION, false));
      return false;
   } else if (token_id == TOKENID_END_OF_FILE) {
      ntsymbol_dtor(ntsymstack_pop(stack));
      return false;
   } else {
      return true;
   }
}

bool nonterminal_package_derivation(ntsymstack_t* stack, int token_id) {
   if (token_id == TOKENID_KEYWORD_PACKAGE) {
      ntsymbol_dtor(ntsymstack_pop(stack));
      ntsymstack_push(stack, ntsymbol_ctor(TOKENID_IDENTIFIER, true));
      ntsymstack_push(stack, ntsymbol_ctor(TOKENID_KEYWORD_PACKAGE, true));
      return false;
   } else {
      return true;
   }
}

bool nonterminal_packages_derivation(ntsymstack_t* stack, int token_id) {
   if (token_id == TOKENID_KEYWORD_PACKAGE) {
      ntsymstack_push(stack, ntsymbol_ctor(TOKENID_NEWLINE, true));
      ntsymstack_push(stack, ntsymbol_ctor(NONTERMINAL_PACKAGE, false));
      return false;
   } else if (token_id == TOKENID_KEYWORD_FUNC || token_id == TOKENID_END_OF_FILE) {
      ntsymbol_dtor(ntsymstack_pop(stack));
      return false;
   } else {
      return true;
   }
}

bool nonterminal_program_derivation(ntsymstack_t* stack, int token_id) {
   if (token_id == TOKENID_KEYWORD_PACKAGE || token_id == TOKENID_KEYWORD_FUNC || token_id == TOKENID_END_OF_FILE) {
      ntsymbol_dtor(ntsymstack_pop(stack));
      ntsymstack_push(stack, ntsymbol_ctor(NONTERMINAL_FUNCTIONS, false));
      ntsymstack_push(stack, ntsymbol_ctor(NONTERMINAL_PACKAGES, false));
      return false;
   } else {
      return true;
   }
}

/**
 * Reduce function for precedence parser.
 * Determines if there is a rule to reduce nonterminals and terminals on the
 * stack.
 * Returns true if there isn't any or if any error occurs.
 */
bool reduce(ntposymstack_t* stack) {
   ntposymbol_t* ntpo_symbol = ntposymstack_pop(stack);

   if (ntpo_symbol->type == TERMINAL && (ntpo_symbol->id == TERMINAL_IDENTIFIER || ntpo_symbol->id == TERMINAL_NUM ||
         ntpo_symbol->id == TERMINAL_NUM_DECIMAL || ntpo_symbol->id == TERMINAL_BOOL_LITERAL ||
         ntpo_symbol->id == TERMINAL_STRING_LITERAL)) {
      ntposymbol_dtor(ntpo_symbol);
      ntpo_symbol = ntposymstack_pop(stack);
      if (ntpo_symbol->type == PRECEDENCE_OPERATOR) {
         ntposymbol_dtor(ntpo_symbol);
         ntposymstack_push(stack, ntposymbol_ctor(NONTERMINAL_EXPRESSION, NONTERMINAL));
      } else {
         ntposymbol_dtor(ntpo_symbol);
         return true;
      }
   } else if (ntpo_symbol->type == TERMINAL && ntpo_symbol->id == TERMINAL_RIGHT_PARENTHESES) {
      ntposymbol_dtor(ntpo_symbol);
      ntpo_symbol = ntposymstack_pop(stack);
      if (ntpo_symbol->type == NONTERMINAL && ntpo_symbol->id == NONTERMINAL_EXPRESSION) {
         ntposymbol_dtor(ntpo_symbol);
         ntpo_symbol = ntposymstack_pop(stack);
      } else {
         ntposymbol_dtor(ntpo_symbol);
         return true;
      }
      if (ntpo_symbol->type == TERMINAL && ntpo_symbol->id == TERMINAL_LEFT_PARENTHESES) {
         ntposymbol_dtor(ntpo_symbol);
         ntpo_symbol = ntposymstack_pop(stack);
      } else {
         ntposymbol_dtor(ntpo_symbol);
         return true;
      }
      if (ntpo_symbol->type == PRECEDENCE_OPERATOR) {
         ntposymbol_dtor(ntpo_symbol);
      } else {
         ntposymbol_dtor(ntpo_symbol);
         return true;
      }
      ntposymstack_push(stack, ntposymbol_ctor(NONTERMINAL_EXPRESSION, NONTERMINAL));
   } else if (ntpo_symbol->type == NONTERMINAL && ntpo_symbol->id == NONTERMINAL_EXPRESSION) {
      ntposymbol_dtor(ntpo_symbol);
      ntpo_symbol = ntposymstack_pop(stack);
      if (ntpo_symbol->type == TERMINAL && ntpo_symbol->id == TERMINAL_OPERATOR_NOT) {
         ntposymbol_dtor(ntpo_symbol);
         ntpo_symbol = ntposymstack_pop(stack);
         if (ntpo_symbol->type == PRECEDENCE_OPERATOR) {
            ntposymbol_dtor(ntpo_symbol);
            ntposymstack_push(stack, ntposymbol_ctor(NONTERMINAL_EXPRESSION, NONTERMINAL));
         } else {
            ntposymbol_dtor(ntpo_symbol);
            return true;
         }
      } else if (ntpo_symbol->type == TERMINAL && (ntpo_symbol->id == TERMINAL_OPERATOR_OR || 
                  ntpo_symbol->id == TERMINAL_OPERATOR_AND || ntpo_symbol->id == TERMINAL_OPERATOR_EQUALS || 
                  ntpo_symbol->id == TERMINAL_OPERATOR_NOT_EQUAL || ntpo_symbol->id == TERMINAL_OPERATOR_LESS || 
                  ntpo_symbol->id == TERMINAL_OPERATOR_LESS_OR_EQUAL || ntpo_symbol->id == TERMINAL_OPERATOR_GREATER || 
                  ntpo_symbol->id == TERMINAL_OPERATOR_GREATER_OR_EQUAL || ntpo_symbol->id == TERMINAL_OPERATOR_ADD || 
                  ntpo_symbol->id == TERMINAL_OPERATOR_SUB || ntpo_symbol->id == TERMINAL_OPERATOR_MUL || 
                  ntpo_symbol->id == TERMINAL_OPERATOR_DIV)) {
         ntposymbol_dtor(ntpo_symbol);
         ntpo_symbol = ntposymstack_pop(stack);
         if (ntpo_symbol->type == NONTERMINAL && ntpo_symbol->id == NONTERMINAL_EXPRESSION) {
            ntposymbol_dtor(ntpo_symbol);
            ntpo_symbol = ntposymstack_pop(stack);
         } else {
            ntposymbol_dtor(ntpo_symbol);
            return true;
         }
         if (ntpo_symbol->type == PRECEDENCE_OPERATOR) {
            ntposymbol_dtor(ntpo_symbol);
            ntposymstack_push(stack, ntposymbol_ctor(NONTERMINAL_EXPRESSION, NONTERMINAL));
         } else {
            ntposymbol_dtor(ntpo_symbol);
            return true;
         }
      } else {
         ntposymbol_dtor(ntpo_symbol);
         return true;
      }

   } else {
      return true;
   }
   return false;
}

/**
 * Shift function for precedence parser.
 * Adds an operator before the first terminal on the stack, pushes input
 * terminal onto stack and reads next token.
 */
void add_operator_and_shift(token_t** token, token_t** token_next, ntposymstack_t* stack, ntposymstack_t* help_stack,
                              int input_terminal_id, int stack_top_terminal_id) {
   while (stack_top_terminal_id != ntposymstack_top(stack)->id) {
      ntposymstack_push(help_stack, ntposymstack_pop(stack));
   }
   ntposymstack_push(stack, ntposymbol_ctor(PRECEDENCE_OPERATOR, PRECEDENCE_OPERATOR));
   while (ntposymstack_top(help_stack)->id != TERMINAL_END_OF_FILE) {
      ntposymstack_push(stack, ntposymstack_pop(help_stack));
   }

   ntposymstack_push(stack, ntposymbol_ctor(input_terminal_id, TERMINAL));
   token_dtor((*token));
   (*token) = (*token_next);
   if ((*token)->id != TOKENID_END_OF_FILE) {
      (*token_next) = get_next_token();
   }
   return;
}

/**
 * Shift function for precedence parser.
 * Pushes input terminal onto stack and reads next token.
 */
void shift(token_t** token, token_t** token_next, ntposymstack_t* stack, int input_terminal_id) {
   ntposymstack_push(stack, ntposymbol_ctor(input_terminal_id, TERMINAL));
   token_dtor((*token));
   (*token) = (*token_next);
   if ((*token)->id != TOKENID_END_OF_FILE) {
      (*token_next) = get_next_token();
   }
   return;
}

/**
 * Precedence parsing for expressions.
 * Returns true if any error occurs.
 */
bool precedence_parser(token_t** token, token_t** token_next) {
   ntposymstack_t* stack = ntposymstack_ctor();
   ntposymstack_t* help_stack = ntposymstack_ctor();
   int input_terminal_id;
   int stack_top_terminal_id;

   bool error = false;

   ntposymstack_push(stack, ntposymbol_ctor(TERMINAL_END_OF_FILE, TERMINAL));
   ntposymstack_push(help_stack, ntposymbol_ctor(TERMINAL_END_OF_FILE, TERMINAL));

   while (!error) {
      if ((*token)->id == TOKENID_NEWLINE || (*token)->id == TOKENID_COMMA || (*token)->id == TOKENID_LEFT_BRACKET ||
            (*token)->id == TOKENID_SEMICOLON) {
         if (ntposymstack_top_terminal(stack)->id == TERMINAL_END_OF_FILE) {
            break;
         } else {
            input_terminal_id = TERMINAL_END_OF_FILE;
         }
      } else {
         input_terminal_id = get_precedence_terminal_id((*token)->id);
      }

      stack_top_terminal_id = ntposymstack_top_terminal(stack)->id;

      switch (precedence_table[stack_top_terminal_id][input_terminal_id]) {
         case PT_E:
            shift(token, token_next, stack, input_terminal_id);
            break;
         case PT_L:
            add_operator_and_shift(token, token_next, stack, help_stack, input_terminal_id, stack_top_terminal_id);
            break;
         case PT_G:
            error = reduce(stack);
            break;
         case PT_N:
         default:
            error = true;
            break;
      }
   }

   ntposymstack_dtor(help_stack);
   ntposymstack_dtor(stack);

   return error;
}

/**
 * Calls nonterminal derivation by given nonterminal ID.
 * Returns true if any error occurs.
 */
bool find_derivation(int stack_top_id, ntsymstack_t* stack, token_t** token, token_t** token_next) {
   switch (stack_top_id) {
      case NONTERMINAL_PROGRAM:
         return nonterminal_program_derivation(stack, (*token)->id);
      case NONTERMINAL_PACKAGES:
         return nonterminal_packages_derivation(stack, (*token)->id);
      case NONTERMINAL_PACKAGE:
         return nonterminal_package_derivation(stack, (*token)->id);
      case NONTERMINAL_FUNCTIONS:
         return nonterminal_functions_derivation(stack, (*token)->id);
      case NONTERMINAL_FUNCTION:
         return nonterminal_function_derivation(stack, (*token)->id);
      case NONTERMINAL_PARAMETERS:
         return nonterminal_parameters_derivation(stack, (*token)->id);
      case NONTERMINAL_PARAMETER:
         return nonterminal_parameter_derivation(stack, (*token)->id);
      case NONTERMINAL_PARAMETER_NEXT:
         return nonterminal_parameter_next_derivation(stack, (*token)->id);
      case NONTERMINAL_RETURN_TYPES:
         return nonterminal_return_types_derivation(stack, (*token)->id);
      case NONTERMINAL_TYPES:
         return nonterminal_types_derivation(stack, (*token)->id);
      case NONTERMINAL_TYPE:
         return nonterminal_type_derivation(stack, (*token)->id);
      case NONTERMINAL_TYPE_NEXT:
         return nonterminal_type_next_derivation(stack, (*token)->id);
      case NONTERMINAL_RETURN:
         return nonterminal_return_derivation(stack, (*token)->id);
      case NONTERMINAL_DEFINITION:
         return nonterminal_definition_derivation(stack, (*token)->id);
      case NONTERMINAL_FOR_DEFINITION:
         return nonterminal_for_definition_derivation(stack, (*token)->id);
      case NONTERMINAL_ASSIGNMENT:
         return nonterminal_assignment_derivation(stack, (*token)->id);
      case NONTERMINAL_FOR_ASSIGNMENT:
         return nonterminal_for_assignment_derivation(stack, (*token)->id);
      case NONTERMINAL_ASSIGNMENT_RIGHT:
         return nonterminal_assignment_right_derivation(stack, (*token)->id, (*token_next)->id);
      case NONTERMINAL_FOR_ASSIGNMENT_RIGHT:
         return nonterminal_for_assignment_right_derivation(stack, (*token)->id, (*token_next)->id);
      case NONTERMINAL_ID_NEXT:
         return nonterminal_id_next_derivation(stack, (*token)->id);
      case NONTERMINAL_COMMANDS:
         return nonterminal_commands_derivation(stack, (*token)->id);
      case NONTERMINAL_COMMAND:
         return nonterminal_command_derivation(stack, (*token)->id, (*token_next)->id);
      case NONTERMINAL_IF:
         return nonterminal_if_derivation(stack, (*token)->id);
      case NONTERMINAL_ELSE_IF:
         return nonterminal_else_if_derivation(stack, (*token)->id, (*token_next)->id);
      case NONTERMINAL_FOR:
         return nonterminal_for_derivation(stack, (*token)->id);
      case NONTERMINAL_CALL:
         return nonterminal_call_derivation(stack, (*token)->id);
      case NONTERMINAL_TERMS:
         return nonterminal_terms_derivation(stack, (*token)->id);
      case NONTERMINAL_TERM:
         return nonterminal_term_derivation(stack, (*token)->id);
      case NONTERMINAL_TERM_NEXT:
         return nonterminal_term_next_derivation(stack, (*token)->id);
      case NONTERMINAL_EXPRESSIONS:
         return nonterminal_expressions_derivation(stack, (*token)->id);
      case NONTERMINAL_EXPRESSION:
         ntsymbol_dtor(ntsymstack_pop(stack));
         return precedence_parser(token, token_next);
      case NONTERMINAL_EXPRESSION_NEXT:
         return nonterminal_expression_next_derivation(stack, (*token)->id);
      default:
         return true;
   }
}

/**
 * Predictive syntax analysis.
 */
void parse() {
   ntsymstack_t* stack = ntsymstack_ctor();
   token_t* token = get_next_token();
   token_t* token_next = token;
   ntsymbol_t* stack_top;
   bool error = false;
   bool eol_flag = true;

   ntsymstack_push(stack, ntsymbol_ctor(TOKENID_END_OF_FILE, true));
   ntsymstack_push(stack, ntsymbol_ctor(NONTERMINAL_PROGRAM, false));

   if (token->id != TOKENID_END_OF_FILE) {
      token_next = get_next_token();
   }

   while (!error && ntsymstack_get_length(stack) != 0) {
      stack_top = ntsymstack_top(stack);
      if (eol_flag) {
         if (token->id == TOKENID_NEWLINE) {
            token_dtor(token);
            token = token_next;
            if (token->id != TOKENID_END_OF_FILE) {
               token_next = get_next_token();
            }
         } else {
            eol_flag = false;
         }
      } else if (stack_top->is_terminal && stack_top->id == TOKENID_END_OF_FILE) {
         if (token->id == TOKENID_END_OF_FILE) {
            ntsymbol_dtor(ntsymstack_pop(stack));
            token_dtor(token);
         } else {
            error = true;
         }
      } else if (stack_top->is_terminal) {
         if ((tokenid_e)stack_top->id == token->id) {
            eol_flag = (token->id == TOKENID_NEWLINE) ? true : false;
            token_dtor(token);
            ntsymbol_dtor(ntsymstack_pop(stack));
            token = token_next;
            if (token->id != TOKENID_END_OF_FILE) {
               token_next = get_next_token();
            }
         } else {
            error = true;
         }
      } else {
         error = find_derivation(stack_top->id, stack, &token, &token_next);
      }
   }
   ntsymstack_dtor(stack);

   if (error) {
      token_dtor(token);
      token_dtor(token_next);
      exit(ERRCODE_SYNTAX_ERROR);
   }

   return;
}
