/**
 * IFJ20 Compiler
 * @file main.c
 * @authors Martin Bednář (xbedna77)
 */

#include <stdio.h>
#include <stdlib.h>

#include "generator/utils.h"
#include "generator/generator.h"

int main() {
   astnode_global_t* global = safe_alloc(sizeof(astnode_global_t));
   global->functions = safe_alloc(1 * sizeof(astnode_funcdecl_t*));
   global->functions_count = 1;
   global->functions[0] = safe_alloc(sizeof(astnode_funcdecl_t));
   global->functions[0]->name = safe_alloc(sizeof(char) * 5);
   strcpy(global->functions[0]->name, "main");
   global->functions[0]->body = safe_alloc(sizeof(astnode_codeblock_t));
   global->functions[0]->body->children_count = 1;
   global->functions[0]->body->children = safe_alloc(sizeof(astnode_generic_t*));
   global->functions[0]->body->children[0] = safe_alloc(sizeof(astnode_generic_t));
   global->functions[0]->body->children[0]->type = ANT_DEFVAR;
   global->functions[0]->body->children[0]->value.defvarval = safe_alloc(sizeof(astnode_defvar_t));
   token_value_u value;
   value.string_value = safe_alloc(6);
   strcpy(value.string_value, "abcds");
   global->functions[0]->body->children[0]->value.defvarval->variable = token_ctor(TOKENID_IDENTIFIER, value);
   global->functions[0]->body->children[0]->value.defvarval->expression = safe_alloc(sizeof(astnode_exp_t));
   global->functions[0]->body->children[0]->value.defvarval->expression->tokens_count = 5;
   global->functions[0]->body->children[0]->value.defvarval->expression->tokens = safe_alloc(sizeof(token_t*) * 4);
   global->functions[0]->body->children[0]->value.defvarval->expression->tokens[0] = safe_alloc(sizeof(token_t));
   global->functions[0]->body->children[0]->value.defvarval->expression->tokens[0]->id = TOKENID_NUM_DECIMAL;
   global->functions[0]->body->children[0]->value.defvarval->expression->tokens[0]->value.decimal_value = 5.0;

   global->functions[0]->body->children[0]->value.defvarval->expression->tokens[1] = safe_alloc(sizeof(token_t));
   global->functions[0]->body->children[0]->value.defvarval->expression->tokens[1]->id = TOKENID_OPERATOR_ADD;

   global->functions[0]->body->children[0]->value.defvarval->expression->tokens[2] = safe_alloc(sizeof(token_t));
   global->functions[0]->body->children[0]->value.defvarval->expression->tokens[2]->id = TOKENID_NUM_DECIMAL;
   global->functions[0]->body->children[0]->value.defvarval->expression->tokens[2]->value.decimal_value = 2.2;

   global->functions[0]->body->children[0]->value.defvarval->expression->tokens[3] = safe_alloc(sizeof(token_t));
   global->functions[0]->body->children[0]->value.defvarval->expression->tokens[3]->id = TOKENID_OPERATOR_MUL;

   global->functions[0]->body->children[0]->value.defvarval->expression->tokens[4] = safe_alloc(sizeof(token_t));
   global->functions[0]->body->children[0]->value.defvarval->expression->tokens[4]->id = TOKENID_NUM_DECIMAL;
   global->functions[0]->body->children[0]->value.defvarval->expression->tokens[4]->value.decimal_value = 9.1;
   
   generate(global, bintree_ctor());
   return EXIT_SUCCESS;
}
