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
   astparentbody_t parentbody;   //current body of parent ast node
} astparentnode_t;

typedef struct astnodestack_t {
   int length;
   int capacity;
   astparentnode_t** memory;
} astnodestack_t;

/**
 * Allocates a new stack
 */
astnodestack_t* astnodestack_ctor();

/**
 * Deallocates stack, does not dealocates ast nodes
 */
void astnodestack_dtor(astnodestack_t* stack);

/**
 * Returns top element
 */
astparentnode_t* astnodestack_peek(astnodestack_t* stack);

/**
 * Returns top element and remove it from the stack
 */
astparentnode_t* astnodestack_pop(astnodestack_t* stack);

/**
 * Add new element on of the stack
 */
void astnodestack_push(astnodestack_t* stack, astnode_generic_t* astnode, astparentbody_t parentbody);

/**
 * Returns length of the stack
 */
int astnodestack_length(astnodestack_t* stack);

