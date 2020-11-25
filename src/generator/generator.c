/**
 * IFJ20 Compiler
 * @file generator.c
 * @authors Martin Bednář (xbedna77)
 */

#include "generator.h"
#include "expression.h"
#include "vargen.h"
#include "utils.h"
#include "../symtable/bintreestack.h"
#include <stdbool.h>
#include <stdio.h>

void generate_returns_pops(astnode_assign_t* node, bintreestack_t* varstack) {
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
         int depth = get_var_depth(node->left_ids[i]->value.string_value, varstack);
         printcm("POPS %s", generate_var_str(node->left_ids[i]->value.string_value, FT_TF, depth));
      } else {
         printcm("POPS GF@$tmp");
      }
   }
   if (clears_from != node->ids_count) {
      printcm("CLEARS");
   }
}

void generate_funccall(astnode_funccall_t* node, bintreestack_t* varstack, bintree_t* fntable, bool clears) {
   symbol_t* fn_declaration = bintree_find(fntable, node->name);
   
   printcm("PUSHFRAME");
   printcm("CREATEFRAME");

   for (int i = 0; i < node->params_count; i++) {
      char* param = generate_var_str(fn_declaration->value.fn->arg_names[i], FT_TF, 0);
      printcm("DEFVAR %s", param);

      if (is_const_tokenid(node->params[i]->id)) {
         printcm("MOVE %s %s", param, generate_const_str(node->params[i]));
      } else {
         int depth = get_var_depth(node->params[i]->value.string_value, varstack);
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

void generate_assign(astnode_assign_t* node, bintreestack_t* varstack, bintree_t* fntable) {
   if (node->right_function != NULL) {
      generate_funccall(node->right_function, varstack, fntable, false);
      generate_returns_pops(node, varstack);
   }
}

void generate_defvar(astnode_defvar_t* node, bintreestack_t* varstack) {
   int depth = new_var_depth(varstack);
   char* var = generate_var_str(node->variable->value.string_value, FT_TF, depth);
   
   symbol_t* symbol = symbol_ctor(node->variable->value.string_value, ST_VARIABLE, symbolval_var_ctor(VT_UNDEFINED));
   bintree_add(bintreestack_peek(varstack), symbol);
   
   printcm("DEFVAR %s", var);
   generate_assign_expression(node->variable->value.string_value, var, node->expression, varstack, true);

   free(var);
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

void generate_generic(astnode_generic_t* node, bintreestack_t* varstack, bintree_t* fntable) {
   fntable = fntable; // TODO: Remove when fntable is used
   switch (node->type) {
      case ANT_ASSIGN:
         generate_assign(node->value.assignval, varstack, fntable);
         break;
      case ANT_DEFVAR:
         generate_defvar(node->value.defvarval, varstack);
         break;
      case ANT_FOR:
         generate_for(node->value.forval);
         break;
      case ANT_IF:
         generate_if(node->value.ifval);
         break;
      case ANT_FUNCCALL:
         generate_funccall(node->value.funccallval, varstack, fntable, true);
         break;
      case ANT_RET:
         generate_ret(node->value.returnval);
         break;
      default: 
         exit(ERRCODE_INTERNAL_ERROR);
   }
}

void generate_funcdecl(astnode_funcdecl_t* func, bintreestack_t* varstack, bintree_t* fntable) {
   printlb("LABEL %s", func->name);
   bintreestack_push(varstack, bintree_ctor());
   for (int i = 0; i < func->body->children_count; i++) {
      generate_generic(func->body->children[i], varstack, fntable);
   }
   bintreestack_print(varstack);
   bintree_dtor(bintreestack_pop(varstack));
   printcm("RETURN");
}

void generate(astnode_global_t* global, bintree_t* fntable) {
   bintreestack_t* varstack = bintreestack_ctor();
   printlb(".IFJcode20");
   printcm("DEFVAR GF@$tmp");
   printcm("CREATEFRAME");
   printcm("CALL main");
   printcm("EXIT int@0");

   for (int i = 0; i < global->functions_count; i++) {
      printcm(" ");
      generate_funcdecl(global->functions[i], varstack, fntable);
   }

   bintreestack_dtor(varstack);
   bintree_dtor(fntable);
}
