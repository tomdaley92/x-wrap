#include "Stack.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

Stack::Stack() {
    size = 0;
    stack = (char **) malloc(sizeof(char *) * LIMIT);
    bottom = 0; 
}

Stack::Stack(char *data) {
    size = 1;
    // TODO: init Stack, add data to Stack
    if (data) {
        stack = (char **) calloc(LIMIT, sizeof(char *));
    }
    stack[0] = (char *) calloc(strlen(data), sizeof(char));
    bottom = 0;
}

Stack::~Stack() {
    /* empty */
    for (int i = 0; i < size; i++) {
        free(stack[i]);
    }
    free(stack);
}

void Stack::push(char *data) {
    // TODO: copy data onto top of stack
    bottom++;
    if (bottom >= limit) {
        //TODO: Shift all the elements up?

    }

    if (data) {
        size = strlen(data) + 1;
        stack[bottom] = (char *) calloc(strlen(data) + 1, sizeof(char));
        memcpy_s(stack[bottom], strlen(data) + 1, data , strlen(data) + 1); 
    }
}

char *Stack::pop() {
    // TODO: return apointer to the top of the stack
    if (size <= 0) return NULL;

    char *top = stack[0];

    stack++;
    size--;

    if (size >= LIMIT) {
        limit = LIMIT;
        stack = (char **) realloc(stack, sizeof(char *) * limit);
    }

    return top;
}

char *Stack::peek() {
    if (size >= 0) return stack[0];
    return NULL;
}

int Stack::len() {
    return size;
}
