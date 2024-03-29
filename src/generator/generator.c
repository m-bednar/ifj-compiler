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

void generate_generic(astnode_generic_t* node, vartable_t* vartable, bintree_t* fntable);

/**
 * Generates stack pops of return variables after function call.
 */
void generate_returns_pops(astnode_assign_t* node, vartable_t* vartable) {
   int clears_to = 0;
   for (int i = 0; i < node->ids_count; i++) {
      if (streq(node->left_ids[i]->value.string_value, "_")) {
         clears_to++;
      } else {
         break;
      }
   }
   for (int i = node->ids_count - 1; i >= clears_to; i--) {
      if (!streq(node->left_ids[i]->value.string_value, "_")) {
         int depth = vartable_depth(vartable, node->left_ids[i]->value.string_value);
         char* var = generate_var_str(node->left_ids[i]->value.string_value, FT_TF, depth);
         printcm("POPS %s", var);
         free(var);
      } else {
         printcm("POPS GF@$tmp0");
      }
   }
   if (clears_to != 0) {
      printcm("CLEARS");
   }
}

/**
 * Generates function call with relevant argmunets.
 */
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
            int depth = vartable_depth(vartable, node->params[i]->value.string_value);
            char* var = generate_var_str(node->params[i]->value.string_value, FT_LF, depth);
            printcm("MOVE %s %s", param, var);
            free(var);
         }
         free(param);
      }
      printcm("CALL %s", node->name);
      printcm("POPFRAME");
   }
}

/**
 * Generates variable assign.
 */
void generate_assign(astnode_assign_t* node, vartable_t* vartable, bintree_t* fntable) {
   if (node->right_function != NULL) {
      generate_funccall(node->right_function, vartable, fntable);
      generate_returns_pops(node, vartable);
   } else {
      for (int i = 0; i < node->ids_count; i++) {
         if (!streq(node->left_ids[i]->value.string_value, "_")) {
            int depth = vartable_depth(vartable, node->left_ids[i]->value.string_value);
            char* var = generate_var_str(node->left_ids[i]->value.string_value, FT_TF, depth);
            generate_assign_expression(var, node->right_expressions[i], vartable);
            free(var);
         }
      }
   }
}

/**
 * Generates defvar statement and adds variable to the vartable if necessary.
 */
void generate_defvar(astnode_defvar_t* node, vartable_t* vartable, bool assign) {
   guard(node != NULL);
   guard(vartable != NULL);
   vartype_e exptype = determine_expression_type(node->expression, vartable);
   char* var = generate_var_str(node->variable->value.string_value, FT_TF, vartable->depth);
   if (vartable_should_define(vartable, node->variable->value.string_value, exptype)) {
      vartable_add(vartable, node->variable->value.string_value, exptype);
      printcm("DEFVAR %s", var);
   }
   if (assign) {
      generate_assign_expression(var, node->expression, vartable);
   }
   free(var);
}

/**
 * Utility method, that will pre-define all variables in given codeblock.
 */
void defvar_all_vars(astnode_codeblock_t* node, vartable_t* vartable) {
   for (int i = 0; i < node->children_count; i++) {
      if (node->children[i]->type == ANT_DEFVAR) {
         generate_defvar(node->children[i]->value.defvarval, vartable, false);
      }
      if (node->children[i]->type == ANT_IF) {
         vartable_descent(vartable);
         defvar_all_vars(node->children[i]->value.ifval->true_body, vartable);
         if (node->children[i]->value.ifval->else_body != NULL) {
            defvar_all_vars(node->children[i]->value.ifval->else_body, vartable);
         }
         vartable_ascent(vartable);
      }
      if (node->children[i]->type == ANT_FOR) {
         vartable_descent(vartable);
         if (node->children[i]->value.forval->defvar != NULL) {
            generate_defvar(node->children[i]->value.forval->defvar, vartable, false);
         }
         vartable_descent(vartable);
         defvar_all_vars(node->children[i]->value.forval->body, vartable);
         vartable_ascent(vartable);
         vartable_ascent(vartable);
      }
   }
}

/**
 * Generates all nodes of given codeblock.
 */
void generate_codeblock(astnode_codeblock_t* node, vartable_t* vartable, bintree_t* fntable) {
   for (int i = 0; i < node->children_count; i++) {
      generate_generic(node->children[i], vartable, fntable);
   }
}

/**
 * Generates conditional expression with relevant jump.
 */
void generate_condition(astnode_exp_t* cond, vartable_t* vartable, char* label, bool jump_on) {
   bool stack = generate_expression(cond, vartable, false);
   if (stack) {
      printcm("PUSHS bool@%s", jump_on ? "true" : "false");
      printcm("JUMPIFEQS %s", label);
   } else {
      printcm("JUMPIFEQ %s GF@$tmp0 bool@%s", label, jump_on ? "true" : "false");
   }
}

/**
 * Generates for loop.
 */
void generate_for(astnode_for_t* node, vartable_t* vartable, bintree_t* fntable) {
   vartable_descent(vartable);
   if (node->defvar != NULL) {
      generate_defvar(node->defvar, vartable, true);
   }
   char* l1 = labelgen_new();
   char* l2 = labelgen_new();
   generate_condition(node->condition, vartable, l1, false);
   vartable_descent(vartable);
   defvar_all_vars(node->body, vartable);
   printjl("LABEL %s", l2);
   generate_codeblock(node->body, vartable, fntable);
   vartable_ascent(vartable);
   if (node->assign != NULL) {
      generate_assign(node->assign, vartable, fntable);
   }
   generate_condition(node->condition, vartable, l2, true);
   printjl("LABEL %s", l1);
   vartable_ascent(vartable);
   free(l1);
   free(l2);
}

/**
 * Generates if statement.
 */
void generate_if(astnode_if_t* node, vartable_t* vartable, bintree_t* fntable) {
   char* l1 = labelgen_new();
   char* l2;
   generate_condition(node->condition, vartable, l1, false);
   vartable_descent(vartable);
   generate_codeblock(node->true_body, vartable, fntable);
   vartable_ascent(vartable);
   if (node->else_body != NULL) {
      l2 = labelgen_new();
      printcm("JUMP %s", l2);
   }
   printjl("LABEL %s", l1);
   if (node->else_body != NULL) {
      generate_codeblock(node->else_body, vartable, fntable);
      printjl("LABEL %s", l2);
      free(l2);
   }
   free(l1);
}

/**
 * Generates function return statement.
 */
void generate_ret(astnode_ret_t* node, vartable_t* vartable) {
   for (int i = 0; i < node->expressions_count; i++) {
      if (!generate_expression(node->expressions[i], vartable, true)) {
         printcm("PUSHS GF@$tmp0");
      }
   }
   printcm("RETURN");
}

/**
 * Generates function declaration from ast node.
 */
void generate_funcdecl(astnode_funcdecl_t* node, bintree_t* fntable) {
   guard(node != NULL);
   guard(fntable != NULL);
   printlb("LABEL %s", node->name);
   vartable_t* vartable = vartable_ctor();
   symbolvalfn_t* fn = bintree_find(fntable, node->name)->value.fn;
   for (int i = 0; i < fn->arg_count; i++) {
      vartable_add(vartable, fn->arg_names[i], fn->arg_types[i]);
   }
   generate_codeblock(node->body, vartable, fntable);
   vartable_dtor(vartable);
   printcm("RETURN");
}

/**
 * Generates node depending on it's type.
 */
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
         generate_defvar(node->value.defvarval, vartable, true);
         pcomment("Defvar end");
         break;
      case ANT_FOR:
         pcomment("For start");
         generate_for(node->value.forval, vartable, fntable);
         pcomment("For end");
         break;
      case ANT_IF:
         pcomment("If start");
         generate_if(node->value.ifval, vartable, fntable);
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

void generate(astnode_global_t* global, bintree_t* fntable) {
   guard(global != NULL);
   guard(fntable != NULL);

   printlb(".IFJcode20");
   printcm("DEFVAR GF@$tmp0");
   printcm("DEFVAR GF@$tmp1");
   printcm("DEFVAR GF@$tmp2");
   printcm("DEFVAR GF@$tmp3");
   printcm("CREATEFRAME");
   printcm("CALL main");
   printcm("EXIT int@0");

   for (int i = 0; i < global->functions_count; i++) {
      printcm(" ");
      generate_funcdecl(global->functions[i], fntable);
   }
}
