/**
 * IFJ20 Compiler
 * @file ast.c
 * @authors Michal Trlica (xtrlic02)
 */

#include "ast.h"
#include "../memory.h"

astnode_generic_t* ast_ctor() {
   astnode_generic_t* ast = safe_alloc(sizeof(astnode_generic_t));
   ast->type = ANT_GLOBAL;
   ast->value.globalval.functions = NULL;
   return ast;
}

astnode_generic_t* astnode_if_ctor() {
   astnode_generic_t* ast_node = safe_alloc(sizeof(astnode_generic_t));
   ast_node->type = ANT_IF;
   ast_node->value.ifval.condition = NULL;
   ast_node->value.ifval.else_body = NULL;
   ast_node->value.ifval.true_body = NULL;
   return ast_node;
}

astnode_generic_t* astnode_for_ctor() {
   astnode_generic_t* ast_node = safe_alloc(sizeof(astnode_generic_t));
   ast_node->type = ANT_FOR;
   ast_node->value.forval.body = NULL;
   ast_node->value.forval.condition = NULL;
   return ast_node;
}

astnode_generic_t* astnode_exp_ctor() {
   astnode_generic_t* ast_node = safe_alloc(sizeof(astnode_generic_t));
   ast_node->type = ANT_EXP;
   ast_node->value.expval.tokens = NULL;
   return ast_node;
}

astnode_generic_t* astnode_ret_ctor() {
   astnode_generic_t* ast_node = safe_alloc(sizeof(astnode_generic_t));
   ast_node->type = ANT_RET;
   ast_node->value.returnval.expressions = NULL;
   return ast_node;
}

astnode_generic_t* astnode_funcdecl_ctor() {
   astnode_generic_t* ast_node = safe_alloc(sizeof(astnode_generic_t));
   ast_node->type = ANT_FUNCDECL;
   ast_node->value.funcdeclval.body = NULL;
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
