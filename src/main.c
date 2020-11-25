/**
 * IFJ20 Compiler
 * @file main.c
 * @authors Martin Bednář (xbedna77)
 */

#include <stdio.h>
#include <stdlib.h>

#include "generator/utils.h"
#include "generator/generator.h"

void global_function_push(astnode_global_t* global, astnode_funcdecl_t* func) {
   global->functions = safe_realloc(global->functions, (global->functions_count + 1) * sizeof(astnode_funcdecl_t*));
   global->functions[global->functions_count] = func;
   global->functions_count++;
}

astnode_global_t* new_global() {
   astnode_global_t* global = safe_alloc(sizeof(astnode_global_t));
   global->functions = NULL;
   global->functions_count = 0;
   return global;
}

astnode_funcdecl_t* new_funcdecl(char* name) {
   astnode_funcdecl_t* func = safe_alloc(sizeof(astnode_funcdecl_t));
   func->name = safe_alloc(strlen(name) + 1);
   strcpy(func->name, name);
   func->body = safe_alloc(sizeof(astnode_codeblock_t));
   return func;
}

int main() {
   astnode_global_t* global = new_global();

   astnode_funcdecl_t* fn1 = new_funcdecl("main");
   astnode_funcdecl_t* fn2 = new_funcdecl("myfunction");

   global_function_push(global, fn1);
   global_function_push(global, fn2);

   fn1->body->children_count = 2;
   fn1->body->children = safe_alloc(sizeof(astnode_generic_t*) * 2);
   fn1->body->children[0] = safe_alloc(sizeof(astnode_generic_t));
   fn1->body->children[0]->type = ANT_DEFVAR;
   fn1->body->children[0]->value.defvarval = safe_alloc(sizeof(astnode_defvar_t));
   token_value_u value;
   value.string_value = safe_alloc(6);
   strcpy(value.string_value, "abc");
   fn1->body->children[0]->value.defvarval->variable = token_ctor(TOKENID_IDENTIFIER, value);
   fn1->body->children[0]->value.defvarval->expression = safe_alloc(sizeof(astnode_exp_t));
   fn1->body->children[0]->value.defvarval->expression->tokens_count = 5;
   fn1->body->children[0]->value.defvarval->expression->tokens = safe_alloc(sizeof(token_t*) * 5);
   // 3 + 4 * 2   =>   3 4 2 * +   ==   11
   value.decimal_value = 5;
   fn1->body->children[0]->value.defvarval->expression->tokens[0] = token_ctor(TOKENID_NUM, value);
   fn1->body->children[0]->value.defvarval->expression->tokens[0]->value.int_value = 2;

   fn1->body->children[0]->value.defvarval->expression->tokens[1] = token_ctor(TOKENID_OPERATOR_ADD, value);

   
   fn1->body->children[0]->value.defvarval->expression->tokens[2] = token_ctor(TOKENID_NUM, value);
   fn1->body->children[0]->value.defvarval->expression->tokens[2]->value.int_value = 3;

   fn1->body->children[0]->value.defvarval->expression->tokens[3] = token_ctor(TOKENID_OPERATOR_DIV, value);

   fn1->body->children[0]->value.defvarval->expression->tokens[4] = token_ctor(TOKENID_NUM, value);
   fn1->body->children[0]->value.defvarval->expression->tokens[4]->value.int_value = 4;

   char* names[] = { "x", "y" };
   vartype_e types[] = { VT_INT, VT_INT };
   
   fn1->body->children[1] = safe_alloc(sizeof(astnode_funccall_t));
   fn1->body->children[1]->type = ANT_FUNCCALL;
   fn1->body->children[1]->value.funccallval = safe_alloc(sizeof(astnode_funccall_t));
   fn1->body->children[1]->value.funccallval->name = safe_alloc(strlen("myfunction") + 1);
   strcpy(fn1->body->children[1]->value.funccallval->name, "myfunction");
   fn1->body->children[1]->value.funccallval->params = safe_alloc(2 * sizeof(token_t*));
   value.int_value = 5;
   fn1->body->children[1]->value.funccallval->params[0] = token_ctor(TOKENID_NUM, value);
   value.bool_value = false;
   fn1->body->children[1]->value.funccallval->params[1] = token_ctor(TOKENID_BOOL_LITERAL, value);
   fn1->body->children[1]->value.funccallval->params_count = 2;

   bintree_t* fntable = bintree_ctor();
   bintree_add(fntable, symbol_ctor("myfunction", ST_FUNCTION, symbolval_fn_ctor(2, 0, names, types, NULL, true)));

   fn2->body->children_count = 0;
   
   generate(global, fntable);
   // bintree_dtor(fntable); TODO: Uncomment 

   return EXIT_SUCCESS;
}
