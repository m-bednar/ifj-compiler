/**
 * IFJ20 Compiler
 * @file semantic.c
 * @authors Michal Trlica (xtrlic02)
 */

#include "semantic.h"
#include "../error.h"

int semantic(token_t* token){
   static astnode_generic_t* ast;
   if(ast == NULL){
      ast = ast_ctor();
   }
   token = token; // TODO: remove, only for compiler compliance
   return -1;
}
