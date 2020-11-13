/**
 * IFJ20 Compiler
 * @file parser.c
 * @authors Daniel Havranek (xhavra18)
 */

#include "parser.h"

bool nonterminal_ids_derivation(ntsymstack_t *stack, int token_id) {
    if(token_id == TOKENID_IDENTIFIER) {
        ntsymbol_dtor(ntsymstack_pop(stack));
        ntsymstack_push(stack, ntsymbol_ctor(NONTERMINAL_ID_NEXT, false));
        ntsymstack_push(stack, ntsymbol_ctor(TOKENID_IDENTIFIER, true));
        return false;
    }
    else if(token_id == TOKENID_RIGHT_PARENTHESES) {
        ntsymbol_dtor(ntsymstack_pop(stack));
        return false;
    }
    else {
        return true;
    }
}

bool nonterminal_call_derivation(ntsymstack_t *stack, int token_id) {
    if(token_id == TOKENID_IDENTIFIER) {
        ntsymbol_dtor(ntsymstack_pop(stack));
        ntsymstack_push(stack, ntsymbol_ctor(TOKENID_RIGHT_PARENTHESES, true));
        ntsymstack_push(stack, ntsymbol_ctor(NONTERMINAL_IDS, false));
        ntsymstack_push(stack, ntsymbol_ctor(TOKENID_LEFT_PARENTHESES, true));
        ntsymstack_push(stack, ntsymbol_ctor(TOKENID_IDENTIFIER, true));
        return false;
    }
    else {
        return true;
    }
}

bool nonterminal_for_derivation(ntsymstack_t *stack, int token_id) {
    if(token_id == TOKENID_KEYWORD_FOR) {
        ntsymbol_dtor(ntsymstack_pop(stack));
        ntsymstack_push(stack, ntsymbol_ctor(TOKENID_RIGHT_BRACKET, true));
        ntsymstack_push(stack, ntsymbol_ctor(NONTERMINAL_COMMANDS, false));
        ntsymstack_push(stack, ntsymbol_ctor(TOKENID_LEFT_BRACKET, true));
        ntsymstack_push(stack, ntsymbol_ctor(NONTERMINAL_FOR_ASSIGNMENT, false));
        ntsymstack_push(stack, ntsymbol_ctor(TOKENID_SEMICOLON, true));
        ntsymstack_push(stack, ntsymbol_ctor(NONTERMINAL_EXPRESSION, false));
        ntsymstack_push(stack, ntsymbol_ctor(TOKENID_SEMICOLON, true));
        ntsymstack_push(stack, ntsymbol_ctor(NONTERMINAL_FOR_DEFINITION, false));
        ntsymstack_push(stack, ntsymbol_ctor(TOKENID_KEYWORD_FOR, true));
        return false;
    }
    else {
        return true;
    }
}

bool nonterminal_else_derivation(ntsymstack_t *stack, int token_id) {
    if(token_id == TOKENID_IDENTIFIER || token_id == TOKENID_KEYWORD_IF || token_id == TOKENID_KEYWORD_FOR || 
    token_id == TOKENID_KEYWORD_RETURN || token_id == TOKENID_RIGHT_BRACKET) {
        ntsymbol_dtor(ntsymstack_pop(stack));
        return false;
    }
    else if(token_id == TOKENID_KEYWORD_ELSE) {
        ntsymbol_dtor(ntsymstack_pop(stack));
        ntsymstack_push(stack, ntsymbol_ctor(TOKENID_RIGHT_BRACKET, true));
        ntsymstack_push(stack, ntsymbol_ctor(NONTERMINAL_COMMANDS, false));
        ntsymstack_push(stack, ntsymbol_ctor(TOKENID_LEFT_BRACKET, true));
        ntsymstack_push(stack, ntsymbol_ctor(TOKENID_KEYWORD_ELSE, true));
        return false;
    }
    else {
        return true;
    }
}

bool nonterminal_if_derivation(ntsymstack_t *stack, int token_id) {
    if(token_id == TOKENID_KEYWORD_IF) {
        ntsymbol_dtor(ntsymstack_pop(stack));
        ntsymstack_push(stack, ntsymbol_ctor(NONTERMINAL_ELSE, false));
        ntsymstack_push(stack, ntsymbol_ctor(TOKENID_RIGHT_BRACKET, true));
        ntsymstack_push(stack, ntsymbol_ctor(NONTERMINAL_COMMANDS, false));
        ntsymstack_push(stack, ntsymbol_ctor(TOKENID_LEFT_BRACKET, true));
        ntsymstack_push(stack, ntsymbol_ctor(NONTERMINAL_EXPRESSION, false));
        ntsymstack_push(stack, ntsymbol_ctor(TOKENID_KEYWORD_IF, true));
        return false;
    }
    else {
        return true;
    }
}

bool nonterminal_command_derivation(ntsymstack_t *stack, int token_id, int token_next_id) {
    if(token_id == TOKENID_IDENTIFIER) {
        if(token_next_id == TOKENID_OPERATOR_DECLARE) {
            ntsymbol_dtor(ntsymstack_pop(stack));
            ntsymstack_push(stack, ntsymbol_ctor(NONTERMINAL_DEFINITION, false));
            return false;
        }
        else if(token_next_id == TOKENID_COMMA || token_next_id == TOKENID_OPERATOR_ASSIGN) {
            ntsymbol_dtor(ntsymstack_pop(stack));
            ntsymstack_push(stack, ntsymbol_ctor(NONTERMINAL_ASSIGNMENT, false));
            return false;
        }
        else if(token_next_id == TOKENID_LEFT_PARENTHESES) {
            ntsymbol_dtor(ntsymstack_pop(stack));
            ntsymstack_push(stack, ntsymbol_ctor(NONTERMINAL_CALL, false));
            return false;
        }
        else {
            return true;
        }
    }
    else if(token_id == TOKENID_KEYWORD_IF) {
        ntsymbol_dtor(ntsymstack_pop(stack));
        ntsymstack_push(stack, ntsymbol_ctor(NONTERMINAL_IF, false));
        return false;
    }
    else if(token_id == TOKENID_KEYWORD_FOR) {
        ntsymbol_dtor(ntsymstack_pop(stack));
        ntsymstack_push(stack, ntsymbol_ctor(NONTERMINAL_FOR, false));
        return false;
    }
    else {
        return true;
    }
}

bool nonterminal_commands_derivation(ntsymstack_t *stack, int token_id) {
    if(token_id == TOKENID_IDENTIFIER || token_id == TOKENID_KEYWORD_IF || token_id == TOKENID_KEYWORD_FOR) {
        ntsymstack_push(stack, ntsymbol_ctor(NONTERMINAL_COMMAND, false));
        return false;
    }
    else if(token_id == TOKENID_KEYWORD_RETURN || token_id == TOKENID_RIGHT_BRACKET) {
        ntsymbol_dtor(ntsymstack_pop(stack));
        return false;
    }
    else {
        return true;
    }
}

bool nonterminal_id_next_derivation(ntsymstack_t *stack, int token_id) {
    if(token_id == TOKENID_OPERATOR_ASSIGN || token_id == TOKENID_RIGHT_PARENTHESES) {
        ntsymbol_dtor(ntsymstack_pop(stack));
        return false;
    }
    else if(token_id == TOKENID_COMMA) {
        ntsymstack_push(stack, ntsymbol_ctor(TOKENID_IDENTIFIER, true));
        ntsymstack_push(stack, ntsymbol_ctor(TOKENID_COMMA, true));
        return false;
    }
    else {
        return true;
    }
}

bool nonterminal_for_assignment_right_derivation(ntsymstack_t *stack, int token_id, int token_next_id) {
    if(token_id == TOKENID_IDENTIFIER) {
        if(token_next_id == TOKENID_LEFT_PARENTHESES) {
            ntsymbol_dtor(ntsymstack_pop(stack));
            ntsymstack_push(stack, ntsymbol_ctor(NONTERMINAL_CALL, false));
            return false;
        }
        else if(token_next_id == TOKENID_LEFT_BRACKET || token_next_id == TOKENID_COMMA || token_next_id == TOKENID_OPERATOR_OR || 
        token_next_id == TOKENID_OPERATOR_AND || token_next_id == TOKENID_OPERATOR_NOT_EQUAL || token_next_id == TOKENID_OPERATOR_EQUALS || 
        token_next_id == TOKENID_OPERATOR_LESS || token_next_id == TOKENID_OPERATOR_LESS_OR_EQUAL || token_next_id == TOKENID_OPERATOR_GREATER || 
        token_next_id == TOKENID_OPERATOR_GREATER_OR_EQUAL || token_next_id == TOKENID_OPERATOR_ADD || token_next_id == TOKENID_OPERATOR_SUB || 
        token_next_id == TOKENID_OPERATOR_MUL || token_next_id == TOKENID_OPERATOR_DIV) {
            ntsymbol_dtor(ntsymstack_pop(stack));
            ntsymstack_push(stack, ntsymbol_ctor(NONTERMINAL_EXPRESSION_NEXT, false));
            ntsymstack_push(stack, ntsymbol_ctor(NONTERMINAL_EXPRESSION, false));
            // switch to expression analysis
            return false;
        }
        else {
            return true;
        }
    }
    else if(token_id == TOKENID_LEFT_PARENTHESES || token_id == TOKENID_OPERATOR_NOT || token_id == TOKENID_NUM || 
    token_id == TOKENID_NUM_DECIMAL || token_id == TOKENID_STRING_LITERAL || token_id == TOKENID_BOOL_LITERAL) {
        ntsymbol_dtor(ntsymstack_pop(stack));
        ntsymstack_push(stack, ntsymbol_ctor(NONTERMINAL_EXPRESSION_NEXT, false));
        ntsymstack_push(stack, ntsymbol_ctor(NONTERMINAL_EXPRESSION, false));
        // switch to expression analysis
        return false;
    }
    else {
        return true;
    }
}

bool nonterminal_assignment_right_derivation(ntsymstack_t *stack, int token_id, int token_next_id) {
    if(token_id == TOKENID_IDENTIFIER) {
        if(token_next_id == TOKENID_LEFT_PARENTHESES) {
            ntsymbol_dtor(ntsymstack_pop(stack));
            ntsymstack_push(stack, ntsymbol_ctor(NONTERMINAL_CALL, false));
            return false;
        }
        else if(token_next_id == TOKENID_NEWLINE || token_next_id == TOKENID_COMMA || token_next_id == TOKENID_OPERATOR_OR || 
        token_next_id == TOKENID_OPERATOR_AND || token_next_id == TOKENID_OPERATOR_NOT_EQUAL || token_next_id == TOKENID_OPERATOR_EQUALS || 
        token_next_id == TOKENID_OPERATOR_LESS || token_next_id == TOKENID_OPERATOR_LESS_OR_EQUAL || token_next_id == TOKENID_OPERATOR_GREATER || 
        token_next_id == TOKENID_OPERATOR_GREATER_OR_EQUAL || token_next_id == TOKENID_OPERATOR_ADD || token_next_id == TOKENID_OPERATOR_SUB || 
        token_next_id == TOKENID_OPERATOR_MUL || token_next_id == TOKENID_OPERATOR_DIV) {
            ntsymbol_dtor(ntsymstack_pop(stack));
            ntsymstack_push(stack, ntsymbol_ctor(NONTERMINAL_EXPRESSION_NEXT, false));
            ntsymstack_push(stack, ntsymbol_ctor(NONTERMINAL_EXPRESSION, false));
            // switch to expression analysis
            return false;
        }
        else {
            return true;
        }
    }
    else if(token_id == TOKENID_LEFT_PARENTHESES || token_id == TOKENID_OPERATOR_NOT || token_id == TOKENID_NUM || 
    token_id == TOKENID_NUM_DECIMAL || token_id == TOKENID_STRING_LITERAL || token_id == TOKENID_BOOL_LITERAL) {
        ntsymbol_dtor(ntsymstack_pop(stack));
        ntsymstack_push(stack, ntsymbol_ctor(NONTERMINAL_EXPRESSION_NEXT, false));
        ntsymstack_push(stack, ntsymbol_ctor(NONTERMINAL_EXPRESSION, false));
        // switch to expression analysis
        return false;
    }
    else {
        return true;
    }
}

bool nonterminal_for_assignment_derivation(ntsymstack_t *stack, int token_id) {
    if(token_id == TOKENID_IDENTIFIER) {
        ntsymbol_dtor(ntsymstack_pop(stack));
        ntsymstack_push(stack, ntsymbol_ctor(NONTERMINAL_FOR_ASSIGNMENT_RIGHT, false));
        ntsymstack_push(stack, ntsymbol_ctor(TOKENID_OPERATOR_ASSIGN, true));
        ntsymstack_push(stack, ntsymbol_ctor(NONTERMINAL_ID_NEXT, false));
        ntsymstack_push(stack, ntsymbol_ctor(TOKENID_IDENTIFIER, true));
        return false;
    }
    else if(token_id == TOKENID_LEFT_BRACKET) {
        ntsymbol_dtor(ntsymstack_pop(stack));
        return false;
    }
    else {
        return true;
    }
}

bool nonterminal_assignment_derivation(ntsymstack_t *stack, int token_id) {
    if(token_id == TOKENID_IDENTIFIER) {
        ntsymbol_dtor(ntsymstack_pop(stack));
        ntsymstack_push(stack, ntsymbol_ctor(NONTERMINAL_ASSIGNMENT_RIGHT, false));
        ntsymstack_push(stack, ntsymbol_ctor(TOKENID_OPERATOR_ASSIGN, true));
        ntsymstack_push(stack, ntsymbol_ctor(NONTERMINAL_ID_NEXT, false));
        ntsymstack_push(stack, ntsymbol_ctor(TOKENID_IDENTIFIER, true));
        return false;
    }
    else {
        return true;
    }
}

bool nonterminal_for_definition_derivation(ntsymstack_t *stack, int token_id) {
    if(token_id == TOKENID_IDENTIFIER) {
        ntsymbol_dtor(ntsymstack_pop(stack));
        ntsymstack_push(stack, ntsymbol_ctor(NONTERMINAL_EXPRESSION, false));
        ntsymstack_push(stack, ntsymbol_ctor(TOKENID_OPERATOR_DECLARE, true));
        ntsymstack_push(stack, ntsymbol_ctor(TOKENID_IDENTIFIER, true));
        return false;
    }
    else if(token_id == TOKENID_SEMICOLON) {
        ntsymbol_dtor(ntsymstack_pop(stack));
        return false;
    }
    else {
        return true;
    }
}

bool nonterminal_definition_derivation(ntsymstack_t *stack, int token_id) {
    if(token_id == TOKENID_IDENTIFIER) {
        ntsymbol_dtor(ntsymstack_pop(stack));
        ntsymstack_push(stack, ntsymbol_ctor(NONTERMINAL_EXPRESSION, false));
        ntsymstack_push(stack, ntsymbol_ctor(TOKENID_OPERATOR_DECLARE, true));
        ntsymstack_push(stack, ntsymbol_ctor(TOKENID_IDENTIFIER, true));
        return false;
    }
    else {
        return true;
    }
}

bool nonterminal_return_derivation(ntsymstack_t *stack, int token_id) {
    if(token_id == TOKENID_KEYWORD_RETURN) {
        ntsymbol_dtor(ntsymstack_pop(stack));
        ntsymstack_push(stack, ntsymbol_ctor(NONTERMINAL_EXPRESSIONS, false));
        ntsymstack_push(stack, ntsymbol_ctor(TOKENID_KEYWORD_RETURN, true));
        return false;
    }
    else if(token_id == TOKENID_RIGHT_BRACKET) {
        ntsymbol_dtor(ntsymstack_pop(stack));
        return false;
    }
    else {
        return true;
    }
}

bool nonterminal_type_next_derivation(ntsymstack_t *stack, int token_id) {
    if(token_id == TOKENID_RIGHT_PARENTHESES) {
        ntsymbol_dtor(ntsymstack_pop(stack));
        return false;
    }
    else if(token_id == TOKENID_COMMA) {
        ntsymstack_push(stack, ntsymbol_ctor(NONTERMINAL_TYPE, false));
        ntsymstack_push(stack, ntsymbol_ctor(TOKENID_COMMA, true));
        return false;
    }
    else {
        return true;
    }
}

bool nonterminal_types_derivation(ntsymstack_t *stack, int token_id) {
    if(token_id == TOKENID_KEYWORD_INT || token_id == TOKENID_KEYWORD_FLOAT64 || token_id == TOKENID_KEYWORD_STRING ||
    token_id == TOKENID_KEYWORD_BOOL) {
        ntsymbol_dtor(ntsymstack_pop(stack));
        ntsymstack_push(stack, ntsymbol_ctor(NONTERMINAL_TYPE_NEXT, false));
        ntsymstack_push(stack, ntsymbol_ctor(NONTERMINAL_TYPE, false));
        return false;
    }
    else if(token_id == TOKENID_RIGHT_PARENTHESES) {
        ntsymbol_dtor(ntsymstack_pop(stack));
        return false;
    }
    else {
        return true;
    }
}

bool nonterminal_return_types_derivation(ntsymstack_t *stack, int token_id) {
    if(token_id == TOKENID_LEFT_PARENTHESES) {
        ntsymbol_dtor(ntsymstack_pop(stack));
        ntsymstack_push(stack, ntsymbol_ctor(TOKENID_RIGHT_PARENTHESES, true));
        ntsymstack_push(stack, ntsymbol_ctor(NONTERMINAL_TYPES, false));
        ntsymstack_push(stack, ntsymbol_ctor(TOKENID_LEFT_PARENTHESES, true));
        return false;
    }
    else if(token_id == TOKENID_LEFT_BRACKET) {
        ntsymbol_dtor(ntsymstack_pop(stack));
        return false;
    }
    else {
        return true;
    }
}

bool nonterminal_type_derivation(ntsymstack_t *stack, int token_id) {
    if(token_id == TOKENID_KEYWORD_INT) {
        ntsymbol_dtor(ntsymstack_pop(stack));
        ntsymstack_push(stack, ntsymbol_ctor(TOKENID_KEYWORD_INT, true));
        return false;
    }
    else if(token_id == TOKENID_KEYWORD_FLOAT64) {
        ntsymbol_dtor(ntsymstack_pop(stack));
        ntsymstack_push(stack, ntsymbol_ctor(TOKENID_KEYWORD_FLOAT64, true));
        return false;
    }
    else if(token_id == TOKENID_KEYWORD_STRING) {
        ntsymbol_dtor(ntsymstack_pop(stack));
        ntsymstack_push(stack, ntsymbol_ctor(TOKENID_KEYWORD_STRING, true));
        return false;
    }
    else if(token_id == TOKENID_KEYWORD_BOOL) {
        ntsymbol_dtor(ntsymstack_pop(stack));
        ntsymstack_push(stack, ntsymbol_ctor(TOKENID_KEYWORD_BOOL, true));
        return false;
    }
    else {
        return true;
    }
}

bool nonterminal_parameter_next_derivation(ntsymstack_t *stack, int token_id) {
    if(token_id == TOKENID_RIGHT_PARENTHESES) {
        ntsymbol_dtor(ntsymstack_pop(stack));
        return false;
    }
    else if(token_id == TOKENID_COMMA) {
        ntsymstack_push(stack, ntsymbol_ctor(NONTERMINAL_PARAMETER, false));
        ntsymstack_push(stack, ntsymbol_ctor(TOKENID_COMMA, true));
        return false;
    }
    else {
        return true;
    }
}

bool nonterminal_parameter_derivation(ntsymstack_t *stack, int token_id) {
    if(token_id == TOKENID_IDENTIFIER) {
        ntsymbol_dtor(ntsymstack_pop(stack));
        ntsymstack_push(stack, ntsymbol_ctor(NONTERMINAL_TYPE, false));
        ntsymstack_push(stack, ntsymbol_ctor(TOKENID_IDENTIFIER, true));
        return false;
    }
    else {
        return true;
    }
}

bool nonterminal_parameters_derivation(ntsymstack_t *stack, int token_id) {
    if(token_id == TOKENID_IDENTIFIER) {
        ntsymbol_dtor(ntsymstack_pop(stack));
        ntsymstack_push(stack, ntsymbol_ctor(NONTERMINAL_PARAMETER_NEXT, false));
        ntsymstack_push(stack, ntsymbol_ctor(NONTERMINAL_PARAMETER, false));
        return false;
    }
    else if(token_id == TOKENID_RIGHT_PARENTHESES) {
        ntsymbol_dtor(ntsymstack_pop(stack));
        return false;
    }
    else {
        return true;
    }
}

bool nonterminal_function_derivation(ntsymstack_t *stack, int token_id) {
    if(token_id == TOKENID_KEYWORD_FUNC) {
        ntsymbol_dtor(ntsymstack_pop(stack));
        ntsymstack_push(stack, ntsymbol_ctor(TOKENID_RIGHT_BRACKET, true));
        ntsymstack_push(stack, ntsymbol_ctor(NONTERMINAL_RETURN, false));
        ntsymstack_push(stack, ntsymbol_ctor(NONTERMINAL_COMMANDS, false));
        ntsymstack_push(stack, ntsymbol_ctor(TOKENID_LEFT_BRACKET, true));
        ntsymstack_push(stack, ntsymbol_ctor(NONTERMINAL_RETURN_TYPES, false));
        ntsymstack_push(stack, ntsymbol_ctor(TOKENID_RIGHT_PARENTHESES, true));
        ntsymstack_push(stack, ntsymbol_ctor(NONTERMINAL_PARAMETERS, false));
        ntsymstack_push(stack, ntsymbol_ctor(TOKENID_LEFT_PARENTHESES, true));
        ntsymstack_push(stack, ntsymbol_ctor(TOKENID_IDENTIFIER, true));
        ntsymstack_push(stack, ntsymbol_ctor(TOKENID_KEYWORD_FUNC, true));
        return false;
    }
    else {
        return true;
    }
}

bool nonterminal_functions_derivation(ntsymstack_t *stack, int token_id) {
    if(token_id == TOKENID_KEYWORD_FUNC) {
        ntsymstack_push(stack, ntsymbol_ctor(NONTERMINAL_FUNCTION, false));
        return false;
    }
    else if(token_id == TOKENID_END_OF_FILE) {
        ntsymbol_dtor(ntsymstack_pop(stack));
        return false;
    }
    else {
        return true;
    }
}

bool nonterminal_package_derivation(ntsymstack_t *stack, int token_id) {
    if(token_id == TOKENID_KEYWORD_PACKAGE) {
        ntsymbol_dtor(ntsymstack_pop(stack));
        ntsymstack_push(stack, ntsymbol_ctor(TOKENID_IDENTIFIER, true));
        ntsymstack_push(stack, ntsymbol_ctor(TOKENID_KEYWORD_PACKAGE, true));
        return false;
    }
    else {
        return true;
    }
}

bool nonterminal_packages_derivation(ntsymstack_t *stack, int token_id) {
    if(token_id == TOKENID_KEYWORD_PACKAGE) {
        ntsymstack_push(stack, ntsymbol_ctor(NONTERMINAL_PACKAGE, false));
        return false;
    }
    else if(token_id == TOKENID_KEYWORD_FUNC || token_id == TOKENID_END_OF_FILE) {
        ntsymbol_dtor(ntsymstack_pop(stack));
        return false;
    }
    else {
        return true;
    }
}

bool nonterminal_program_derivation(ntsymstack_t *stack, int token_id) {
    if(token_id == TOKENID_KEYWORD_PACKAGE || token_id == TOKENID_KEYWORD_FUNC || token_id == TOKENID_END_OF_FILE) {
        ntsymbol_dtor(ntsymstack_pop(stack));
        ntsymstack_push(stack, ntsymbol_ctor(NONTERMINAL_FUNCTIONS, false));
        ntsymstack_push(stack, ntsymbol_ctor(NONTERMINAL_PACKAGES, false));
        return false;
    }
    else {
        return true;
    }
}

void parse() {
    ntsymstack_t *stack = ntsymstack_ctor();
    token_t *token = get_next_token();
    token_t *token_next = token;
    ntsymbol_t *stack_top;

    bool error = false;

    ntsymstack_push(stack, ntsymbol_ctor(TOKENID_END_OF_FILE, true));
    ntsymstack_push(stack, ntsymbol_ctor(NONTERMINAL_PROGRAM, false));

    if(token->id != TOKENID_END_OF_FILE) {
        token_next = get_next_token();
    }

    while(!error) {
        
        stack_top = ntsymstack_top(stack);

        if(stack_top->is_terminal && stack_top->id == TOKENID_END_OF_FILE) {
            if(token->id == TOKENID_END_OF_FILE) {
                ntsymbol_dtor(ntsymstack_pop(stack));
                break;
            }
            else {
                error = true;
            }
        }
        else if(stack_top->is_terminal) {
            if((tokenid_e)stack_top->id == token->id) {
                ntsymbol_dtor(ntsymstack_pop(stack));
                token = token_next;
                if(token->id != TOKENID_END_OF_FILE) {
                    token_next = get_next_token();
                }
            }
            else {
                error = true;
            }
        }
        else {
            switch(stack_top->id) {
                case NONTERMINAL_PROGRAM:
                    error = nonterminal_program_derivation(stack, token->id);
                    break;
                case NONTERMINAL_PACKAGES:
                    error = nonterminal_packages_derivation(stack, token->id);
                    break;
                case NONTERMINAL_PACKAGE:
                    error = nonterminal_package_derivation(stack, token->id);
                    break;
                case NONTERMINAL_FUNCTIONS:
                    error = nonterminal_functions_derivation(stack, token->id);
                    break;
                case NONTERMINAL_FUNCTION:
                    error = nonterminal_function_derivation(stack, token->id);
                    break;
                case NONTERMINAL_PARAMETERS:
                
                    break;
                case NONTERMINAL_PARAMETER:
                
                    break;
                case NONTERMINAL_PARAMETER_NEXT:
                
                    break;
                case NONTERMINAL_RETURN_TYPES:
                
                    break;
                case NONTERMINAL_TYPES:
                
                    break;
                case NONTERMINAL_TYPE:
                
                    break;
                case NONTERMINAL_TYPE_NEXT:
                
                    break;
                case NONTERMINAL_RETURN:
                
                    break;
                case NONTERMINAL_DEFINITION:
                
                    break;
                case NONTERMINAL_FOR_DEFINITION:
                
                    break;
                case NONTERMINAL_ASSIGNMENT:
                
                    break;
                case NONTERMINAL_FOR_ASSIGNMENT:
                
                    break;
                case NONTERMINAL_ASSIGNMENT_RIGHT:
                
                    break;
                case NONTERMINAL_FOR_ASSIGNMENT_RIGHT:
                
                    break;
                case NONTERMINAL_ID_NEXT:
                
                    break;
                case NONTERMINAL_COMMANDS:
                
                    break;
                case NONTERMINAL_COMMAND:
                
                    break;
                case NONTERMINAL_IF:
                
                    break;
                case NONTERMINAL_ELSE:
                
                    break;
                case NONTERMINAL_FOR:
                
                    break;
                case NONTERMINAL_CALL:
                
                    break;
                case NONTERMINAL_IDS:
                
                    break;
                case NONTERMINAL_EXPRESSIONS:
                
                    break;
                case NONTERMINAL_EXPRESSION:
                
                    break;
                case NONTERMINAL_EXPRESSION_NEXT:
                
                    break;
                default:
                    error = true;
                    break;
            }
        }
    }

    ntsymstack_dtor(stack);

    if(error) {
        exit(ERRCODE_SYNTAX_ERROR);
    }
   
    return;
}
