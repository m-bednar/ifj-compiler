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
#include <stdbool.h>
#include <stdio.h>

void generate_returns_pops(astnode_assign_t* node, vartable_t* vartable) {
   int clears_from = node->ids_count;
   for (int i = node->ids_count - 1; i >= 0; i--) {
      if (strcmp(node->left_ids[i]->value.string_value, "_") == 0) {
         clears_from = i;
      } else {
         break;
      }
   }
   for (int i = 0; i < clears_from; i++) {
      if (strcmp(node->left_ids[i]->value.string_value, "_") != 0) {
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

void generate_funccall(astnode_funccall_t* node, vartable_t* vartable, bintree_t* fntable, bool clears) {
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

   if (clears && fn_declaration->value.fn->ret_count > 0) {
      printcm("CLEARS");
   }
}

void generate_assign(astnode_assign_t* node, vartable_t* vartable, bintree_t* fntable) {
   if (node->right_function != NULL) {
      generate_funccall(node->right_function, vartable, fntable, false);
      generate_returns_pops(node, vartable);
   } else {
      // TODO:
   }
}

void generate_defvar(astnode_defvar_t* node, vartable_t* vartable) {
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

void generate_ret(astnode_ret_t* node) {
   node = node;
}

void generate_generic(astnode_generic_t* node, vartable_t* vartable, bintree_t* fntable) {
   fntable = fntable; // TODO: Remove when fntable is used
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
         generate_funccall(node->value.funccallval, vartable, fntable, true);
         pcomment("Funccall end");
         break;
      case ANT_RET:
         pcomment("Return start");
         generate_ret(node->value.returnval);
         pcomment("Return end");
         break;
      default: 
         exit(ERRCODE_INTERNAL_ERROR);
   }
}

void generate_funcdecl(astnode_funcdecl_t* func, bintree_t* fntable) {
   printlb("LABEL %s", func->name);
   vartable_t* vartable = vartable_ctor();
   for (int i = 0; i < func->body->children_count; i++) {
      generate_generic(func->body->children[i], vartable, fntable);
   }
   vartable_dtor(vartable);
   printcm("RETURN");
}

void generate(astnode_global_t* global, bintree_t* fntable) {
   printlb(".IFJcode20");
   printcm("DEFVAR GF@$tmp");
   printcm("CREATEFRAME");
   printcm("CALL main");
   printcm("EXIT int@0");

   for (int i = 0; i < global->functions_count; i++) {
      printcm(" ");
      generate_funcdecl(global->functions[i], fntable);
   }
}
