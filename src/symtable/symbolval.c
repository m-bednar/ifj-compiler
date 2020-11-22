/**
 * IFJ20 Compiler
 * @file symbolval.c
 * @authors Martin Bednář (xbedna77)
 */

#include <stdlib.h>
#include "../memory.h"
#include "../error.h"
#include "symbolval.h"

symbolval_u symbolval_fn_ctor(int arg_count, int ret_count, vartype_e* arg_types, vartype_e* ret_types, bool defined) {
   guard((arg_count == 0 && arg_types == NULL) || (arg_count != 0 && arg_types != NULL));
   guard((ret_count == 0 && ret_types == NULL) || (ret_count != 0 && ret_types != NULL));
   
   symbolval_u symbolval;
   symbolvalfn_t* fn = safe_alloc(sizeof(symbolvalfn_t));
   
   fn->arg_count = arg_count;
   fn->arg_types = arg_types;
   fn->ret_count = ret_count;
   fn->ret_types = ret_types;
   fn->defined = defined;
   symbolval.fn = fn;
   
   return symbolval;
}

symbolval_u symbolval_var_ctor(vartype_e type) {
   symbolval_u symbolval;
   symbolvalvar_t* var = safe_alloc(sizeof(symbolvalvar_t));
   
   var->type = type;
   symbolval.var = var;
   
   return symbolval;
}

void symbolval_fn_dtor(symbolval_u symbolval) {
   guard(symbolval.fn != NULL);
   
   if (symbolval.fn->arg_count > 0 && symbolval.fn->arg_types != NULL) {
      free(symbolval.fn->arg_types);
      symbolval.fn->arg_types = NULL;
   }
   if (symbolval.fn->ret_count > 0 && symbolval.fn->ret_types != NULL) {
      free(symbolval.fn->ret_types);
      symbolval.fn->ret_types = NULL;
   }
   
   free(symbolval.fn);
   symbolval.fn = NULL;
}

void symbolval_var_dtor(symbolval_u symbolval) {
   guard(symbolval.var != NULL);
   free(symbolval.var);
   symbolval.var = NULL;
}
