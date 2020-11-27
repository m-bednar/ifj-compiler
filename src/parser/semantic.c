/**
 * IFJ20 Compiler
 * @file semantic.c
 * @authors Michal Trlica (xtrlic02)
 */

#include <stdbool.h>
#include <string.h>
#include <stdio.h>
#include "semantic.h"
#include "tokenstack.h"
#include "tokenvector.h"
#include "astnodestack.h"
#include "../error.h"
#include "../symtable/bintreestack.h"

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

   printf("EXP %d:\n",tokenvector_get_lenght(token_vector));
   tokenvector_print(token_vector);

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

   printf("DECL %d:\n",tokenvector_get_lenght(token_vector));
   tokenvector_print(token_vector);
   i=2;
   
   token = tokenvector_get(token_vector, i);
   while(tokenvector_get_lenght(token_vector) > i){
      tokenvector_push(expresion, token);

      i++;
      if(tokenvector_get_lenght(token_vector) == i){
         break;
      }
      token = tokenvector_get(token_vector, i);
      //printf("\nvector: %d, i: %d \n",tokenvector_get_lenght(token_vector), i);
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
   token_dtor(token);

   // create ast node
   expresion_array = tokenvector_get_array(expresion, &size);
   (*def_node) = astnode_defvar_ctor(tokenvector_get(token_vector,0), astnode_exp_ctor(expresion_array, size));
   
   return 0;
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


   printf("ASSIGN: ");
   tokenvector_print(token_vector);
   token = tokenvector_get(token_vector, i);
   //reading variables
   while(token->id != TOKENID_OPERATOR_ASSIGN){
      if(token -> id == TOKENID_IDENTIFIER){
         tokenvector_push(variables, token);
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

         tokenvector_push(expression, token);

         i++;
         if(tokenvector_get_lenght(token_vector) == i){
            break;
         }
         token = tokenvector_get(token_vector, i);
         //printf("\nvector: %d, i: %d \n",tokenvector_get_lenght(token_vector), i);
      }

      expression_array = tokenvector_get_array(expression, &size);
      astnode_assign_add_exp((*assign_node), astnode_exp_ctor(expression_array, size));
      err = semantic_expression(expression, &type, symtable_stack);
      if(err){
         return err;
      }

      if(expression_types == NULL){
         expression_types = safe_alloc(sizeof(vartype_e));
      }
      else{
         expression_types = safe_realloc(expression_types, sizeof(vartype_e) * (expression_types_size + 1));
      }
      expression_types[expression_types_size] = type;
      expression_types_size++;

      tokenvector_dtor(expression);

   }


   printf(" \n variables: ");
   tokenvector_print(variables);
   printf("\n vars: %d , exps: %d",tokenvector_get_lenght(variables),expression_types_size);

   if(tokenvector_get_lenght(variables) != expression_types_size){
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

   int i;
   vartype_e* arg_types = NULL;
   vartype_e* ret_types = NULL;
   int arg_count = 0;
   int ret_count = 0;
   i = 3;
   printf("\n IF args  ");
   //function args handling
   token = tokenvector_get(token_vector, i);
   while(token->id != TOKENID_RIGHT_PARENTHESES){
      
      if(token->id == TOKENID_KEYWORD_FLOAT64 || token->id == TOKENID_KEYWORD_INT || token->id == TOKENID_KEYWORD_STRING || token->id == TOKENID_KEYWORD_BOOL){
         if(arg_types == NULL){
            arg_types = safe_alloc(sizeof(vartype_e));
         }
         else{
            arg_types = safe_realloc(arg_types, sizeof(vartype_e)* (arg_count + 1));
         }
         arg_types[arg_count] = tokenid_to_vartype(token->id);
         arg_count++;
         printf("%d", tokenid_to_vartype(token->id));
         //add arguments to local symtable
         token_t* token_before = tokenvector_get(token_vector, i - 1);
         if(bintreestack_get_length(symtable_stack) == 0){
            bintreestack_push(symtable_stack, bintree_ctor());
            symbol = symbol_ctor(token_before->value.string_value, ST_VARIABLE, symbolval_var_ctor(tokenid_to_vartype(token->id)));
            bintree_add(bintreestack_peek(symtable_stack), symbol);
         }
         else{
            symbol = bintreestack_find(symtable_stack, token_before->value.string_value, NULL);
            if(symbol != NULL){
               return ERRCODE_GENERAL_SEMANTIC_ERROR;
            }
            else{
               symbol = symbol_ctor(token_before->value.string_value, ST_VARIABLE, symbolval_var_ctor(tokenid_to_vartype(token->id)));
               bintree_add(bintreestack_peek(symtable_stack), symbol);
            }
         }

      }
      i++;
      token = tokenvector_get(token_vector, i);

   }
   printf(" rets  ");
   //function rets handling
   i++;
   token = tokenvector_get(token_vector, i);
   if(token->id != TOKENID_LEFT_BRACKET){
      while(token->id != TOKENID_RIGHT_PARENTHESES){
         if(token->id == TOKENID_KEYWORD_FLOAT64 || token->id == TOKENID_KEYWORD_INT || token->id == TOKENID_KEYWORD_STRING || token->id == TOKENID_KEYWORD_BOOL){
            if(ret_types == NULL){
               ret_types = safe_alloc(sizeof(vartype_e));
            }
            else{
               ret_types = safe_realloc(ret_types, sizeof(vartype_e)* (arg_count + 1));
            }
            ret_types[arg_count] = tokenid_to_vartype(token->id);
            ret_count++;
            printf("%d", tokenid_to_vartype(token->id));
         }
         i++;
         token = tokenvector_get(token_vector, i);
      }
   }
   else{
      ret_types = NULL;
      ret_count = 0;
   }

   //add function declaration to symtable
   symbol = bintree_find(symtable_global, token->value.string_value);
   if(symbol == NULL){
      bintree_add(symtable_global, symbol_ctor(token->value.string_value, ST_FUNCTION, symbolval_fn_ctor(arg_count, ret_count, arg_types, ret_types, true)));
   }

   return 0;
}

int semantic_ret(tokenvector_t* token_vector, astnode_generic_t** node_ret){
   int i;
   tokenvector_t* expression;
   token_t** expressionArray;
   token_t* token;
   int size;
   (*node_ret) = astnode_ret_ctor();
   i = 1;

   printf("RET %d:\n",tokenvector_get_lenght(token_vector));
   tokenvector_print(token_vector);

   while(tokenvector_get_lenght(token_vector) != i){
      expression = tokenvector_ctor();
      token = tokenvector_get(token_vector, i);

      while(token->id != TOKENID_COMMA && tokenvector_get_lenght(token_vector) > i){
         
         tokenvector_push(expression, token);

         i++;
         if(tokenvector_get_lenght(token_vector) == i){
            break;
         }
         token = tokenvector_get(token_vector, i);
         //printf("\nvector: %d, i: %d \n",tokenvector_get_lenght(token_vector), i);
      }

      expressionArray = tokenvector_get_array(expression, &size);
      astnode_ret_add_exp((*node_ret), astnode_exp_ctor(expressionArray, size));

      tokenvector_dtor(expression);

   }

   return 0;
}

int semantic_if(tokenvector_t* token_vector, astnode_generic_t** ast_node){
   tokenvector_t* condition = tokenvector_ctor();
   token_t** condition_array;
   token_t* token;
   int i;
   int size;
   tokenvector_print(token_vector);
   i = 1; // skip token with if 
   token = tokenvector_get(token_vector, i);
   while(token->id != TOKENID_LEFT_BRACKET && tokenvector_get_lenght(token_vector) > i){
      tokenvector_push(condition, token);

      i++;
      token = tokenvector_get(token_vector, i);
      //printf("\nvector: %d, i: %d \n",tokenvector_get_lenght(token_vector), i);
   }
   tokenvector_print(condition);
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

   printf("FOR: ");
   tokenvector_print(token_vector);
   i = 1; // skip token with for
   // read def
   token = tokenvector_get(token_vector, i);
   while(token->id != TOKENID_SEMICOLON && tokenvector_get_lenght(token_vector) > i){
      tokenvector_push(def, token);

      i++;
      token = tokenvector_get(token_vector, i);
      //printf("\nvector: %d, i: %d \n",tokenvector_get_lenght(token_vector), i);
   }
   // read condition
   token = tokenvector_get(token_vector, i);
   while(token->id != TOKENID_SEMICOLON && tokenvector_get_lenght(token_vector) > i){
      tokenvector_push(condition, token);

      i++;
      token = tokenvector_get(token_vector, i);
      //printf("\nvector: %d, i: %d \n",tokenvector_get_lenght(token_vector), i);
   }

   // read assign
   token = tokenvector_get(token_vector, i);
   while(token->id != TOKENID_LEFT_BRACKET && tokenvector_get_lenght(token_vector) > i){
      tokenvector_push(assign, token);

      i++;
      token = tokenvector_get(token_vector, i);
      //printf("\nvector: %d, i: %d \n",tokenvector_get_lenght(token_vector), i);
   }
   
   if(tokenvector_get_lenght(condition) != 0){
      condition_array = tokenvector_get_array(condition, &size);
      condition_node = astnode_exp_ctor(condition_array, size);
   }

   bintreestack_push(symtable_stack, bintree_ctor());

   if(tokenvector_get_lenght(def) != 0){
      err = semantic_definition(def, symtable_stack, &def_node);
   }

   (*ast_node) = astnode_for_ctor(condition_node, def_node, assign_node);


   if(err){
      return err;
   }
   return 0;
}

int semantic(token_t* token, nonterminalid_e flag, int eol_flag){
   static astnode_generic_t* ast = NULL;
   static astnode_funcdecl_t* function;
   static bintreestack_t* symtable_stack = NULL;
   static bintree_t* symtable_global = NULL;
   static tokenvector_t* token_vector = NULL;
   static astnodestack_t* ast_parents = NULL;
   static bool function_call = false;
   static bool ast_if_body = false;
   static nonterminalid_e current_flag;
   static bool was_right_bracket = false;
   int err = 0;

   
   if(ast == NULL){
      ast = ast_ctor();
   }
   if(symtable_stack == NULL){
      symtable_stack = bintreestack_ctor();
   }
   if(symtable_global == NULL){
      symtable_global = bintree_ctor();
   }
   if(token_vector == NULL){
      token_vector = tokenvector_ctor();
   }
   if(ast_parents == NULL){
      ast_parents = astnodestack_ctor();
   }
   
   if(token->id == TOKENID_RIGHT_BRACKET){
      was_right_bracket = true;
      printf("\npop\n");
      if(bintreestack_get_length(symtable_stack) != 0)
         bintree_dtor(bintreestack_pop(symtable_stack));
      return 0;
   }

   // TODO: ignor new line
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
   
   astnode_defvar_t* ast_def = NULL;
   astnode_assign_t* ast_assign = NULL;
   astnode_generic_t* ast_node_generic = NULL;
   //ast insert

   if(was_right_bracket && (current_flag == NONTERMINAL_ELSE || current_flag == NONTERMINAL_ELSE_IF)){
      ast_if_body = false;
   }
   else{
      if(astnodestack_lenght(ast_parents) != 0){
         astnodestack_pop(ast_parents);
      }
   }

   switch(current_flag){
      // FUNCTION DECLARATION
      case NONTERMINAL_FUNCTION:
         token = tokenvector_get(token_vector, 0);
         function = astnode_funcdecl_ctor(token->value.string_value);
         ast_global_add_func(ast, function);
         err = semantic_function_decl(token_vector, symtable_stack, symtable_global);
         bintreestack_push(symtable_stack, bintree_ctor());
         

         break;
      case NONTERMINAL_DEFINITION:
         err = semantic_definition(token_vector, symtable_stack, &ast_def);
         ast_node_generic = astnode_generic_defvar_ctor(ast_def);
         if(astnodestack_lenght(ast_parents) != 0){
            if(astnodestack_peek(ast_parents)->type == ANT_IF){
               if(ast_if_body){
                  astnode_if_add_truebody(astnodestack_peek(ast_parents), ast_node_generic);
               }
               else{
                  astnode_if_add_elsebody(astnodestack_peek(ast_parents), ast_node_generic);
               }
            }
            else if(astnodestack_peek(ast_parents)->type == ANT_FOR){
               astnode_for_add_body(astnodestack_peek(ast_parents), ast_node_generic);
            }
         }
         else{
            astnode_funcdecl_add(function, ast_node_generic);
         }

         break;
      case NONTERMINAL_ASSIGNMENT:
         if(!function_call){
            err = semantic_assign(token_vector, symtable_stack, &ast_assign);
         }
         break;
      case NONTERMINAL_RETURN:
         err = semantic_ret(token_vector, &ast_node_generic);

         if(astnodestack_lenght(ast_parents) != 0){
            if(astnodestack_peek(ast_parents)->type == ANT_IF){
               if(ast_if_body){
                  astnode_if_add_truebody(astnodestack_peek(ast_parents), ast_node_generic);
               }
               else{
                  astnode_if_add_elsebody(astnodestack_peek(ast_parents), ast_node_generic);
               }
            }
            else if(astnodestack_peek(ast_parents)->type == ANT_FOR){
               astnode_for_add_body(astnodestack_peek(ast_parents), ast_node_generic);
            }
         }
         else{
            astnode_funcdecl_add(function, ast_node_generic);
         }

         break;
      case NONTERMINAL_IF:
         err = semantic_if(token_vector, &ast_node_generic);
         bintreestack_push(symtable_stack, bintree_ctor());

         if(astnodestack_lenght(ast_parents) != 0){
            if(astnodestack_peek(ast_parents)->type == ANT_IF){
               if(ast_if_body){
                  astnode_if_add_truebody(astnodestack_peek(ast_parents), ast_node_generic);
               }
               else{
                  astnode_if_add_elsebody(astnodestack_peek(ast_parents), ast_node_generic);
               }
            }
            else if(astnodestack_peek(ast_parents)->type == ANT_FOR){
               astnode_for_add_body(astnodestack_peek(ast_parents), ast_node_generic);
            }
         }
         else{
            astnode_funcdecl_add(function, ast_node_generic);
         }

         ast_if_body = true;
         break;
      case NONTERMINAL_ELSE:
         bintreestack_push(symtable_stack, bintree_ctor());
         break;
      case NONTERMINAL_ELSE_IF:
         err = semantic_if(token_vector, &ast_node_generic);
         bintreestack_push(symtable_stack, bintree_ctor());

         if(astnodestack_lenght(ast_parents) != 0){
            if(astnodestack_peek(ast_parents)->type == ANT_IF){
               if(ast_if_body){
                  astnode_if_add_truebody(astnodestack_peek(ast_parents), ast_node_generic);
               }
               else{
                  astnode_if_add_elsebody(astnodestack_peek(ast_parents), ast_node_generic);
               }
            }
            else if(astnodestack_peek(ast_parents)->type == ANT_FOR){
               astnode_for_add_body(astnodestack_peek(ast_parents), ast_node_generic);
            }
         }
         else{
            astnode_funcdecl_add(function, ast_node_generic);
         }

         ast_if_body = true;

         break;
      case NONTERMINAL_FOR:
         err = semantic_for(token_vector, symtable_stack, &ast_node_generic);
         bintreestack_push(symtable_stack, bintree_ctor());

         if(astnodestack_lenght(ast_parents) != 0){
            if(astnodestack_peek(ast_parents)->type == ANT_IF){
               if(ast_if_body){
                  astnode_if_add_truebody(astnodestack_peek(ast_parents), ast_node_generic);
               }
               else{
                  astnode_if_add_elsebody(astnodestack_peek(ast_parents), ast_node_generic);
               }
            }
            else if(astnodestack_peek(ast_parents)->type == ANT_FOR){
               astnode_for_add_body(astnodestack_peek(ast_parents), ast_node_generic);
            }
         }
         else{
            astnode_funcdecl_add(function, ast_node_generic);
         }

         break;
      default:
         break;
   }
   /*tokenvector_print(token_vector);
   printf("   %d",flag);*/
   tokenvector_dtor(token_vector);
   token_vector = NULL;
   
   if(err){
      printf("\n\nERROR\n\n");
   }

   return err;
}


