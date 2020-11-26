/**
 * IFJ20 Compiler
 * @file builtins.c
 * @authors Martin Bednář (xbedna77)
 */

#include "builtins.h"
#include "vargen.h"
#include "utils.h"

void builtin_input(char* type) {
   char* l1 = labelgen_new();
   char* l2 = labelgen_new();
   printcm("READ GF@$tmp %s", type);
   printcm("PUSHS GF@$tmp");
   printcm("JUMPIFEQ %s GF@$tmp nil@nil", l1);
   printcm("PUSHS int@0");
   printcm("JUMP %s", l2);
   printcm("LABEL %s", l1);
   printcm("PUSHS int@1");
   printcm("LABEL %s", l2);
   free(l1);
   free(l2);
}

void builtin_inputs() {
   pcomment("Built-in inputs start");
   builtin_input("string");
   pcomment("Built-in inputs end");
}

void builtin_inputi() {
   pcomment("Built-in inputi start");
   builtin_input("int");
   pcomment("Built-in inputi end");
}

void builtin_inputf() {
   pcomment("Built-in inputf start");
   builtin_input("float");
   pcomment("Built-in inputf end");
}

void builtin_print(vartable_t* vartable, token_t** params, int param_count) {
   pcomment("Built-in print start");
   for (int i = 0; i < param_count; i++) {
      if (is_const_tokenid(params[i]->id)) {
         printcm("WRITE %s", generate_const_str(params[i]));
      } else {
         char* identifier = params[i]->value.string_value;
         char* var = generate_var_str(identifier, FT_TF, vartable_depth(vartable, identifier));
         printcm("WRITE %s", var);
         free(var);
      }
   }
   pcomment("Built-in print end");
}

void builtin_int2float(vartable_t* vartable, token_t* param) {
   pcomment("Built-in int2float start");
   if (is_const_tokenid(param->id)) {
      param->value.decimal_value = (double)param->value.int_value;
      param->id = TOKENID_NUM_DECIMAL;
      printcm("PUSHS %s", generate_const_str(param));
   } else {
      char* identifier = param->value.string_value;
      char* var = generate_var_str(identifier, FT_TF, vartable_depth(vartable, identifier));
      printcm("PUSHS %s", var);
      printcm("INT2FLOATS");
      free(var);
   }
   pcomment("Built-in int2float end");
}

void builtin_float2int(vartable_t* vartable, token_t* param) {
   pcomment("Built-in float2int start");
   if (is_const_tokenid(param->id)) {
      param->value.int_value = (int)param->value.decimal_value;
      param->id = TOKENID_NUM;
      printcm("PUSHS %s", generate_const_str(param));
   } else {
      char* identifier = param->value.string_value;
      char* var = generate_var_str(identifier, FT_TF, vartable_depth(vartable, identifier));
      printcm("PUSHS %s", var);
      printcm("FLOAT2INTS");
      free(var);
   }
   pcomment("Built-in float2int end");
}

void builtin_len(vartable_t* vartable, token_t* param) {
   pcomment("Built-in len start");
   if (is_const_tokenid(param->id)) {
      printcm("PUSHS %s", strlen(param->value.string_value));
   } else {
      char* identifier = param->value.string_value;
      char* var = generate_var_str(identifier, FT_TF, vartable_depth(vartable, identifier));
      printcm("STRLEN GF@$tmp %s", var);
      printcm("PUSHS GF@$tmp");
      free(var);
   }
   pcomment("Built-in len end");
}

// TODO: ord, chr, substr
