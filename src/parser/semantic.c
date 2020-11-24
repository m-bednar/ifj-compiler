/**
 * IFJ20 Compiler
 * @file semantic.c
 * @authors Michal Trlica (xtrlic02)
 */

#include "semantic.h"
#include "tokenstack.h"
#include "../error.h"
#include "../symtable/bintreestack.h"
#include <stdbool.h>

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

int semantic_declare(tokenstack_t* stack, bintreestack_t* symtable_stack){
   tokenstack_t* expresion_stack = tokenstack_ctor();
   token_t* token;
   vartype_e type;
   symbol_t* new_symbol;
   symbol_t* symbol;
   int err;
   int level;

   do{
      token = tokenstack_pop(stack);
      if(token->id != TOKENID_OPERATOR_DECLARE){
         tokenstack_push(expresion_stack, token);
      }
   }while(token->id != TOKENID_OPERATOR_DECLARE);

   err = semantic_expression(expresion_stack, &type, symtable_stack);
   tokenstack_dtor(expresion_stack);


   token = tokenstack_pop(stack); // pop identifier
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

   do{
      token = tokenstack_pop(stack);
      if(token->id != TOKENID_COMMA){
         tokenstack_push(expression_stack, token);
      }
   }while(token->id != TOKENID_COMMA);

   err = semantic_expression(expression_stack, &type, symtable_stack);
   tokenstack_dtor(expression_stack);

   if(err != -1){
      return err; //error occured in expression
   }
   
   if(expression_types == NULL){
      expression_types = safe_alloc(sizeof(vartype_e));
   }
   else{
      expression_types = safe_realloc(expression_types, sizeof(vartype_e) * (expression_types_size+1));
   }
   expression_types_size++;

   expression_types[expression_types_size-1] = type;

   return -1;
}

int semantic(token_t* token, bool eol_flag){
   static astnode_generic_t* ast;
   static bintreestack_t* symtable_stack;
   static bintree_t* symtable_global;
   static tokenstack_t* token_stack;

   if(ast == NULL){
      ast = ast_ctor();
   }
   if(symtable_stack == NULL){
      symtable_stack = bintreestack_ctor();
   }
   if(symtable_global == NULL){
      symtable_global = bintree_ctor();
   }
   if(token_stack == NULL){
      token_stack = tokenstack_ctor();
   }

   while(!eol_flag){
      tokenstack_push(token_stack, token);
   }
   
   if(!eol_flag){
      return -1; //whole line wasnt read yet
   }

   // TODO: ast insert


   // TODO: call semantic functions

   tokenstack_dtor(token_stack);

   return -1;
}
