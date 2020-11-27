/**
 * IFJ20 Compiler
 * @file generator.c
 * @authors Martin Bednář (xbedna77)
 */

#include "generator.h"
#include "expression.h"
#include "vargen.h"
#include "utils.h"
#include "vartable.h"
#include "builtins.h"
#include <stdbool.h>
#include <stdio.h>

void generate_returns_pops(astnode_assign_t* node, vartable_t* vartable) {
   int clears_from = node->ids_count;
   for (int i = node->ids_count - 1; i >= 0; i--) {
      if (streq(node->left_ids[i]->value.string_value, "_")) {
         clears_from = i;
      } else {
         break;
      }
   }
   for (int i = 0; i < clears_from; i++) {
      if (!streq(node->left_ids[i]->value.string_value, "_")) {
         int depth = vartable_find(vartable, node->left_ids[i]->value.string_value)->depth;
         printcm("POPS %s", generate_var_str(node->left_ids[i]->value.string_value, FT_TF, depth));
      } else {
         printcm("POPS GF@$tmp");
      }
   }
   if (clears_from != node->ids_count) {
      printcm("CLEARS");
   }
}

void generate_funccall(astnode_funccall_t* node, vartable_t* vartable, bintree_t* fntable) {
   if (is_builtin(node->name)) {
      generate_builtin(node->name, node->params, node->params_count, vartable);
   } else {
      symbol_t* fn_declaration = bintree_find(fntable, node->name);
      printcm("PUSHFRAME");
      printcm("CREATEFRAME");
      for (int i = 0; i < node->params_count; i++) {
         char* param = generate_var_str(fn_declaration->value.fn->arg_names[i], FT_TF, 0);
         printcm("DEFVAR %s", param);

         if (is_const_tokenid(node->params[i]->id)) {
            printcm("MOVE %s %s", param, generate_const_str(node->params[i]));
         } else {
            int depth = vartable_find(vartable, node->params[i]->value.string_value)->depth;
            char* var = generate_var_str(node->params[i]->value.string_value, FT_TF, depth);
            printcm("MOVE %s %s", param, var);
            free(var);
         }
         free(param);
      }
      printcm("CALL %s", node->name);
      printcm("POPFRAME");
   }
}

void generate_assign(astnode_assign_t* node, vartable_t* vartable, bintree_t* fntable) {
   if (node->right_function != NULL) {
      generate_funccall(node->right_function, vartable, fntable);
      generate_returns_pops(node, vartable);
   } else {
      for (int i = 0; i < node->ids_count; i++) {
         generate_assign_expression(node->left_ids[i]->value.string_value, node->right_expressions[i], vartable);
      }
   }
}

void generate_defvar(astnode_defvar_t* node, vartable_t* vartable) {
   guard(node != NULL);
   guard(vartable != NULL);
   vartype_e exptype = determine_expression_type(node->expression, vartable);
   if (vartable_should_define(vartable, node->variable->value.string_value, exptype)) {
      vartable_add(vartable, node->variable->value.string_value, exptype);
      char* var = generate_var_str(node->variable->value.string_value, FT_TF, vartable->depth);
      printcm("DEFVAR %s", var);
      free(var);
   }
   generate_assign_expression(node->variable->value.string_value, node->expression, vartable);
}

void generate_for(astnode_for_t* node) {
   node = node;
}

void generate_if(astnode_if_t* node) {
   node = node;
}

void generate_ret(astnode_ret_t* node, vartable_t* vartable) {
   for (int i = 0; i < node->expressions_count; i++) {
      if (!generate_expression(node->expressions[i], vartable, true)) {
         printcm("PUSHS GF@$tmp");
      }
   }
}

void generate_generic(astnode_generic_t* node, vartable_t* vartable, bintree_t* fntable) {
   guard(node != NULL);
   guard(vartable != NULL);
   guard(fntable != NULL);
   switch (node->type) {
      case ANT_ASSIGN:
         pcomment("Assigment start");
         generate_assign(node->value.assignval, vartable, fntable);
         pcomment("Assigment end");
         break;
      case ANT_DEFVAR:
         pcomment("Defvar start");
         generate_defvar(node->value.defvarval, vartable);
         pcomment("Defvar end");
         break;
      case ANT_FOR:
         pcomment("For start");
         generate_for(node->value.forval);
         pcomment("For end");
         break;
      case ANT_IF:
         pcomment("If start");
         generate_if(node->value.ifval);
         pcomment("If end");
         break;
      case ANT_FUNCCALL:
         pcomment("Funccall start");
         generate_funccall(node->value.funccallval, vartable, fntable);
         pcomment("Funccall end");
         break;
      case ANT_RET:
         pcomment("Return start");
         generate_ret(node->value.returnval, vartable);
         pcomment("Return end");
         break;
      default: 
         exit(ERRCODE_INTERNAL_ERROR);
   }
}

void generate_funcdecl(astnode_funcdecl_t* node, bintree_t* fntable) {
   guard(node != NULL);
   guard(fntable != NULL);
   printlb("LABEL %s", node->name);
   vartable_t* vartable = vartable_ctor();
   for (int i = 0; i < node->body->children_count; i++) {
      generate_generic(node->body->children[i], vartable, fntable);
   }
   vartable_dtor(vartable);
   printcm("RETURN");
}

void generate(astnode_global_t* global, bintree_t* fntable) {
   guard(global != NULL);
   guard(fntable != NULL);

   printlb(".IFJcode20");
   printcm("DEFVAR GF@$tmp");
   printcm("DEFVAR GF@$tmp2");
   printcm("CREATEFRAME");
   printcm("CALL main");
   printcm("EXIT int@0");

   for (int i = 0; i < global->functions_count; i++) {
      printcm(" ");
      generate_funcdecl(global->functions[i], fntable);
   }
}
