/**
 * IFJ20 Compiler
 * @file parser.c
 * @authors Daniel Havranek (xhavra18)
 */

#include "parser.h"

#define EOL_FLAG true

void print_nonterminal(int nonterminal_id) {
    switch(nonterminal_id) {
        case NONTERMINAL_PROGRAM:
            printf("<program>");
            break;
        case NONTERMINAL_PACKAGES:
            printf("<packages>");
            break;
        case NONTERMINAL_PACKAGE:
            printf("<package>");
            break;
        case NONTERMINAL_FUNCTIONS:
            printf("<functions>");
            break;
        case NONTERMINAL_FUNCTION:
            printf("<function>");
            break;
        case NONTERMINAL_PARAMETERS:
            printf("<parameters>");
            break;
        case NONTERMINAL_PARAMETER:
            printf("<parameter>");
            break;
        case NONTERMINAL_PARAMETER_NEXT:
            printf("<parameter_next>");
            break;
        case NONTERMINAL_RETURN_TYPES:
            printf("<return_types>");
            break;
        case NONTERMINAL_TYPES:
            printf("<types>");
            break;
        case NONTERMINAL_TYPE:
            printf("<type>");
            break;
        case NONTERMINAL_TYPE_NEXT:
            printf("<type_next>");
            break;
        case NONTERMINAL_RETURN:
            printf("<return>");
            break;
        case NONTERMINAL_DEFINITION:
            printf("<definition>");
            break;
        case NONTERMINAL_FOR_DEFINITION:
            printf("<for_definition>");
            break;
        case NONTERMINAL_ASSIGNMENT:
            printf("<assignment>");
            break;
        case NONTERMINAL_FOR_ASSIGNMENT:
            printf("<for_assignment>");
            break;
        case NONTERMINAL_ASSIGNMENT_RIGHT:
            printf("<assignment_right>");
            break;
        case NONTERMINAL_FOR_ASSIGNMENT_RIGHT:
            printf("<for_assignment_right>");
            break;
        case NONTERMINAL_ID_NEXT:
            printf("<id_next>");
            break;
        case NONTERMINAL_COMMANDS:
            printf("<commands>");
            break;
        case NONTERMINAL_COMMAND:
            printf("<command>");
            break;
        case NONTERMINAL_IF:
            printf("<if>");
            break;
        case NONTERMINAL_ELSE:
            printf("<else>");
            break;
        case NONTERMINAL_FOR:
            printf("<for>");
            break;
        case NONTERMINAL_CALL:
            printf("<call>");
            break;
        case NONTERMINAL_IDS:
            printf("<ids>");
            break;
        case NONTERMINAL_EXPRESSIONS:
            printf("<expressions>");
            break;
        case NONTERMINAL_EXPRESSION:
            printf("<expression>");
            break;
        case NONTERMINAL_EXPRESSION_NEXT:
            printf("<expression_next>");
            break;
        default:
            printf("not nonterminal");
            break;
    }
    return;
}

void print_terminal(int terminal_id) {
    switch(terminal_id) {
        case TOKENID_IDENTIFIER:
            printf("id");
            break;
        case TOKENID_KEYWORD_IF:
            printf("if");
            break;
        case TOKENID_KEYWORD_FOR:
            printf("for");
            break;
        
        case TOKENID_KEYWORD_ELSE:
            printf("else");
            break;
        case TOKENID_KEYWORD_RETURN:
            printf("return");
            break;
        case TOKENID_KEYWORD_PACKAGE:
            printf("package");
            break;
        case TOKENID_KEYWORD_FUNC:
            printf("func");
            break;
        case TOKENID_KEYWORD_FLOAT64:
            printf("float64");
            break;
        case TOKENID_KEYWORD_INT:
            printf("int");
            break;
        case TOKENID_KEYWORD_STRING:
            printf("string");
            break;
        case TOKENID_KEYWORD_BOOL:
            printf("bool");
            break;
        case TOKENID_STRING_LITERAL:
            printf("string_literal");
            break;
        case TOKENID_BOOL_LITERAL:
            printf("bool_literal");
            break;
        case TOKENID_NUM:
            printf("num");
            break;
        case TOKENID_NUM_DECIMAL:
            printf("num_decimal");
            break;
        case TOKENID_OPERATOR_ADD:
            printf("+");
            break;
        case TOKENID_OPERATOR_ADD_AND_ASSIGN:
            printf("+=");
            break;
        case TOKENID_OPERATOR_SUB:
            printf("-");
            break;
        case TOKENID_OPERATOR_SUB_AND_ASSIGN:
            printf("-=");
            break;
        case TOKENID_OPERATOR_MUL:
            printf("*");
            break;
        case TOKENID_OPERATOR_MUL_AND_ASSIGN:
            printf("*=");
            break;
        case TOKENID_OPERATOR_DIV:
            printf("/");
            break;
        case TOKENID_OPERATOR_DIV_AND_ASSIGN:
            printf("/=");
            break;
        case TOKENID_OPERATOR_LESS:
            printf("<");
            break;
        case TOKENID_OPERATOR_LESS_OR_EQUAL:
            printf("<=");
            break;
        case TOKENID_OPERATOR_GREATER:
            printf(">");
            break;
        case TOKENID_OPERATOR_GREATER_OR_EQUAL:
            printf(">=");
            break;
        case TOKENID_OPERATOR_DECLARE:
            printf(":=");
            break;
        case TOKENID_OPERATOR_ASSIGN:
            printf("=");
            break;
        case TOKENID_OPERATOR_EQUALS:
            printf("==");
            break;
        case TOKENID_OPERATOR_NOT:
            printf("!");
            break;
        case TOKENID_OPERATOR_NOT_EQUAL:
            printf("!=");
            break;
        case TOKENID_OPERATOR_AND:
            printf("&&");
            break;
        case TOKENID_OPERATOR_OR:
            printf("||");
            break;
        case TOKENID_LEFT_PARENTHESES:
            printf("(");
            break;
        case TOKENID_RIGHT_PARENTHESES:
            printf(")");
            break;
        case TOKENID_LEFT_BRACKET:
            printf("{");
            break;
        case TOKENID_RIGHT_BRACKET:
            printf("}");
            break;
        case TOKENID_SEMICOLON:
            printf(";");
            break;
        case TOKENID_COMMA:
            printf(",");
            break;
        case TOKENID_NEWLINE:
            printf("\\n");
            break;
        case TOKENID_END_OF_FILE:
            printf("$");
            break;
        default:
            printf("not terminal");
            break;
    }
    return;
}

void print_stack(ntsymstack_t *stack) {
    printf("----  Stack  ----\n");
    for(int i = stack->length - 1; i >= 0; i--) {
        printf(" ");
        if(stack->memory[i]->is_terminal) {
            print_terminal(stack->memory[i]->id);
            printf("\n");
        }
        else {
            print_nonterminal(stack->memory[i]->id);
            printf("\n");
        }
    }
    if(stack->length == 0) {
        printf(" empty\n");
    }
    printf("----  -----  ----\n");
    printf(" Length: %6d\n", stack->length);
    printf(" Capacity: %4d\n", stack->capacity);
    printf("-----------------\n");
    return;
}

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

        printf("----  Token  ----\n ");
        print_terminal(token->id);
        printf("\n-----------------\n\n");
        print_stack(stack);
        printf("\n\n\n");

        if(token->id == TOKENID_NEWLINE) {
            if(EOL_FLAG == true) {
                token_dtor(token);
                token = token_next;
                if(token->id != TOKENID_END_OF_FILE) {
                    token_next = get_next_token();
                }
            }
            else {
                error = true;
            }
        }
        else if(stack_top->is_terminal && stack_top->id == TOKENID_END_OF_FILE) {
            if(token->id == TOKENID_END_OF_FILE) {
                ntsymbol_dtor(ntsymstack_pop(stack));
                token_dtor(token);
                break;
            }
            else {
                error = true;
            }
        }
        else if(stack_top->is_terminal) {
            if((tokenid_e)stack_top->id == token->id) {
                token_dtor(token);
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
                    error = nonterminal_parameters_derivation(stack, token->id);
                    break;
                case NONTERMINAL_PARAMETER:
                    error = nonterminal_parameter_derivation(stack, token->id);
                    break;
                case NONTERMINAL_PARAMETER_NEXT:
                    error = nonterminal_parameter_next_derivation(stack, token->id);
                    break;
                case NONTERMINAL_RETURN_TYPES:
                    error = nonterminal_return_types_derivation(stack, token->id);
                    break;
                case NONTERMINAL_TYPES:
                    error = nonterminal_types_derivation(stack, token->id);
                    break;
                case NONTERMINAL_TYPE:
                    error = nonterminal_type_derivation(stack, token->id);
                    break;
                case NONTERMINAL_TYPE_NEXT:
                    error = nonterminal_type_next_derivation(stack, token->id);
                    break;
                case NONTERMINAL_RETURN:
                    error = nonterminal_return_derivation(stack, token->id);
                    break;
                case NONTERMINAL_DEFINITION:
                    error = nonterminal_definition_derivation(stack, token->id);
                    break;
                case NONTERMINAL_FOR_DEFINITION:
                    error = nonterminal_for_definition_derivation(stack, token->id);
                    break;
                case NONTERMINAL_ASSIGNMENT:
                    error = nonterminal_assignment_derivation(stack, token->id);
                    break;
                case NONTERMINAL_FOR_ASSIGNMENT:
                    error = nonterminal_for_assignment_derivation(stack, token->id);
                    break;
                case NONTERMINAL_ASSIGNMENT_RIGHT:
                    error = nonterminal_assignment_right_derivation(stack, token->id, token_next->id);
                    break;
                case NONTERMINAL_FOR_ASSIGNMENT_RIGHT:
                    error = nonterminal_for_assignment_right_derivation(stack, token->id, token_next->id);
                    break;
                case NONTERMINAL_ID_NEXT:
                    error = nonterminal_id_next_derivation(stack, token->id);
                    break;
                case NONTERMINAL_COMMANDS:
                    error = nonterminal_commands_derivation(stack, token->id);
                    break;
                case NONTERMINAL_COMMAND:
                    error = nonterminal_command_derivation(stack, token->id, token_next->id);
                    break;
                case NONTERMINAL_IF:
                    error = nonterminal_if_derivation(stack, token->id);
                    break;
                case NONTERMINAL_ELSE:
                    error = nonterminal_else_derivation(stack, token->id);
                    break;
                case NONTERMINAL_FOR:
                    error = nonterminal_for_derivation(stack, token->id);
                    break;
                case NONTERMINAL_CALL:
                    error = nonterminal_call_derivation(stack, token->id);
                    break;
                case NONTERMINAL_IDS:
                    error = nonterminal_ids_derivation(stack, token->id);
                    break;
                case NONTERMINAL_EXPRESSIONS:
                    ntsymbol_dtor(ntsymstack_pop(stack));
                    break;
                case NONTERMINAL_EXPRESSION:
                    ntsymbol_dtor(ntsymstack_pop(stack));
                    break;
                case NONTERMINAL_EXPRESSION_NEXT:
                    ntsymbol_dtor(ntsymstack_pop(stack));
                    break;
                default:
                    error = true;
                    break;
            }
        }
    }

    ntsymstack_dtor(stack);

    if(error) {
        token_dtor(token);
        token_dtor(token_next);
        exit(ERRCODE_SYNTAX_ERROR);
    }
   
    return;
}
