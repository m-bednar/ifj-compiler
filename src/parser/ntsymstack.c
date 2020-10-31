/**
 * IFJ20 Compiler
 * @file ntsymstack.c
 * @authors Daniel Havranek (xhavra18)
 */

#include "ntsymstack.h"

static const float DEALLOC_MIN_UNUSED_PERCENTAGE = 0.5;
static const int ALLOC_MIN_ELEMENTS_COUNT = 4;

ntsymstack_t* ntsymstack_ctor() {
    ntsymstack_t* stack = safe_alloc(sizeof(ntsymstack_t));

    stack->length = 0;
    stack->capacity = 0;
    stack->memory = NULL;
    return stack;
}

void ntsymstack_dtor(ntsymstack_t* stack) {
    guard(stack != NULL);

    if (stack->memory != NULL) {
        for (int i = 0; i < stack->length; i++) {
            ntsymbol_dtor(stack->memory[i]);
        }
        free(stack->memory);
        stack->length = 0;
        stack->capacity = 0;
        stack->memory = NULL;
        free(stack);
    }
    return;
}

void ntsymstack_push(ntsymstack_t* stack, ntsymbol_t* ntsymbol) {
    guard(stack != NULL);
    guard(ntsymbol != NULL);
    
    // Allocates additional memory, when capacity is reached
    if (stack->capacity == stack->length) {
        stack->memory = safe_realloc(stack->memory, sizeof(ntsymbol_t*) * (stack->length + ALLOC_MIN_ELEMENTS_COUNT));
        stack->capacity += ALLOC_MIN_ELEMENTS_COUNT;
    }

    stack->memory[stack->length] = ntsymbol;
    stack->length++;
    return;
}

ntsymbol_t* ntsymstack_pop(ntsymstack_t* stack) {
    guard(stack != NULL);
    guard(stack->length != 0);

    ntsymbol_t* top = stack->memory[stack->length - 1];
    stack->length--;

    // Deallocates excessive memory, when 1/3 or less than 1/3 of the capacity is used
    if (stack->length < stack->capacity * DEALLOC_MIN_UNUSED_PERCENTAGE) {
        stack->memory = safe_realloc(stack->memory, sizeof(ntsymbol_t*) * (stack->length));
        stack->capacity = stack->length;
    }

    return top; 
}

ntsymbol_t* ntsymstack_top(ntsymstack_t* stack) {
    guard(stack != NULL);
    guard(stack->length != 0);

    ntsymbol_t* top = stack->memory[stack->length - 1];
    return top; 
}

int ntsymstack_get_length(ntsymstack_t* stack) {
    return stack->length;
}
