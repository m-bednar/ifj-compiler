/**
 * IFJ20 Compiler
 * @file semantic.c
 * @authors Michal Trlica (xtrlic02)
 */

#include "semantic.h"
#include "../error.h"

int semantic(token_t* token){
   static astnode_t* ast;
   if(ast == NULL){
      ast = ast_ctor();
   }
   return -1;
}
