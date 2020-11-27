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
         return VT_BOOL;
      break;
      case TOKENID_KEYWORD_INT:
         return VT_INT;
      break;
      case TOKENID_KEYWORD_STRING:
         return VT_FLOAT;
      break;
      case TOKENID_KEYWORD_BOOL:
         return VT_STRING;
      break;
      default:
         return 0;
   }
}
/*
 * returns true if type1 and type2 are same or if type1 is NULL
 * if type1 is -1 then type1 = type2
 */

bool check_same_type(vartype_e* type1, vartype_e type2){
   if(type1 == NULL){
      (*type1) = type2;
      return true;
   }
   if((*type1) == type2){
      return true;
   }
   return false;
}


int semantic_expression(tokenstack_t* stack, vartype_e* type, bintreestack_t* symtable_stack){
   token_t* token_current = NULL;
   token_t* token_last;
   symbol_t* symbol;

   type = NULL;
   while(tokenstack_get_lenght(stack) != 0){
      if(token_current != NULL){
         token_last = token_current;
         token_last = token_last; // TODO: remove, only for compiler compliance
      }
      token_current = tokenstack_pop(stack);
      
      switch(token_current->id){
         // variable in expresion
         case TOKENID_IDENTIFIER:
            symbol = bintreestack_find(symtable_stack, token_current->value.string_value, NULL);
            if(symbol == NULL){
               return ERRCODE_VAR_UNDEFINED_ERROR;
            }
            else{
               if(!check_same_type(type, symbol->value.var->type)){
                  return ERRCODE_TYPE_INCOMPATIBLE_ERROR;
               }
            }
         break;
         // int const
         case TOKENID_NUM:
            if(!check_same_type(type, VT_INT)){
                  return ERRCODE_TYPE_INCOMPATIBLE_ERROR;
            }
         break;
         // float const
         case TOKENID_NUM_DECIMAL:
            if(!check_same_type(type, VT_FLOAT)){
                  return ERRCODE_TYPE_INCOMPATIBLE_ERROR;
            }
         break;
         default:
         break;
      }
      //TODO: free token
   }
   return -1;
}


int semantic_definition(tokenvector_t* vector, bintreestack_t* symtable_stack){
   tokenvector_t* expresion = tokenvector_ctor();
   token_t* token = token; //TODO: compilator  err, delete
   vartype_e type = 0;
   symbol_t* new_symbol;
   symbol_t* symbol;
   int err = 0;
   int level;

   do{
      //token = tokenstack_pop(stack);
      if(token->id != TOKENID_OPERATOR_DECLARE){
         tokenvector_push(expresion, token);
      }
   }while(token->id != TOKENID_OPERATOR_DECLARE);

   //err = semantic_expression(expresion, &type, symtable_stack);
   //tokenstack_dtor(expresion);


   token = tokenvector_get(vector, 0); // pop identifier
   //TODO: dtor stack
   if(err != -1){
      return err; //error occured in expression
   }

   symbol = bintreestack_find(symtable_stack, token->value.string_value, &level);
   if(level == (bintreestack_get_length(symtable_stack) - 1) && symbol != NULL){
      token_dtor(token);
      return ERRCODE_VAR_UNDEFINED_ERROR; //variable was already declared in this scope 
   }

   new_symbol = symbol_ctor(token->value.string_value, ST_VARIABLE, symbolval_var_ctor(type));
   bintree_add(bintreestack_peek(symtable_stack), new_symbol);
   token_dtor(token);
   return -1;
}


int semantic_assign(tokenstack_t* stack, bintreestack_t* symtable_stack){
   tokenstack_t* expression_stack = tokenstack_ctor();
   vartype_e* expression_types = NULL; //vartype array
   int expression_types_size = 0;
   token_t* token;
   int err;
   vartype_e type;
   symbol_t* symbol;
   //checking expressions
   do{

      do{
         token = tokenstack_pop(stack);
         if(token->id != TOKENID_COMMA && token->id != TOKENID_OPERATOR_ASSIGN){
            tokenstack_push(expression_stack, token);
         }
      }while(token->id != TOKENID_COMMA && token->id != TOKENID_OPERATOR_ASSIGN);
      
      // dtor token with comma
      if(token->id == TOKENID_COMMA){
         token_dtor(token);
      }

      err = semantic_expression(expression_stack, &type, symtable_stack);
      tokenstack_dtor(expression_stack);

      if(err != -1){
         return err; //error occured in expression
      }
      
      //realloc expression_types array
      if(expression_types == NULL){
         expression_types = safe_alloc(sizeof(vartype_e));
      }
      else{
         expression_types = safe_realloc(expression_types, sizeof(vartype_e) * (expression_types_size+1));
      }
      expression_types_size++;

      expression_types[expression_types_size-1] = type;

   }while(token->id != TOKENID_OPERATOR_ASSIGN);

   token_dtor(token);

   int i = 0;
   while(tokenstack_get_lenght(stack) != 0){
      token = tokenstack_pop(stack);

      if(!strcmp(token->value.string_value, "_")){
         i++;
      }
      else if(token->id != TOKENID_COMMA){
         symbol = bintreestack_find(symtable_stack, token->value.string_value, NULL);
         if(symbol == NULL){
            // TODO: dtor
            return ERRCODE_VAR_UNDEFINED_ERROR; // 
         }
         else if(symbol->value.var->type != expression_types[i]){
            // TODO: dtor
            return ERRCODE_GENERAL_SEMANTIC_ERROR;
         }
         i++;
      }

      token_dtor(token);
   }



   return -1;
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
         arg_types[arg_count] = tokenid_to_vartype(tokenid_to_vartype(token->id));
         arg_count++;

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


   }

   //function rets handling
   i++;
   token = tokenvector_get(token_vector, i);
   while(token->id != TOKENID_RIGHT_PARENTHESES){
      if(token->id == TOKENID_KEYWORD_FLOAT64 || token->id == TOKENID_KEYWORD_INT || token->id == TOKENID_KEYWORD_STRING || token->id == TOKENID_KEYWORD_BOOL){
         if(ret_types == NULL){
            ret_types = safe_alloc(sizeof(vartype_e));
         }
         else{
            ret_types = safe_realloc(ret_types, sizeof(vartype_e)* (arg_count + 1));
         }
         ret_types[arg_count] = tokenid_to_vartype(tokenid_to_vartype(token->id));
         ret_count++;
      }
      i++;
   }


   //add function declaration to symtable
   symbol = bintree_find(symtable_global, token->value.string_value);
   if(symbol == NULL){
      bintree_add(symtable_global, symbol_ctor(token->value.string_value, ST_FUNCTION, symbolval_fn_ctor(arg_count, ret_count, arg_types, ret_types, true)));
   }

   return 0;
}

int semantic_ret(tokenvector_t* token_vector, astnode_funcdecl_t* function, astnodestack_t* astnode_parents){
   int i;
   tokenvector_t* expression;
   token_t** expressionArray;
   token_t* token;
   int size;
   astnode_generic_t* node_ret = astnode_ret_ctor();
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
      astnode_ret_add_exp(node_ret, astnode_exp_ctor(expressionArray, size));

      tokenvector_dtor(expression);

   }

   //add to ast
   if(astnodestack_lenght(astnode_parents) == 0){
      astnode_funcdecl_add(function, node_ret);
   }
   

   return 0;
}

int semantic_if(tokenvector_t* token_vector, astnode_funcdecl_t* function, astnodestack_t* astnode_parents){
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

   if(astnodestack_lenght(astnode_parents) == 0){
      astnode_funcdecl_add(function, astnode_if_ctor(astnode_exp_ctor(condition_array, size)));
   }

   return 0;
}

int semantic_for(tokenvector_t* token_vector, astnode_funcdecl_t* function, astnodestack_t* astnode_parents){
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

   if(astnodestack_lenght(astnode_parents) == 0){
      astnode_funcdecl_add(function, astnode_for_ctor(condition_node, def_node, assign_node));
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
   int err = 0;
   

   flag = flag;
   token = token;
   eol_flag=eol_flag;

   
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
      printf("\npop\n");
      if(bintreestack_get_length(symtable_stack) != 0)
         bintree_dtor(bintreestack_pop(symtable_stack));
      return 0;
   }

   // TODO: ignor new line
   if(!eol_flag){
      tokenvector_push(token_vector, token);
      return 0; //whole line wasnt read yet
   }
   
   if(tokenvector_get_lenght(token_vector) == 0){
      tokenvector_dtor(token_vector);
      token_vector = NULL;
      return 0;
   }
   

   //ast insert
   switch(flag){
      // FUNCTION DECLARATION
      case NONTERMINAL_FUNCTION:
         token = tokenvector_get(token_vector, 0);
         function = astnode_funcdecl_ctor(token->value.string_value);
         ast_global_add_func(ast, function);
         //err = semantic_function_decl(token_vector, symtable_stack, symtable_global);

         break;
      case NONTERMINAL_DEFINITION:
         //err = semantic_definition();

         break;
      case NONTERMINAL_ASSIGNMENT:

         break;
      case NONTERMINAL_RETURN:
         err = semantic_ret(token_vector, function, ast_parents);

         break;
      case NONTERMINAL_IF:
         err = semantic_if(token_vector, function, ast_parents);
         break;
      case NONTERMINAL_ELSE:
         break;
      case NONTERMINAL_ELSE_IF:
         break;
      case NONTERMINAL_FOR:
         err = semantic_for(token_vector, function, ast_parents);
         break;
      default:
         break;
   }
   /*tokenvector_print(token_vector);
   printf("   %d",flag);*/
   tokenvector_dtor(token_vector);
   token_vector = NULL;
   
   return err;
}


