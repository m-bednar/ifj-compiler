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
#include "../error.h"

// converts tokenid type keyword to vartype
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

int semantic_expression(tokenvector_t* token_vector, vartype_e* ret_type, bintreestack_t* symtable_stack){
   token_t* token;
   token_t* next_token;
   symbol_t* symbol;
   vartype_e next_type;
   vartype_e type;
   tokenvector_t* operators;
   int level;
   
   token = tokenvector_get(token_vector, 0);
   if(token->id == TOKENID_OPERATOR_NOT){
      token = tokenvector_get(token_vector, 1);
   }

   if(token->id == TOKENID_IDENTIFIER){
      symbol = bintreestack_find(symtable_stack, token->value.string_value, &level);
      if(symbol == NULL){
         return ERRCODE_VAR_UNDEFINED_ERROR;
      }
      else{
         type = symbol->value.var->type;
         next_type = type;
      }
   }
   else{
      type = get_const_type(token);
      next_type = type;
   }

   operators = tokenvector_ctor();
   for(int i = 0; i < tokenvector_get_lenght(token_vector); i++){
      token =  tokenvector_get(token_vector, i);

      // check type
      if(token->id == TOKENID_IDENTIFIER){
         // check if var was declared
         symbol = bintreestack_find(symtable_stack, token->value.string_value, &level);
         if(symbol == NULL){
            return ERRCODE_VAR_UNDEFINED_ERROR;
         }
         else{
            next_type = symbol->value.var->type;
         }
      }
      else if(token->id == TOKENID_STRING_LITERAL || token->id == TOKENID_BOOL_LITERAL || token->id == TOKENID_NUM || token->id == TOKENID_NUM_DECIMAL){
         next_type = get_const_type(token);
      }
      else{
         tokenvector_push(operators, token);
      }

      if(type == VT_FLOAT || type == VT_INT){
         
         // zero division check
         if(token->id == TOKENID_OPERATOR_DIV){
            next_token = tokenvector_get(token_vector, i + 1);
            if(next_token->id == TOKENID_NUM && next_token->value.int_value == 0){
               return ERRCODE_ZERO_DIVISION_ERROR;
            }
            else if(next_token->id == TOKENID_NUM_DECIMAL && next_token->value.decimal_value == 0.0){
               return ERRCODE_ZERO_DIVISION_ERROR;
            }
         }
      }
      if(type != next_type){
         return ERRCODE_TYPE_INCOMPATIBLE_ERROR;
      }

   }

   
   if(tokenvector_get_lenght(operators) != 0){
      for(int i = 0; i < tokenvector_get_lenght(operators); i++){
         token =  tokenvector_get(operators, i);
         if(is_relational_operator(token)){
            type = VT_BOOL;
         }
         switch(type){
            case VT_BOOL:
               if(token->id != TOKENID_OPERATOR_EQUALS && token->id != TOKENID_OPERATOR_NOT_EQUAL && token->id != TOKENID_OPERATOR_NOT && token->id != TOKENID_OPERATOR_AND && token->id != TOKENID_OPERATOR_OR){
                  return ERRCODE_GENERAL_SEMANTIC_ERROR;
               }
               break;
            case VT_INT:

               break;
            case VT_FLOAT:
               break;
            case VT_STRING:
               if(token->id == TOKENID_OPERATOR_SUB || token->id == TOKENID_OPERATOR_MUL || token->id == TOKENID_OPERATOR_LESS || token->id == TOKENID_OPERATOR_GREATER || token->id == TOKENID_OPERATOR_GREATER_OR_EQUAL){
                  return ERRCODE_GENERAL_SEMANTIC_ERROR;
               }
               break;
            default:
               break;
         }
      }
   }

   (*ret_type) = type;
   tokenvector_dtor(operators);
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
   astnode_assign_add_ids((*assign_node), variables_array, size);

   i++;
   
   do{
      token = tokenvector_get(token_vector, i);
      tokenvector_push(funccall, token);
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
         for(int i = 0 ; i < funccall_node->params_count; i++){
            token = tokenvector_get(variables, i);
            return_types[i] = bintreestack_find(symtable_stack, token->value.string_value, NULL)->value.var->type;
         }
      }
      token = tokenvector_get(funccall, 0);
      symbolval_u new_symbolval = symbolval_fn_ctor(funccall_node->params_count, tokenvector_get_lenght(variables), NULL, arg_types, return_types, false);
      bintree_add(symtable_global, symbol_ctor(token->value.string_value, ST_FUNCTION, new_symbolval));
   }

   tokenvector_dtor(funccall);
   tokenvector_dtor(variables);
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

      tokenvector_dtor(expression);

   }

   if(tokenvector_get_lenght(variables) != expression_types_size){
      //tokenvector_print(variables);
      return ERRCODE_GENERAL_SEMANTIC_ERROR;
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
      symbol->value.fn->arg_names=arg_names;
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
      expressions_count=expressions_count;
      if(function_symbol->value.fn->ret_count < expressions_count || function_symbol->value.fn->ret_types[expressions_count - 1] != expression_type){
         tokenvector_dtor(expression);
         return ERRCODE_ARGS_OR_RETURN_ERROR;
      }

      expressionArray = tokenvector_get_array(expression, &size);
      astnode_ret_add_exp((*node_ret), astnode_exp_ctor(expressionArray, size));
      tokenvector_dtor(expression);
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
   //int err;
   //vartype_e type;
   i = 1; // skip token with if 
   token = tokenvector_get(token_vector, i);
   while(token->id != TOKENID_LEFT_BRACKET && tokenvector_get_lenght(token_vector) > i){
      tokenvector_push(condition, token_copy(token));

      i++;
      token = tokenvector_get(token_vector, i);
   }

   /*err = semantic_expression(condition, &type, symtable_stack);

   if(err){
      return err;
   }
   if(type != VT_BOOL){
      return ERRCODE_GENERAL_SEMANTIC_ERROR;
   }*/
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
   //printf("for: ");
   //tokenvector_print(token_vector);
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
   tokenvector_dtor(condition);
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
         printf(" konec nebyla definovana %s",functions[i]->identifier);
         return ERRCODE_VAR_UNDEFINED_ERROR;
      }
   }
   return 0;
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
   
   if(token->id == TOKENID_RIGHT_BRACKET){
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
      if(astnodestack_lenght(ast_parents) != 0)
         astnodestack_pop(ast_parents);
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
   tokenvector_print(token_vector);
   switch(current_flag){
      case NONTERMINAL_PACKAGE:
         err = semantic_package(token_vector, was_main);
         if(!err){
            was_main = true;
         }
         break;
      // FUNCTION DECLARATION
      case NONTERMINAL_FUNCTION:
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
         }

         astnodestack_push(ast_parents, ast_node_generic, AP_IF_TRUE);
         break;
      case NONTERMINAL_ELSE:
         bintreestack_push(symtable_stack, bintree_ctor());
         was_right_bracket = false;
         printf("paren body : %d \n",astnodestack_peek(ast_parents)->parentbody);
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


