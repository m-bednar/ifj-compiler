/**
 * IFJ20 Compiler
 * @file parser.c
 * @authors Daniel Havranek (xhavra18)
 */

#include "parser.h"

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

    ntsymbol_t *end_of_file = ntsymbol_ctor(TOKENID_END_OF_FILE, true);
    ntsymbol_t *program = ntsymbol_ctor(NONTERMINAL_PROGRAM, false);

    ntsymstack_push(stack, end_of_file);
    ntsymstack_push(stack, program);

    token_t *token;
    ntsymbol_t *stack_top;
    bool error = false;

    while(!error) {
        
        token = get_next_token();
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
            if(stack_top->id == token->id) {
                ntsymbol_dtor(ntsymstack_pop(stack));
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
