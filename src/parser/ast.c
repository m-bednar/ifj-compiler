/**
 * IFJ20 Compiler
 * @file ast.c
 * @authors Michal Trlica (xtrlic02)
 */

#include "ast.h"
#include "../memory.h"

astnode_t* ast_ctor(){
   astnode_t* ast = safe_alloc(sizeof(astnode_t));
   ast->parent = NULL;
   ast->type = ANT_GLOBAL;
   ast->value.globalval.functions = NULL;
   return ast;
}

astnode_t* astnode_if_ctor(){
   astnode_t* ast_node = safe_alloc(sizeof(astnode_t));
   ast_node->parent = NULL;
   ast_node->type = ANT_IF;
   ast_node->value.ifval.condition = NULL;
   ast_node->value.ifval.else_body = NULL;
   ast_node->value.ifval.true_body = NULL;
   return ast_node;
}

astnode_t* astnode_for_ctor(){
   astnode_t* ast_node = safe_alloc(sizeof(astnode_t));
   ast_node->parent = NULL;
   ast_node->type = ANT_FOR;
   ast_node->value.forval.body = NULL;
   ast_node->value.forval.condition = NULL;
   return ast_node;
}

astnode_t* astnode_exp_ctor(){
   astnode_t* ast_node = safe_alloc(sizeof(astnode_t));
   ast_node->parent = NULL;
   ast_node->type = ANT_EXP;
   ast_node->value.expval.postfix = NULL;
   return ast_node;
}

astnode_t* astnode_ret_ctor(){
   astnode_t* ast_node = safe_alloc(sizeof(astnode_t));
   ast_node->parent = NULL;
   ast_node->type = ANT_RET;
   ast_node->value.returnval.expresion = NULL;
   return ast_node;
}

astnode_t* astnode_funcdec_ctor(){
   astnode_t* ast_node = safe_alloc(sizeof(astnode_t));
   ast_node->parent = NULL;
   ast_node->type = ANT_FUNCDEC;
   ast_node->value.funcdecval.body = NULL;
   ast_node->value.funcdecval.return_val = NULL;
   return ast_node;
}
