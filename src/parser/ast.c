/**
 * IFJ20 Compiler
 * @file ast.c
 * @authors Michal Trlica (xtrlic02)
 */

#include "ast.h"
#include "../memory.h"

astnode_t* ast_ctor(){
   astnode_t* ast = safe_alloc(sizeof(astnode_t));
   ast->type = TYPE_GLOBAL;
   ast->value.globalval.functions = NULL;
   return ast;
}
