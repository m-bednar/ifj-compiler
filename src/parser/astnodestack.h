/**
 * IFJ20 Compiler
 * @file ast.c
 * @authors Michal Trlica (xtrlic02)
 */

#include "ast.h"

typedef struct astnodestack_t {
   int length;
   int capacity;
   astnode_generic_t** memory;
} astnodestack_t;

astnodestack_t* astnodestack_ctor();

void astnodestack_dtor(astnodestack_t* stack);

astnode_generic_t* astnodestack_peek(astnodestack_t* stack);

astnode_generic_t* astnodestack_pop(astnodestack_t* stack);

void astnodestack_push(astnodestack_t* stack, astnode_generic_t* astnode);

int astnodestack_lenght(astnodestack_t* stack);

