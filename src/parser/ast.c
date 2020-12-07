/**
 * IFJ20 Compiler
 * @file ast.c
 * @authors Michal Trlica (xtrlic02)
 */

#include <string.h>
#include "ast.h"
#include "../memory.h"
#include "../error.h"

astnode_generic_t* ast_ctor() {
   astnode_generic_t* ast = safe_alloc(sizeof(astnode_generic_t));
   ast->value.globalval = safe_alloc(sizeof(astnode_global_t));
   ast->value.globalval->functions = NULL;
   ast->value.globalval->functions_count = 0;
   return ast;
}

void ast_global_add_func(astnode_generic_t* global, astnode_funcdecl_t* funcdecl){
   if(global->value.globalval->functions == NULL){
      global->value.globalval->functions = safe_alloc(sizeof(astnode_funcdecl_t*));
   }
   else{
      global->value.globalval->functions = safe_realloc(global->value.globalval->functions, sizeof(astnode_funcdecl_t*) * (global->value.globalval->functions_count + 1));
   }
   global->value.globalval->functions[global->value.globalval->functions_count] = funcdecl;
   global->value.globalval->functions_count++;
}

astnode_generic_t* astnode_if_ctor(astnode_exp_t* condition) {
   astnode_generic_t* ast_node = safe_alloc(sizeof(astnode_generic_t));
   ast_node->type = ANT_IF;
   ast_node->value.ifval = safe_alloc(sizeof(astnode_if_t));
   ast_node->value.ifval->condition = condition;
   ast_node->value.ifval->else_body = NULL;
   ast_node->value.ifval->true_body = NULL;
   return ast_node;
}

astnode_codeblock_t* astnode_codeblock_ctor(){
   astnode_codeblock_t* ast_node = safe_alloc(sizeof(astnode_codeblock_t));
   ast_node->children_count = 0;
   ast_node->children = NULL;
   return ast_node;
}

void astnode_codeblock_insert(astnode_codeblock_t* codeblock, astnode_generic_t* node) {
   if(codeblock == NULL || node == NULL){
      return;
   }
   if(codeblock->children == NULL){
      codeblock->children = safe_alloc(sizeof(astnode_codeblock_t*));
   }
   codeblock->children = safe_realloc(codeblock->children ,sizeof(astnode_generic_t*) * (codeblock->children_count + 1));
   codeblock->children_count++;
   codeblock->children[codeblock->children_count-1] = node;
}

astnode_generic_t* astnode_for_ctor(astnode_exp_t* condition,  astnode_defvar_t* defvar, astnode_assign_t* assign) {
   astnode_generic_t* ast_node = safe_alloc(sizeof(astnode_generic_t));
   ast_node->type = ANT_FOR;
   ast_node->value.forval = safe_alloc(sizeof(astnode_for_t));
   ast_node->value.forval->body = NULL;
   ast_node->value.forval->condition = condition;
   ast_node->value.forval->defvar = defvar;
   ast_node->value.forval->assign = assign;
   return ast_node;
}

void astnode_for_add_body(astnode_generic_t* ast_node, astnode_generic_t* new_node){
   if(ast_node->value.forval->body == NULL){
      ast_node->value.forval->body = astnode_codeblock_ctor();
   }
   astnode_codeblock_insert(ast_node->value.forval->body, new_node);
}

astnode_exp_t* astnode_exp_ctor(token_t** tokens, int token_count) {
   astnode_exp_t* ast_node = safe_alloc(sizeof(astnode_exp_t));
   ast_node->tokens = tokens;
   ast_node->tokens_count = token_count;
   return ast_node;
}

astnode_generic_t* astnode_ret_ctor() {
   astnode_generic_t* ast_node = safe_alloc(sizeof(astnode_generic_t));
   ast_node->type = ANT_RET;
   ast_node->value.returnval = safe_alloc(sizeof(astnode_ret_t));
   ast_node->value.returnval->expressions = NULL;
   ast_node->value.returnval->expressions_count = 0;
   return ast_node;
}

void astnode_ret_add_exp(astnode_generic_t* ret, astnode_exp_t* exp){
   if(ret->value.returnval->expressions == NULL){
      ret->value.returnval->expressions = safe_alloc(sizeof(astnode_exp_t));
   }
   else{
      ret->value.returnval->expressions = safe_realloc(ret->value.returnval->expressions, sizeof(astnode_exp_t) * (ret->value.returnval->expressions_count + 1));
   }
   ret->value.returnval->expressions[ret->value.returnval->expressions_count] = exp;
   ret->value.returnval->expressions_count++;
}

astnode_funcdecl_t* astnode_funcdecl_ctor(char* name) {
   astnode_funcdecl_t* ast_node = safe_alloc(sizeof(astnode_generic_t));
   ast_node->name = safe_alloc(sizeof(char) * (strlen(name) + 1));
   strcpy(ast_node->name, name);
   ast_node->body = astnode_codeblock_ctor();
   return ast_node;
}

void astnode_funcdecl_add(astnode_funcdecl_t* func, astnode_generic_t* child){
   guard(child != NULL);
   astnode_codeblock_insert(func->body, child);
}

astnode_defvar_t* astnode_defvar_ctor(token_t* variable, astnode_exp_t* expression){
   astnode_defvar_t* ast_node = safe_alloc(sizeof(astnode_defvar_t));
   ast_node->expression = expression;
   ast_node->variable = variable;
   return ast_node;
}

astnode_generic_t* astnode_generic_defvar_ctor(astnode_defvar_t* def){
   astnode_generic_t* ast_node = safe_alloc(sizeof(astnode_generic_t));
   ast_node->type = ANT_DEFVAR;
   ast_node->value.defvarval = def;
   return ast_node;
}

void astnode_if_add_truebody(astnode_generic_t* ast_node, astnode_generic_t* new_node){
   if(ast_node->value.ifval->true_body == NULL){
      ast_node->value.ifval->true_body = astnode_codeblock_ctor();
   }
   astnode_codeblock_insert(ast_node->value.ifval->true_body, new_node);
}

void astnode_if_add_elsebody(astnode_generic_t* ast_node, astnode_generic_t* new_node){
   if(ast_node->value.ifval->else_body == NULL){
      ast_node->value.ifval->else_body = astnode_codeblock_ctor();
   }
   astnode_codeblock_insert(ast_node->value.ifval->else_body, new_node);
}

astnode_assign_t* astnode_assign_ctor(){
   astnode_assign_t* ast_node = safe_alloc(sizeof(astnode_assign_t));
   ast_node->expressions_count = 0;
   ast_node->ids_count = 0;
   ast_node->left_ids = NULL;
   ast_node->right_expressions = NULL;
   ast_node->right_function = NULL;
   return ast_node;
}

astnode_generic_t* astnode_generic_assign_ctor(astnode_assign_t* assign){
   astnode_generic_t* ast_node = safe_alloc(sizeof(astnode_generic_t));
   ast_node->type = ANT_ASSIGN;
   ast_node->value.assignval = assign;
   return ast_node;
}


void astnode_assign_add_ids(astnode_assign_t* ast_node, token_t** ids, int ids_count){
   ast_node->left_ids = ids;
   ast_node->ids_count = ids_count;
}

void astnode_assign_add_exp(astnode_assign_t* ast_node, astnode_exp_t* exp){
   if(ast_node->right_expressions == NULL){
      ast_node->right_expressions = safe_alloc(sizeof(astnode_exp_t*));
   }
   else{
      ast_node->right_expressions = safe_realloc(ast_node->right_expressions, sizeof(astnode_exp_t*) * (ast_node->expressions_count + 1));
   }
   ast_node->right_expressions[ast_node->expressions_count] = exp;
   ast_node->expressions_count++;
}

void astnode_assign_add_funccall(astnode_assign_t* ast_node, astnode_funccall_t* funccall){
   ast_node->right_function = funccall;
}


astnode_funccall_t* astnode_funccall_ctor(char* name, token_t** params, int params_count){
   astnode_funccall_t* ast_node = safe_alloc(sizeof(astnode_funccall_t));
   ast_node->name = safe_alloc(sizeof(char) * (strlen(name) + 1));
   strcpy(ast_node->name, name);
   ast_node->params = params;
   ast_node->params_count = params_count;
   return ast_node;
}

astnode_generic_t* astnode_generic_funccall_ctor(astnode_funccall_t* funccall){
   astnode_generic_t* ast_node = safe_alloc(sizeof(astnode_generic_t));
   ast_node->type = ANT_FUNCCALL;
   ast_node->value.funccallval = funccall;
   return ast_node;
}
