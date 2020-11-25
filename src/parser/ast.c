/**
 * IFJ20 Compiler
 * @file ast.c
 * @authors Michal Trlica (xtrlic02)
 */

#include "ast.h"
#include "../memory.h"

astnode_generic_t* ast_ctor() {
   astnode_generic_t* ast = safe_alloc(sizeof(astnode_generic_t));
   // ast->type = ANT_GLOBAL;
   ast->value.globalval->functions = NULL;
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

astnode_generic_t* astnode_if_ctor() {
   astnode_generic_t* ast_node = safe_alloc(sizeof(astnode_generic_t));
   ast_node->type = ANT_IF;
   ast_node->value.ifval->condition = NULL;
   ast_node->value.ifval->else_body = NULL;
   ast_node->value.ifval->true_body = NULL;
   return ast_node;
}

astnode_generic_t* astnode_for_ctor() {
   astnode_generic_t* ast_node = safe_alloc(sizeof(astnode_generic_t));
   ast_node->type = ANT_FOR;
   ast_node->value.forval->body = NULL;
   ast_node->value.forval->condition = NULL;
   return ast_node;
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
   ast_node->value.returnval->expressions = NULL;
   return ast_node;
}

astnode_funcdecl_t* astnode_funcdecl_ctor(char* name) {
   astnode_funcdecl_t* ast_node = safe_alloc(sizeof(astnode_generic_t));
   ast_node->name = name;
   return ast_node;
}

void astnode_codeblock_insert(astnode_codeblock_t* codeblock, astnode_generic_t* node) {
   if(codeblock == NULL || node == NULL){
      return;
   }
   codeblock->children = safe_alloc(sizeof(astnode_generic_t) * (codeblock->children_count + 1));
   codeblock->children_count++;
   codeblock->children[codeblock->children_count-1] = node;
}

astnode_generic_t* astnode_defvar_ctor(token_t* variable, astnode_exp_t* expression){
   astnode_generic_t* ast_node = safe_alloc(sizeof(astnode_generic_t));
   ast_node->type = ANT_DEFVAR;
   ast_node->value.defvarval->expression = expression;
   ast_node->value.defvarval->variable = variable;
   return ast_node;
}
