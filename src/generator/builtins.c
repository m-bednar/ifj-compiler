/**
 * IFJ20 Compiler
 * @file builtins.c
 * @authors Martin Bednář (xbedna77)
 */

#include "builtins.h"
#include "vargen.h"
#include "utils.h"

void builtin_input(char* type) {
   printcm("READ GF@$tmp %s", type);
   printcm("PUSHS GF@$tmp");
   printcm("JUMPIFEQ %s GF@$tmp nil@nil", "label_0"); // TODO: replace labels
   printcm("PUSHS int@0");
   printcm("JUMP %s", "label_1");
   printcm("LABEL %s", "label_0");
   printcm("PUSHS int@1");
   printcm("LABEL %s", "label_1");
}

void builtin_inputs() {
   builtin_input("string");
}

void builtin_inputi() {
   builtin_input("int");
}

void builtin_inputf() {
   builtin_input("float");
}

void builtin_print(vartable_t* vartable, token_t** params, int param_count) {
   for (int i = 0; i < param_count; i++) {
      if (is_const_tokenid(params[i]->id)) {
         printcm("WRITE %s", generate_const_str(params[i]));
      } else {
         char* identifier = params[i]->value.string_value;
         int depth = vartable_depth(vartable, identifier);
         printcm("WRITE %s", generate_var_str(identifier, FT_TF, depth));
      }
   }
}

void builtin_int2float(vartable_t* vartable, token_t* param) {
   if (is_const_tokenid(param->id)) {
      param->value.decimal_value = (double)param->value.int_value;
      param->id = TOKENID_NUM_DECIMAL;
      printcm("PUSHS %s", generate_const_str(param));
   } else {
      char* identifier = param->value.string_value;
      int depth = vartable_depth(vartable, identifier);
      printcm("PUSHS %s", generate_var_str(identifier, FT_TF, depth));
      printcm("INT2FLOATS");
   }
}

void builtin_float2int(vartable_t* vartable, token_t* param) {
   if (is_const_tokenid(param->id)) {
      param->value.int_value = (int)param->value.decimal_value;
      param->id = TOKENID_NUM;
      printcm("PUSHS %s", generate_const_str(param));
   } else {
      char* identifier = param->value.string_value;
      int depth = vartable_depth(vartable, identifier);
      printcm("PUSHS %s", generate_var_str(identifier, FT_TF, depth));
      printcm("FLOAT2INTS");
   }
}

void builtin_len(vartable_t* vartable, token_t* param) {
   if (is_const_tokenid(param->id)) {
      printcm("PUSHS %s", strlen(param->value.string_value));
   } else {
      char* identifier = param->value.string_value;
      int depth = vartable_depth(vartable, identifier);
      printcm("STRLEN GF@$tmp %s", generate_var_str(identifier, FT_TF, depth));
      printcm("PUSHS GF@$tmp");
   }
}

// TODO: ord, chr, substr
