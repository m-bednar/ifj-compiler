/**
 * IFJ20 Compiler
 * @file ast.c
 * @authors Michal Trlica (xtrlic02)
 */

#include "ast.h"

typedef enum astparentbody_t{
   AP_IF_TRUE,
   AP_IF_FALSE,
   AP_FOR
} astparentbody_t;

typedef struct  astparentnode_t{
   astnode_generic_t* astnode;
   astparentbody_t parentbody;
} astparentnode_t;

typedef struct astnodestack_t {
   int length;
   int capacity;
   astparentnode_t** memory;
} astnodestack_t;


astnodestack_t* astnodestack_ctor();

void astnodestack_dtor(astnodestack_t* stack);

astparentnode_t* astnodestack_peek(astnodestack_t* stack);

astparentnode_t* astnodestack_pop(astnodestack_t* stack);

void astnodestack_push(astnodestack_t* stack, astnode_generic_t* astnode, astparentbody_t parentbody);

int astnodestack_length(astnodestack_t* stack);

