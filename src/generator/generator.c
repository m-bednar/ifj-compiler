/**
 * IFJ20 Compiler
 * @file generator.c
 * @authors Martin Bednář (xbedna77)
 */

#include "generator.h"
#include "expression.h"
#include "utils.h"
#include "../symtable/bintreestack.h"
#include <stdbool.h>
#include <stdio.h>

void generate_assign(astnode_assign_t* node) {
   node = node;
}

void generate_defvar(astnode_defvar_t* node, bintreestack_t* varstack) {
   char* var = str_var(node->variable->value.string_value, FT_TF, varstack);
   printcm("DEFVAR %s", var);
   // TODO:
   free(var);
}

void generate_for(astnode_for_t* node) {
   node = node;
}

void generate_if(astnode_if_t* node) {
   node = node;
}

void generate_funccall(astnode_funccall_t* node) {
   node = node;
}

void generate_ret(astnode_ret_t* node) {
   node = node;
}

void generate_generic(astnode_generic_t* node, bintree_t* fntable, bintreestack_t* varstack) {
   fntable = fntable;
   switch (node->type) {
      case ANT_ASSIGN:
         generate_assign(node->value.assignval);
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
         generate_funccall(node->value.funccallval);
         break;
      case ANT_RET:
         generate_ret(node->value.returnval);
         break;
      default: 
         exit(ERRCODE_INTERNAL_ERROR);
   }
}

void generate_funcdecl(astnode_funcdecl_t* func, bintree_t* fntable, bintreestack_t* varstack) {
   printlb("LABEL %s", func->name);
   bintreestack_push(varstack, bintree_ctor());
   for (int i = 0; i < func->body->children_count; i++) {
      generate_generic(func->body->children[i], fntable, varstack);
   }
   bintree_dtor(bintreestack_pop(varstack));
   printcm("RETURN");
}

void generate(astnode_global_t* global, bintree_t* fntable) {
   bintreestack_t* varstack = bintreestack_ctor();
   printlb(".IFJcode20");
   printcm("CALL main");
   printcm("EXIT int@0");

   for (int i = 0; i < global->functions_count; i++) {
      printcm(" ");
      generate_funcdecl(global->functions[i], fntable, varstack);
   }

   bintreestack_dtor(varstack);
   bintree_dtor(fntable);
}
