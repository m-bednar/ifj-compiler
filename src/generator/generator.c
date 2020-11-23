/**
 * IFJ20 Compiler
 * @file generator.c
 * @authors Martin Bednář (xbedna77)
 */

#include "generator.h"
#include <stdbool.h>
#include <stdio.h>

#define streq(a, b) !strcmp(a, b)
#define printcm(s, ...) printf("   "); printf(s,##__VA_ARGS__); printf("\n")   // Print command function
#define printlb(s, ...) printf("\n"); printf(s,##__VA_ARGS__); printf("\n")  // Print label function

void generate_assign(astnode_assign_t node) {

}

void generate_defvar(astnode_defvar_t node) {

}

void generate_for(astnode_forval_t node) {

}

void generate_if(astnode_if_t node) {

}

void generate_funccall(astnode_funccall_t node) {

}

void generate_ret(astnode_ret_t node) {

}

void generate_generic(astnode_generic_t* node) {
   switch (node->type) {
      case ANT_ASSIGN:
         /* code */
         break;
      case ANT_DEFVAR:
         /* code */
         break;
      case ANT_FOR:
         /* code */
         break;
      case ANT_IF:
         /* code */
         break;
      case ANT_FUNCCALL:
         /* code */
         break;
      case ANT_RET:
         /* code */
         break;
      default: 
         exit(ERRCODE_INTERNAL_ERROR);
   }
}

void generate_funcdecl(astnode_funcdecl_t* func) {
   printlb("LABEL %s", func->name);
   for (int i = 0; i < func->body->children_count; i++) {
      generate_generic(func->body->children[i]);
   }
   printcm("RETURN");
}

void generate(astnode_global_t* global, bintree_t* symtable) {
   printlb(".IFJcode20");
   printcm("CALL main");
   printcm("EXIT int@0");
   for (int i = 0; i < global->functions_count; i++) {
      generate_funcdecl(global->functions[i]);
   }
}
