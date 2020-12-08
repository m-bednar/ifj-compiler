/**
 * IFJ20 Compiler
 * @file semantic.c
 * @authors Michal Trlica (xtrlic02)
 */

#include <stdbool.h>
#include <string.h>
#include <stdio.h>
#include "semantic.h"
#include "tokenvector.h"
#include "astnodestack.h"
#include "../generator/postfix.h"
#include "../error.h"

/*
 * converts tokenid type keyword to vartype
 */
vartype_e tokenid_to_vartype(tokenid_e tokenid){
   switch(tokenid){
      case TOKENID_KEYWORD_FLOAT64:
         return VT_FLOAT;
      break;
      case TOKENID_KEYWORD_INT:
         return VT_INT;
      break;
      case TOKENID_KEYWORD_STRING:
         return VT_STRING;
      break;
      case TOKENID_KEYWORD_BOOL:
         return VT_BOOL;
      break;
      default:
         error("invalid tokenid");
   }
}

/*
 * converts vartype to tokenid
 */
tokenid_e vartype_to_tokenid(vartype_e vartype){
   switch(vartype){
      case VT_FLOAT:
         return TOKENID_NUM_DECIMAL;
      break;
      case VT_INT:
         return TOKENID_NUM;
      break;
      case VT_STRING:
         return TOKENID_STRING_LITERAL;
      break;
      case VT_BOOL:
         return TOKENID_BOOL_LITERAL;
      break;
      default:
         error("invalid vartype");
   }
}

/*
 * returns true if type1 and type2 are same or if type1 is NULL
 * if type1 is -1 then type1 = type2
 */
vartype_e get_const_type(token_t* token){
   switch(token->id){
      case TOKENID_STRING_LITERAL:
         return VT_STRING;
         break;
      case TOKENID_BOOL_LITERAL:
         return VT_BOOL;
         break;
      case TOKENID_NUM:
         return VT_INT;
         break;
      case TOKENID_NUM_DECIMAL:
         return VT_FLOAT;
         break;
      default:
         error("invalid const type");
   }
}

bool is_relational_operator(token_t* token){
   if(token->id == TOKENID_OPERATOR_ADD || token->id == TOKENID_OPERATOR_SUB || token->id == TOKENID_OPERATOR_MUL || token->id == TOKENID_OPERATOR_DIV){
      return false;
   }
   else{
      return true;
   }
}

bool is_operator(token_t* token){
   if(token->id == TOKENID_OPERATOR_ADD || token->id == TOKENID_OPERATOR_SUB || token->id == TOKENID_OPERATOR_MUL || token->id == TOKENID_OPERATOR_DIV || token->id == TOKENID_OPERATOR_LESS || token->id == TOKENID_OPERATOR_LESS_OR_EQUAL || token->id == TOKENID_OPERATOR_GREATER || token->id == TOKENID_OPERATOR_GREATER_OR_EQUAL || token->id == TOKENID_OPERATOR_EQUALS || token->id == TOKENID_OPERATOR_EQUALS || token->id == TOKENID_OPERATOR_NOT || token->id == TOKENID_OPERATOR_NOT_EQUAL || token->id == TOKENID_OPERATOR_AND || token->id == TOKENID_OPERATOR_OR){
      return true;
   }
   else{
      return false;
   }
}

bool is_binary_operator(token_t* token){
   if(token->id == TOKENID_OPERATOR_NOT){
      return true;
   }
   else{
      return false;
   }
}

void expression_replace_tree_tokens(astnode_exp_t* exp, token_t* token, int i){
   token_dtor(exp->tokens[i]);
   token_dtor(exp->tokens[i + 1]);
   token_dtor(exp->tokens[i + 2]);

   exp->tokens[i] = token;

   for (int j = i + 3; j < exp->tokens_count; j++) {
      exp->tokens[j - 2] = exp->tokens[j];
   }
   
   exp->tokens_count -= 2;
}

void expression_replace_two_tokens(astnode_exp_t* exp, token_t* token, int i){

   token_dtor(exp->tokens[i]);
   token_dtor(exp->tokens[i + 1]);

   exp->tokens[i] = token;

   for (int j = i + 2; j < exp->tokens_count; j++) {
      exp->tokens[j - 1] = exp->tokens[j];
   }
   
   exp->tokens_count -= 1;
}

/*
 * returns type result of operation or NULL if operation is not valid
 */
token_t* expression_eval_operation_not(token_t* t1, bintreestack_t* symtable_stack){
   symbol_t* symbol;
   tokenid_e tokenid_type_t1;
   if(t1->id == TOKENID_IDENTIFIER){
      symbol = bintreestack_find(symtable_stack, t1->value.string_value, NULL);
      tokenid_type_t1 = vartype_to_tokenid(symbol->value.var->type);
   }
   else{
      tokenid_type_t1 = vartype_to_tokenid(get_const_type(t1));
   }
   if(tokenid_type_t1 != TOKENID_BOOL_LITERAL){
      return NULL;
   }
   token_value_u value;
   return token_ctor(TOKENID_BOOL_LITERAL, value);
}

/*
 * returns type result of operation or NULL if operation is not valid
 */
token_t* expression_eval_operation(token_t* t1, token_t* t2, token_t* t3, bintreestack_t* symtable_stack){
   symbol_t* symbol;
   tokenid_e tokenid_type_t1;
   tokenid_e tokenid_type_t2;
   if(t1->id == TOKENID_IDENTIFIER){
      symbol = bintreestack_find(symtable_stack, t1->value.string_value, NULL);
      tokenid_type_t1 = vartype_to_tokenid(symbol->value.var->type);
   }
   else{
      tokenid_type_t1 = vartype_to_tokenid(get_const_type(t1));
   }
   if(t2->id == TOKENID_IDENTIFIER){
      symbol = bintreestack_find(symtable_stack, t2->value.string_value, NULL);
      tokenid_type_t2 = vartype_to_tokenid(symbol->value.var->type);
   }
   else{
      tokenid_type_t2 = vartype_to_tokenid(get_const_type(t2));
   }
   
   if(tokenid_type_t1 != tokenid_type_t2){
      return NULL;
   }
   
   token_value_u value;
   value.string_value=NULL;
   switch(tokenid_type_t1){
      case TOKENID_NUM_DECIMAL:
         if(t3->id == TOKENID_OPERATOR_LESS || t3->id == TOKENID_OPERATOR_LESS_OR_EQUAL || t3->id == TOKENID_OPERATOR_GREATER || t3->id == TOKENID_OPERATOR_GREATER_OR_EQUAL || t3->id == TOKENID_OPERATOR_EQUALS || t3->id == TOKENID_OPERATOR_NOT_EQUAL){
            return token_ctor(TOKENID_BOOL_LITERAL, value);
         }
         else if(t3->id == TOKENID_OPERATOR_ADD || t3->id == TOKENID_OPERATOR_SUB || t3->id == TOKENID_OPERATOR_MUL || t3->id == TOKENID_OPERATOR_DIV){
            return token_ctor(TOKENID_NUM_DECIMAL, value);
         }
         else{
            return NULL;
         }
         break;
      case TOKENID_NUM:
         if(t3->id == TOKENID_OPERATOR_LESS || t3->id == TOKENID_OPERATOR_LESS_OR_EQUAL || t3->id == TOKENID_OPERATOR_GREATER || t3->id == TOKENID_OPERATOR_GREATER_OR_EQUAL || t3->id == TOKENID_OPERATOR_EQUALS || t3->id == TOKENID_OPERATOR_NOT_EQUAL){
            return token_ctor(TOKENID_BOOL_LITERAL, value);
         }
         else if(t3->id == TOKENID_OPERATOR_ADD || t3->id == TOKENID_OPERATOR_SUB || t3->id == TOKENID_OPERATOR_MUL || t3->id == TOKENID_OPERATOR_DIV){
            return token_ctor(TOKENID_NUM, value);
         }
         else{
            return NULL;
         }
      case TOKENID_STRING_LITERAL:
         if(t3->id == TOKENID_OPERATOR_ADD){
            value.string_value = safe_alloc(sizeof(char));
            strcpy(value.string_value, ""); //string must be set
            return token_ctor(TOKENID_STRING_LITERAL, value);
         }
         else if(t3->id == TOKENID_OPERATOR_EQUALS || t3->id == TOKENID_OPERATOR_NOT_EQUAL){
            return token_ctor(TOKENID_BOOL_LITERAL, value);
         }
         else{
            return NULL;
         }
         return NULL;
         break;
      case TOKENID_BOOL_LITERAL:
         if(t3->id == TOKENID_OPERATOR_EQUALS || t3->id == TOKENID_OPERATOR_NOT_EQUAL || t3->id == TOKENID_OPERATOR_AND || t3->id == TOKENID_OPERATOR_OR){
            return token_ctor(TOKENID_BOOL_LITERAL, value);
         }
         else{
            return NULL;
         }
         break;
      default:
         error("invalid type");
         break;
   }
   return NULL;
}

int semantic_expression(tokenvector_t* token_vector, vartype_e* ret_type, bintreestack_t* symtable_stack){
   int expression_size = 0;
   bool reduced = false;
   symbol_t* symbol;

   //check if all variables was defined and if devide by zero may occure
   for(int i = 0; i < tokenvector_get_lenght(token_vector); i++){
      token_t* token = tokenvector_get(token_vector, i);
      if (token->id == TOKENID_IDENTIFIER) {
         symbol = bintreestack_find(symtable_stack, token->value.string_value, NULL);
         if(symbol == NULL){
            return ERRCODE_VAR_UNDEFINED_ERROR;
         }
      }
      if(token->id == TOKENID_OPERATOR_DIV){
         token_t* token_next = tokenvector_get(token_vector, i + 1);
         if(token_next->id == TOKENID_NUM && token_next->value.int_value == 0){
            return ERRCODE_ZERO_DIVISION_ERROR;
         }
         if(token_next->id == TOKENID_NUM_DECIMAL && token_next->value.decimal_value == 0.0){
            return ERRCODE_ZERO_DIVISION_ERROR;
         }
      }
   }

   token_t** expression_array = tokenvector_get_array(token_vector, &expression_size);
   astnode_exp_t* node_exp = astnode_exp_ctor(expression_array, expression_size);
   infix_to_postfix(node_exp);

   if(node_exp->tokens_count == 1){
      if(node_exp->tokens[0]->id == TOKENID_IDENTIFIER){
         symbol = bintreestack_find(symtable_stack, node_exp->tokens[0]->value.string_value, NULL);
         (*ret_type) = symbol->value.var->type;
      }
      else{
         (*ret_type) = get_const_type(node_exp->tokens[0]);
      }
   }
   else{
      do{
         reduced = false;
         for(int i = 0; i < node_exp->tokens_count && !reduced; i++){
            if(is_operator(node_exp->tokens[i])){
               if(node_exp->tokens[i]->id == TOKENID_OPERATOR_NOT){
                  token_t* t1 = node_exp->tokens[i -1];
                  token_t* token_eval = expression_eval_operation_not(t1, symtable_stack);
                  if(token_eval == NULL){
                     astnode_exp_dtor(node_exp);
                     return ERRCODE_TYPE_INCOMPATIBLE_ERROR;
                  }
                  expression_replace_two_tokens(node_exp, token_eval, i-1);
               }
               else{
                  token_t* t1 = node_exp->tokens[i - 2];
                  token_t* t2 = node_exp->tokens[i - 1];
                  token_t* t3 = node_exp->tokens[i];
                  token_t* token_eval = expression_eval_operation(t1, t2, t3, symtable_stack);
                  if(token_eval == NULL){
                     astnode_exp_dtor(node_exp);
                     return ERRCODE_TYPE_INCOMPATIBLE_ERROR;
                  }
                  expression_replace_tree_tokens(node_exp, token_eval, i-2);
               }
               reduced = true;
            }
         }
      }while(node_exp->tokens_count != 1);
      (*ret_type)= get_const_type(node_exp->tokens[0]);
   }
   astnode_exp_dtor(node_exp);
   return 0;
}


int semantic_definition(tokenvector_t* token_vector, bintreestack_t* symtable_stack, astnode_defvar_t** def_node){
   tokenvector_t* expresion = tokenvector_ctor();
   token_t** expresion_array;
   token_t* token;
   vartype_e type;
   symbol_t* new_symbol;
   symbol_t* symbol;
   int err = 0;
   int level;
   int size;
   int i;
   i=2;
   
   token = tokenvector_get(token_vector, i);
   while(tokenvector_get_lenght(token_vector) > i){
      tokenvector_push(expresion, token_copy(token));

      i++;
      if(tokenvector_get_lenght(token_vector) == i){
         break;
      }
      token = tokenvector_get(token_vector, i);
   }

   err = semantic_expression(expresion, &type, symtable_stack);

   if(err){
      return err; //error occured in expression
   }

   token = tokenvector_get(token_vector, 0);

   symbol = bintreestack_find(symtable_stack, token->value.string_value, &level);
   if(level == (bintreestack_get_length(symtable_stack) - 1) && symbol != NULL){

      return ERRCODE_VAR_UNDEFINED_ERROR; //variable was already declared in this scope 
   }

   new_symbol = symbol_ctor(token->value.string_value, ST_VARIABLE, symbolval_var_ctor(type));
   bintree_add(bintreestack_peek(symtable_stack), new_symbol);

   // create ast node
   expresion_array = tokenvector_get_array(expresion, &size);
   (*def_node) = astnode_defvar_ctor(tokenvector_get(token_vector,0), astnode_exp_ctor(expresion_array, size));
   
   return 0;
}

int semantic_funcall(tokenvector_t* token_vector, astnode_funccall_t** ast_node){
   tokenvector_t* args = tokenvector_ctor();
   token_t** args_array;
   token_t* token;
   int i;
   int size;

   i = 2;
   token = tokenvector_get(token_vector, i);
   while(token->id != TOKENID_RIGHT_PARENTHESES){
      if(token -> id != TOKENID_COMMA){
         tokenvector_push(args, token_copy(token));
      }
      i++;
      token = tokenvector_get(token_vector, i);
   }

   args_array = tokenvector_get_array(args, &size);
   token = tokenvector_get(token_vector, 0);
   (*ast_node) = astnode_funccall_ctor(token->value.string_value, args_array, size);   
   return 0;
}

int semantic_assign_funccall(tokenvector_t* token_vector, bintreestack_t* symtable_stack, bintree_t* symtable_global, astnode_assign_t** assign_node){
   tokenvector_t* funccall = tokenvector_ctor();
   tokenvector_t* variables = tokenvector_ctor();
   token_t** variables_array;
   token_t* token;
   astnode_funccall_t* funccall_node;
   symbol_t* symbol;
   int i;
   int size;
   int err = 0;
   (*assign_node) = astnode_assign_ctor();
   symtable_stack = symtable_stack;
   i = 0;
   token = tokenvector_get(token_vector, i);
   //reading variables
   while(token->id != TOKENID_OPERATOR_ASSIGN){
      if(token -> id == TOKENID_IDENTIFIER){
         tokenvector_push(variables, token_copy(token));
      }
      i++;
      token = tokenvector_get(token_vector, i);
   }

   variables_array = tokenvector_get_array(variables, &size);
   printf("%p %d",variables_array, size);
   astnode_assign_add_ids((*assign_node), variables_array, size);

   i++;
   
   do{
      token = tokenvector_get(token_vector, i);
      tokenvector_push(funccall, token_copy(token));
      i++;
   }while(tokenvector_get_lenght(token_vector) > i);

   err = semantic_funcall(funccall, &funccall_node);
   astnode_assign_add_funccall((*assign_node), funccall_node);
   
   token = tokenvector_get(funccall, 0);
   symbol = bintree_find(symtable_global ,token->value.string_value);
   if(symbol == NULL){
      //expected arguments for yet undefined function
      vartype_e* arg_types = NULL;
      if(funccall_node->params_count != 0){
         arg_types = safe_alloc(sizeof(vartype_e) * funccall_node->params_count);
         for(int i = 0 ; i < funccall_node->params_count; i++){
            token = funccall_node->params[i];
            if(token->id == TOKENID_IDENTIFIER){
               arg_types[i] = bintreestack_find(symtable_stack, token->value.string_value, NULL)->value.var->type;
            }
            else{
               arg_types[i] =  get_const_type(token);
            }
         }
      }
      //expected return types for yet undefined function
      vartype_e* return_types = NULL;
      if(tokenvector_get_lenght(variables) != 0){
         return_types = safe_alloc(sizeof(vartype_e) * tokenvector_get_lenght(variables));
         for(int i = 0 ; i < tokenvector_get_lenght(variables); i++){
            token = tokenvector_get(variables, i);
            return_types[i] = bintreestack_find(symtable_stack, token->value.string_value, NULL)->value.var->type;
         }
      }
      token = tokenvector_get(funccall, 0);
      symbolval_u new_symbolval = symbolval_fn_ctor(funccall_node->params_count, tokenvector_get_lenght(variables), NULL, arg_types, return_types, false);
      bintree_add(symtable_global, symbol_ctor(token->value.string_value, ST_FUNCTION, new_symbolval));
   }

   //tokenvector_dtor(funccall);
   //tokenvector_dtor(variables);
   return err;
}

int semantic_assign(tokenvector_t* token_vector, bintreestack_t* symtable_stack, astnode_assign_t** assign_node){
   tokenvector_t* expression = tokenvector_ctor();
   token_t** expression_array;
   tokenvector_t* variables = tokenvector_ctor();
   token_t** variables_array;
   vartype_e* expression_types = NULL; //vartype array
   int expression_types_size = 0;
   token_t* token;
   vartype_e type = 0;
   symbol_t* symbol;
   int i = 0;
   int level;
   int size;
   int err;

   (*assign_node) = astnode_assign_ctor();

   token = tokenvector_get(token_vector, i);
   //reading variables
   while(token->id != TOKENID_OPERATOR_ASSIGN){
      if(token -> id == TOKENID_IDENTIFIER){
         tokenvector_push(variables, token_copy(token));
      }
      i++;
      token = tokenvector_get(token_vector, i);
   }

   variables_array = tokenvector_get_array(variables, &size);
   astnode_assign_add_ids((*assign_node), variables_array, size);

   //reading expresions
   while(tokenvector_get_lenght(token_vector) != i){
      i++;
      expression = tokenvector_ctor();
      token = tokenvector_get(token_vector, i);
      while(token->id != TOKENID_COMMA && tokenvector_get_lenght(token_vector) > i){
         tokenvector_push(expression, token_copy(token));

         i++;
         if(tokenvector_get_lenght(token_vector) == i){
            break;
         }
         token = tokenvector_get(token_vector, i);
      }

      expression_array = tokenvector_get_array(expression, &size);
      astnode_assign_add_exp((*assign_node), astnode_exp_ctor(expression_array, size));
      err = semantic_expression(expression, &type, symtable_stack);
      if(err){
         return err;
      }

      expression_types = safe_realloc(expression_types, sizeof(vartype_e) * (expression_types_size + 1));
      expression_types[expression_types_size] = type;
      expression_types_size++;

   }

   if(tokenvector_get_lenght(variables) != expression_types_size){
      return ERRCODE_SYNTAX_ERROR;
   }
   for(i = 0; i < tokenvector_get_lenght(variables); i++){
      token = tokenvector_get(variables, i);
      symbol = bintreestack_find(symtable_stack, token->value.string_value, &level);
      if(symbol == NULL){
         return ERRCODE_VAR_UNDEFINED_ERROR;
      }
      if(symbol->value.var->type != expression_types[i]){
         return ERRCODE_GENERAL_SEMANTIC_ERROR;
      }
   }

   return 0;
}

int semantic_function_decl(tokenvector_t* token_vector, bintreestack_t* symtable_stack, bintree_t* symtable_global){
   token_t* token;
   token = tokenvector_get(token_vector, 1);
   symbol_t* symbol;
   char** arg_names = NULL;
   int arg_names_count = 0;
   int i;
   vartype_e* arg_types = NULL;
   vartype_e* ret_types = NULL;
   int arg_count = 0;
   int ret_count = 0;
   i = 3;
   //function args handling
   token = tokenvector_get(token_vector, i);
   while(token->id != TOKENID_RIGHT_PARENTHESES){
      
      if(token->id == TOKENID_KEYWORD_FLOAT64 || token->id == TOKENID_KEYWORD_INT || token->id == TOKENID_KEYWORD_STRING || token->id == TOKENID_KEYWORD_BOOL){
         arg_types = safe_realloc(arg_types, sizeof(vartype_e)* (arg_count + 1));
         arg_types[arg_count] = tokenid_to_vartype(token->id);
         arg_count++;
         //add arguments to local symtable
         token_t* token_before = tokenvector_get(token_vector, i - 1);
         symbol = symbol_ctor(token_before->value.string_value, ST_VARIABLE, symbolval_var_ctor(tokenid_to_vartype(token->id)));
         bintree_add(bintreestack_peek(symtable_stack), symbol);

      }
      if(token->id == TOKENID_IDENTIFIER){
         arg_names = safe_realloc(arg_names, sizeof(char*) * (arg_names_count+1));
         arg_names[arg_names_count] = safe_alloc(sizeof(char) * (strlen(token->value.string_value) + 1));
         strcpy(arg_names[arg_names_count], token->value.string_value);
         arg_names_count++;

      }
      i++;
      token = tokenvector_get(token_vector, i);

   }
   //function rets handling
   i++;
   token = tokenvector_get(token_vector, i);
   if(token->id != TOKENID_LEFT_BRACKET){
      while(token->id != TOKENID_RIGHT_PARENTHESES){
         if(token->id == TOKENID_KEYWORD_FLOAT64 || token->id == TOKENID_KEYWORD_INT || token->id == TOKENID_KEYWORD_STRING || token->id == TOKENID_KEYWORD_BOOL){
            
            ret_types = safe_realloc(ret_types, sizeof(vartype_e)* (ret_count + 1));
            ret_types[ret_count] = tokenid_to_vartype(token->id);
            ret_count++;
         }
         i++;
         token = tokenvector_get(token_vector, i);
      }
   }
   else{
      ret_types = NULL;
      ret_count = 0;
   }

   token = tokenvector_get(token_vector, 1);
   //add function declaration to symtable
   symbol = bintree_find(symtable_global, token->value.string_value);
   if(symbol == NULL){
      bintree_add(symtable_global, symbol_ctor(token->value.string_value, ST_FUNCTION, symbolval_fn_ctor(arg_count, ret_count, arg_names, arg_types, ret_types, true)));
   }
   else{
      if(symbol->value.fn->defined){
         //function was already declared
         return ERRCODE_VAR_UNDEFINED_ERROR;
      }
      if(symbol->value.fn->arg_count != arg_count || symbol->value.fn->ret_count != ret_count){
         //pre declaration doesnt match
         return ERRCODE_ARGS_OR_RETURN_ERROR;
      }

      for(int i = 0; i < symbol->value.fn->arg_count; i++){
         if(symbol->value.fn->arg_types[i] != arg_types[i]){
            return ERRCODE_ARGS_OR_RETURN_ERROR;
         }
      }
      for(int i = 0; i < symbol->value.fn->ret_count; i++){
         if(symbol->value.fn->ret_types[i] != ret_types[i]){
            return ERRCODE_ARGS_OR_RETURN_ERROR;
         }
      }
      //pre declaration match
      symbol->value.fn->arg_names = arg_names;
      symbol->value.fn->defined = true;
   }

   return 0;
}

int semantic_ret(tokenvector_t* token_vector, astnode_funcdecl_t* function, bintreestack_t* symtable_stack, bintree_t* symtable_global, astnode_generic_t** node_ret){
   int i;
   tokenvector_t* expression;
   token_t** expressionArray;
   token_t* token;
   int size;
   int expressions_count = 0;
   symbol_t* function_symbol;
   int err = 0;
   vartype_e expression_type  = VT_BOOL;
   (*node_ret) = astnode_ret_ctor();
   i = 1;

   function_symbol = bintree_find(symtable_global, function->name);
   while(tokenvector_get_lenght(token_vector) > i){
      expression = tokenvector_ctor();
      do{
         token = tokenvector_get(token_vector, i);
         if(token->id != TOKENID_COMMA){
            tokenvector_push(expression, token_copy(token));
         }

         i++;
      }while(token->id != TOKENID_COMMA && tokenvector_get_lenght(token_vector) > i);
      
      err = semantic_expression(expression, &expression_type, symtable_stack);
      if(err){
         tokenvector_dtor(expression);
         return err;
      }
      expressions_count++;
      if(function_symbol->value.fn->ret_count < expressions_count || function_symbol->value.fn->ret_types[expressions_count - 1] != expression_type){
         tokenvector_dtor(expression);
         return ERRCODE_ARGS_OR_RETURN_ERROR;
      }

      expressionArray = tokenvector_get_array(expression, &size);
      astnode_ret_add_exp((*node_ret), astnode_exp_ctor(expressionArray, size));
      //tokenvector_dtor(expression);
   }
   if(function_symbol->value.fn->ret_count != expressions_count){
      return ERRCODE_ARGS_OR_RETURN_ERROR; // too few return values
   }


   return 0;
}

int semantic_if(tokenvector_t* token_vector, astnode_generic_t** ast_node, bintreestack_t* symtable_stack){
   tokenvector_t* condition = tokenvector_ctor();
   token_t** condition_array;
   token_t* token;
   int i;
   int size;
   symtable_stack=symtable_stack;
   int err;
   vartype_e type;
   if(tokenvector_get(token_vector,0)->id == TOKENID_KEYWORD_ELSE){
      i=2; // skip token "else" and "if"
   }
   else{
      i = 1; // skip token "if" 
   }
   token = tokenvector_get(token_vector, i);
   while(token->id != TOKENID_LEFT_BRACKET && tokenvector_get_lenght(token_vector) > i){
      tokenvector_push(condition, token_copy(token));

      i++;
      token = tokenvector_get(token_vector, i);
   }

   err = semantic_expression(condition, &type, symtable_stack);

   if(err){
      return err;
   }
   if(type != VT_BOOL){
      return ERRCODE_GENERAL_SEMANTIC_ERROR;
   }
   condition_array = tokenvector_get_array(condition, &size);

   (*ast_node) = astnode_if_ctor(astnode_exp_ctor(condition_array, size));

   return 0;
}

int semantic_for(tokenvector_t* token_vector, bintreestack_t* symtable_stack, astnode_generic_t** ast_node){
   tokenvector_t* def = tokenvector_ctor();
   tokenvector_t* condition = tokenvector_ctor();
   tokenvector_t* assign = tokenvector_ctor();
   token_t** condition_array;
   //token_t** def_array;
   //token_t** assign_array;
   token_t* token;
   astnode_exp_t* condition_node = NULL;
   astnode_defvar_t* def_node = NULL;
   astnode_assign_t* assign_node = NULL;
   int err;
   int i;
   int size;
   i = 1; // skip token with for
   // read def
   token = tokenvector_get(token_vector, i);
   while(token->id != TOKENID_SEMICOLON && tokenvector_get_lenght(token_vector) > i){
      tokenvector_push(def, token_copy(token));
      i++;
      token = tokenvector_get(token_vector, i);
   }
   i++;
   // read condition
   token = tokenvector_get(token_vector, i);
   while(token->id != TOKENID_SEMICOLON && tokenvector_get_lenght(token_vector) > i){
      tokenvector_push(condition, token_copy(token));

      i++;
      token = tokenvector_get(token_vector, i);
   }
   i++;
   // read assign
   token = tokenvector_get(token_vector, i);
   while(token->id != TOKENID_LEFT_BRACKET && tokenvector_get_lenght(token_vector) > i){
      tokenvector_push(assign, token_copy(token));

      i++;
      token = tokenvector_get(token_vector, i);
   }

   if(tokenvector_get_lenght(def) != 0){
      bintreestack_push(symtable_stack, bintree_ctor());
      err = semantic_definition(def, symtable_stack, &def_node);
   }

   if(tokenvector_get_lenght(condition) != 0){
      condition_array = tokenvector_get_array(condition, &size);
      condition_node = astnode_exp_ctor(condition_array, size);
   }

   if(tokenvector_get_lenght(assign) != 0){
      err = semantic_assign(assign, symtable_stack, &assign_node);
   }

   (*ast_node) = astnode_for_ctor(condition_node, def_node, assign_node);

   tokenvector_dtor(assign);
   tokenvector_dtor(def);
   //tokenvector_dtor(condition);
   if(err){
      return err;
   }

   return 0;
}

int semantic_package(tokenvector_t* token_vector, bool was_main){
   token_t* token_package_name = tokenvector_get(token_vector, 1);
   if(strcmp(token_package_name->value.string_value, "main")){
      return ERRCODE_SYNTAX_ERROR;
   }
   else if(was_main){
      return ERRCODE_SYNTAX_ERROR;
   }
   return 0;
}

int semantic_check_undeclared_func(bintree_t* symtable_global){
   int functions_count = 0;
   symbol_t** functions = bintree_to_array(symtable_global, &functions_count);
   for(int i = 0; i < functions_count; i++){
      if(!functions[i]->value.fn->defined){
         return ERRCODE_VAR_UNDEFINED_ERROR;
      }
   }
   return 0;
}

vartype_e* vartype_arr_static_to_dyn(vartype_e static_arr[], int count){
   vartype_e* dynamic = safe_alloc(sizeof(vartype_e) * count);
   for(int i = 0; i < count; i++){
      dynamic[i] = static_arr[i];
   }
   return static_arr;
}

void add_buildin_funcs(bintree_t* symtable_global){
   symbol_t* symbol;
   //inputs() (string, int)
   vartype_e inputs_arg_types[] = {VT_STRING, VT_INT};
   symbol = symbol_ctor("inputs", ST_FUNCTION, symbolval_fn_ctor(0, 2, NULL, NULL, vartype_arr_static_to_dyn(inputs_arg_types, 2), true));
   bintree_add(symtable_global, symbol);
}

int semantic(token_t* token, nonterminalid_e flag, int eol_flag, astnode_generic_t* ast, bintree_t* symtable_global){
   static astnode_funcdecl_t* function;
   static bintreestack_t* symtable_stack = NULL;
   static tokenvector_t* token_vector = NULL;
   static astnodestack_t* ast_parents = NULL;
   static bool function_call = false;
   static nonterminalid_e current_flag;
   static bool was_right_bracket = false;
   static bool was_main = false;
   int err = 0;
   if(symtable_stack == NULL){
      symtable_stack = bintreestack_ctor();
   }
   if(token_vector == NULL){
      token_vector = tokenvector_ctor();
   }
   if(ast_parents == NULL){
      ast_parents = astnodestack_ctor();
   }
   
   if(symtable_global->root == NULL){
      add_buildin_funcs(symtable_global);
   }

   if(token->id == TOKENID_RIGHT_BRACKET){
      if(was_right_bracket){
         if(astnodestack_lenght(ast_parents) != 0){
            astnodestack_pop(ast_parents);
         }
      }
      was_right_bracket = true;
      if(bintreestack_get_length(symtable_stack) != 0)
         bintree_dtor(bintreestack_pop(symtable_stack));
      token_dtor(token);
      return 0;
   }

   if(!eol_flag){
      tokenvector_push(token_vector, token);
      if(tokenvector_get_lenght(token_vector) == 1){
         current_flag = flag;
      }
      if(flag == NONTERMINAL_CALL){
         function_call = true;
      }
      return 0; //whole line wasnt read yet
   }
   
   if(tokenvector_get_lenght(token_vector) == 0){
      tokenvector_dtor(token_vector);
      token_vector = NULL;
      return 0;
   }

   if(was_right_bracket && (current_flag != NONTERMINAL_ELSE && current_flag != NONTERMINAL_ELSE_IF)){
      was_right_bracket = false;
      if(astnodestack_lenght(ast_parents) != 0){
         astnodestack_pop(ast_parents);
      }
   }

   astnode_defvar_t* ast_def = NULL;
   astnode_assign_t* ast_assign = NULL;
   astnode_funccall_t* ast_funccall = NULL;
   astnode_generic_t* ast_node_generic = NULL;
   token_t* token_tmp = NULL;
   //ast insert

   if(current_flag != NONTERMINAL_PACKAGE && !was_main){
      return ERRCODE_SYNTAX_ERROR;
   }
   //tokenvector_print(token_vector);
   switch(current_flag){
      case NONTERMINAL_PACKAGE:
         err = semantic_package(token_vector, was_main);
         if(!err){
            was_main = true;
         }
         break;
      // FUNCTION DECLARATION
      case NONTERMINAL_FUNCTION:
         //TODO: if function has return
         token = tokenvector_get(token_vector, 1);
         function = astnode_funcdecl_ctor(token->value.string_value);
         ast_global_add_func(ast, function);
         bintreestack_push(symtable_stack, bintree_ctor());
         err = semantic_function_decl(token_vector, symtable_stack, symtable_global);
         
         break;
      case NONTERMINAL_DEFINITION:
         err = semantic_definition(token_vector, symtable_stack, &ast_def);

         if(ast_def != NULL){
            ast_node_generic = astnode_generic_defvar_ctor(ast_def);
            if(astnodestack_lenght(ast_parents) == 0){
               astnode_funcdecl_add(function, ast_node_generic);
            }
            else{
               if(astnodestack_peek(ast_parents)->parentbody == AP_FOR){
                  astnode_for_add_body(astnodestack_peek(ast_parents)->astnode, ast_node_generic);
               }
               else if(astnodestack_peek(ast_parents)->parentbody == AP_IF_TRUE){
                  astnode_if_add_truebody(astnodestack_peek(ast_parents)->astnode, ast_node_generic);
               }
               else if(astnodestack_peek(ast_parents)->parentbody == AP_IF_FALSE){
                  astnode_if_add_elsebody(astnodestack_peek(ast_parents)->astnode, ast_node_generic);
               }
            }
         }
         break;
      case NONTERMINAL_ASSIGNMENT:
         if(!function_call){
            err = semantic_assign(token_vector, symtable_stack, &ast_assign);
         }
         else{
            err = semantic_assign_funccall(token_vector, symtable_stack, symtable_global, &ast_assign);
            function_call = false;
         }
         if(ast_assign != NULL){
            ast_node_generic = astnode_generic_assign_ctor(ast_assign);
            if(astnodestack_lenght(ast_parents) == 0){
               astnode_funcdecl_add(function, ast_node_generic);
            }
            else{
               if(astnodestack_peek(ast_parents)->parentbody == AP_FOR){
                  astnode_for_add_body(astnodestack_peek(ast_parents)->astnode, ast_node_generic);
               }
               else if(astnodestack_peek(ast_parents)->parentbody == AP_IF_TRUE){
                  astnode_if_add_truebody(astnodestack_peek(ast_parents)->astnode, ast_node_generic);
               }
               else if(astnodestack_peek(ast_parents)->parentbody == AP_IF_FALSE){
                  astnode_if_add_elsebody(astnodestack_peek(ast_parents)->astnode, ast_node_generic);
               }
            }
         }
         
         break;
      case NONTERMINAL_RETURN:
         err = semantic_ret(token_vector, function, symtable_stack, symtable_global, &ast_node_generic);

         if(ast_node_generic != NULL){
            if(astnodestack_lenght(ast_parents) == 0){
               astnode_funcdecl_add(function, ast_node_generic);
            }
            else{
               if(astnodestack_peek(ast_parents)->parentbody == AP_FOR){
                  astnode_for_add_body(astnodestack_peek(ast_parents)->astnode, ast_node_generic);
               }
               else if(astnodestack_peek(ast_parents)->parentbody == AP_IF_TRUE){
                  astnode_if_add_truebody(astnodestack_peek(ast_parents)->astnode, ast_node_generic);
               }
               else if(astnodestack_peek(ast_parents)->parentbody == AP_IF_FALSE){
                  astnode_if_add_elsebody(astnodestack_peek(ast_parents)->astnode, ast_node_generic);
               }
            }
         }

         break;
      case NONTERMINAL_IF:
         err = semantic_if(token_vector, &ast_node_generic, symtable_stack);
         bintreestack_push(symtable_stack, bintree_ctor());
         if(ast_node_generic != NULL){
            if(astnodestack_lenght(ast_parents) == 0){
               astnode_funcdecl_add(function, ast_node_generic);
            }
            else{
               if(astnodestack_peek(ast_parents)->parentbody == AP_FOR){
                  astnode_for_add_body(astnodestack_peek(ast_parents)->astnode, ast_node_generic);
               }
               else if(astnodestack_peek(ast_parents)->parentbody == AP_IF_TRUE){
                  astnode_if_add_truebody(astnodestack_peek(ast_parents)->astnode, ast_node_generic);
               }
               else if(astnodestack_peek(ast_parents)->parentbody == AP_IF_FALSE){
                  astnode_if_add_elsebody(astnodestack_peek(ast_parents)->astnode, ast_node_generic);
               }
            }
            astnodestack_push(ast_parents, ast_node_generic, AP_IF_TRUE);
         }
         break;
      case NONTERMINAL_ELSE:
         bintreestack_push(symtable_stack, bintree_ctor());
         was_right_bracket = false;
         astnodestack_peek(ast_parents)->parentbody = AP_IF_FALSE;
         break;
      case NONTERMINAL_ELSE_IF:
         err = semantic_if(token_vector, &ast_node_generic, symtable_stack);
         bintreestack_push(symtable_stack, bintree_ctor());
         was_right_bracket = false;
         astnodestack_peek(ast_parents)->parentbody = AP_IF_FALSE;
         
         if(ast_node_generic != NULL){
            astnode_if_add_elsebody(astnodestack_peek(ast_parents)->astnode, ast_node_generic);
         }
         astnodestack_push(ast_parents, ast_node_generic, AP_IF_TRUE);
         break;
      case NONTERMINAL_FOR:
         err = semantic_for(token_vector, symtable_stack, &ast_node_generic);
         bintreestack_push(symtable_stack, bintree_ctor());

         if(ast_node_generic != NULL){
            if(astnodestack_lenght(ast_parents) == 0){
               astnode_funcdecl_add(function, ast_node_generic);
            }
            else{
               if(astnodestack_peek(ast_parents)->parentbody == AP_FOR){
                  astnode_for_add_body(astnodestack_peek(ast_parents)->astnode, ast_node_generic);
               }
               else if(astnodestack_peek(ast_parents)->parentbody == AP_IF_TRUE){
                  astnode_if_add_truebody(astnodestack_peek(ast_parents)->astnode, ast_node_generic);
               }
               else if(astnodestack_peek(ast_parents)->parentbody == AP_IF_FALSE){
                  astnode_if_add_elsebody(astnodestack_peek(ast_parents)->astnode, ast_node_generic);
               }
            }
         }
         astnodestack_push(ast_parents, ast_node_generic, AP_FOR);
         break;
      case NONTERMINAL_CALL:
         function_call = false;
         err = semantic_funcall(token_vector, &ast_funccall);
         if(ast_funccall != NULL){
            ast_node_generic = astnode_generic_funccall_ctor(ast_funccall);
            if(astnodestack_lenght(ast_parents) == 0){
               astnode_funcdecl_add(function, ast_node_generic);
            }
            else{
               if(astnodestack_peek(ast_parents)->parentbody == AP_FOR){
                  astnode_for_add_body(astnodestack_peek(ast_parents)->astnode, ast_node_generic);
               }
               else if(astnodestack_peek(ast_parents)->parentbody == AP_IF_TRUE){
                  astnode_if_add_truebody(astnodestack_peek(ast_parents)->astnode, ast_node_generic);
               }
               else if(astnodestack_peek(ast_parents)->parentbody == AP_IF_FALSE){
                  astnode_if_add_elsebody(astnodestack_peek(ast_parents)->astnode, ast_node_generic);
               }
            }
         }

         //add to symtable if its not yet declared
         token_tmp = tokenvector_get(token_vector,0);
         if(bintree_find(symtable_global ,token_tmp->value.string_value) == NULL){
            //expected arguments for yet undefined function
            vartype_e* arg_types = NULL;
            if(ast_funccall->params_count != 0){
               arg_types = safe_alloc(sizeof(vartype_e) * ast_funccall->params_count);
               for(int i = 0 ; i < ast_funccall->params_count; i++){
                  token_tmp = ast_funccall->params[i];
                  if(token_tmp->id == TOKENID_IDENTIFIER){
                     arg_types[i] = bintreestack_find(symtable_stack, token_tmp->value.string_value, NULL)->value.var->type;
                  }
                  else{
                     arg_types[i] =  get_const_type(token_tmp);
                  }
               }
            }

            token_tmp = tokenvector_get(token_vector, 0);
            symbolval_u new_symbolval = symbolval_fn_ctor(ast_funccall->params_count, 0, NULL, arg_types, NULL, false);
            bintree_add(symtable_global, symbol_ctor(token_tmp->value.string_value, ST_FUNCTION, new_symbolval));
         }

         break;
      default:
         break;
   }
   tokenvector_dtor(token_vector);
   token_vector = NULL;
   

   return err;
}
