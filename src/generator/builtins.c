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
   printcm("READ GF@$tmp0 %s", type);
   printcm("PUSHS GF@$tmp0");
   printcm("TYPE GF@$tmp1 GF@$tmp0");
   printcm("JUMPIFEQ %s GF@$tmp1 string@nil", l1);
   printcm("PUSHS int@0");
   printcm("JUMP %s", l2);
   printcm("LABEL %s", l1);
   printcm("PUSHS int@1");
   printcm("LABEL %s", l2);
   free(l1);
   free(l2);
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
         char* var = generate_var_str(identifier, FT_TF, vartable_depth(vartable, identifier));
         printcm("WRITE %s", var);
         free(var);
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
      char* var = generate_var_str(identifier, FT_TF, vartable_depth(vartable, identifier));
      printcm("PUSHS %s", var);
      printcm("INT2FLOATS");
      free(var);
   }
}

void builtin_float2int(vartable_t* vartable, token_t* param) {
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
}

void builtin_len(vartable_t* vartable, token_t* param) {
   if (is_const_tokenid(param->id)) {
      printcm("PUSHS int@%d", (int)strlen(param->value.string_value));
   } else {
      char* identifier = param->value.string_value;
      char* var = generate_var_str(identifier, FT_TF, vartable_depth(vartable, identifier));
      printcm("STRLEN GF@$tmp0 %s", var);
      printcm("PUSHS GF@$tmp0");
      free(var);
   }
}

void builtin_chr(vartable_t* vartable, token_t* param) {
   if (is_const_tokenid(param->id)) {
      int value = param->value.int_value;
      if (value < 0 || value > 255) {
         printcm("PUSHS string@");
         printcm("PUSHS int@1");
      } else {
         char str[] = { (char)value, '\0' };
         char* converted = convert_string(str);
         printcm("PUSHS string@%s", converted);
         printcm("PUSHS int@0");
         free(converted);
      }
   } else {
      char* identifier = param->value.string_value;
      char* var = generate_var_str(identifier, FT_TF, vartable_depth(vartable, identifier));
      char* l1 = labelgen_new();
      char* l2 = labelgen_new();
      printcm("PUSHS string@");
      printcm("PUSHS %s", var);
      printcm("LTS");
      printcm("PUSHS %s", var);
      printcm("PUSHS int@255");
      printcm("GTS");
      printcm("ORS");
      printcm("PUSHS bool@true");
      printcm("JUMPIFEQS %s", l1);
      printcm("PUSHS %s", var);
      printcm("INT2CHARS");
      printcm("PUSHS int@0");
      printcm("JUMP %s", l2);
      printcm("LABEL %s", l1);
      printcm("PUSHS string@");
      printcm("PUSHS int@1");
      printcm("LABEL %s", l2);
   }
   pcomment("Built-in len end");
}

void builtin_ord(vartable_t* vartable, token_t** params) {
   if (is_const_tokenid(params[0]->id) && is_const_tokenid(params[1]->id)) {
      char* s = params[0]->value.string_value;
      int i = params[1]->value.int_value;
      if (i >= (int)strlen(s) || i < 0) {
         printcm("PUSHS int@0");
         printcm("PUSHS int@1");
      } else {
         printcm("PUSHS int@%d", (int)s[i]);
         printcm("PUSHS int@0");
      }
   } else if (is_const_tokenid(params[1]->id)) {
      if (params[1]->value.int_value < 0) {
         printcm("PUSHS int@0");
         printcm("PUSHS int@1");
         return;
      } else {
         char* l1 = labelgen_new();
         char* l2 = labelgen_new();
         char* identifier = params[0]->value.string_value;
         char* var = generate_var_str(identifier, FT_TF, vartable_depth(vartable, identifier));
         printcm("STRLEN GF@$tmp0 %s", var);
         printcm("LT GF@$tmp0 GF@$tmp0 int@%d");
         printcm("JUMPIFEQ GF@$tmp0 GF@$false %s", l1);
         printcm("STRI2INT GF@$tmp0 %s int@%d", var, params[1]->value.int_value);
         printcm("PUSHS GF@$tmp0");
         printcm("PUSHS int@0");
         printcm("JUMP %s", l2);
         printcm("LABEL %s", l1);
         printcm("PUSHS string@");
         printcm("PUSHS int@1");
         printcm("LABEL %s", l2);
         free(l1);
         free(l2);
         free(var);
      }
   } else {
      char* l1 = labelgen_new();
      char* l2 = labelgen_new();
      char* id1 = params[0]->value.string_value;
      char* id2 = params[1]->value.string_value;
      char* var_i = generate_var_str(id1, FT_TF, vartable_depth(vartable, id1));
      char* var_s = generate_var_str(id2, FT_TF, vartable_depth(vartable, id2));
      printcm("STRLEN GF@$tmp0 %s", var_s);
      printcm("PUSHS %s", var_i);
      printcm("PUSHS int@0", var_i);
      printcm("LTS");
      printcm("PUSHS %s", var_i);
      printcm("PUSHS GF@$tmp0");
      printcm("LTS");
      printcm("NOTS");
      printcm("ORS");
      printcm("PUSHS bool@$true");
      printcm("JUMPIFEQS %s", l1);
      printcm("STRI2INT GF@$tmp0 %s %s", var_s, var_i);
      printcm("PUSHS GF@$tmp0");
      printcm("PUSHS int@0");
      printcm("JUMP %s", l2);
      printcm("LABEL %s", l1);
      printcm("PUSHS string@");
      printcm("PUSHS int@1");
      printcm("LABEL %s", l2);
      free(l1);
      free(l2);
      free(var_s);
      free(var_i);
   }
}

// TODO: substr

bool is_builtin(char* identifier) {
   char* names[] = {
      "inputs", "inputi", "inputf", "print", "int2float", "float2int", "len", "chr", "ord", "substr"
   };
   for (int i = 0; i < (int)(sizeof(names) / sizeof(char*)); i++) {
      if (streq(identifier, names[i])) {
         return true;
      }
   }
   return false;
}

void generate_builtin(char* identifier, token_t** params, int param_count, vartable_t* vartable) {
   if (streq(identifier, "inputs")) {
      builtin_inputs();
   } else if (streq(identifier, "inputi")) {
      builtin_inputi();
   } else if (streq(identifier, "inputf")) {
      builtin_inputf();
   } else if (streq(identifier, "print")) {
      builtin_print(vartable, params, param_count);
   } else if (streq(identifier, "int2float")) {
      builtin_int2float(vartable, params[0]);
   } else if (streq(identifier, "float2int")) {
      builtin_float2int(vartable, params[0]);
   } else if (streq(identifier, "len")) {
      builtin_len(vartable, params[0]);
   } else if (streq(identifier, "chr")) {
      builtin_chr(vartable, params[0]);
   } else if (streq(identifier, "ord")) {
      builtin_ord(vartable, params);
   } else if (streq(identifier, "substr")) {
      // TODO:
   }
}
