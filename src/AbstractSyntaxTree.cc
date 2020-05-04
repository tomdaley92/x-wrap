#include "AbstractSyntaxTree.h"
#include <stdlib.h>
#include <stdio.h>
#include <math.h> /* floor(), log10(), abs() */

AbstractSyntaxTree::AbstractSyntaxTree(int value) {
    this->value = value;
    this->left = NULL;
    this->right = NULL;
}

AbstractSyntaxTree::AbstractSyntaxTree(
    int value, 
    AbstractSyntaxTree *left, 
    AbstractSyntaxTree *right
) {
    this->value = value;
    left? this->left = left : this->left = NULL;
    right? this->right = right : this->right = NULL;
}

AbstractSyntaxTree::~AbstractSyntaxTree() {
    /* empty */
}

void AbstractSyntaxTree::preOrder(AbstractSyntaxTree *node) {
    if (!node) return;
    fprintf(stdout, "%d ", node->value);
    preOrder(node->left);
    preOrder(node->right);
}

void AbstractSyntaxTree::inOrder(AbstractSyntaxTree *node) {
    if (!node) return;
    inOrder(node->left);
    fprintf(stdout, "%d ", node->value);
    inOrder(node->right);
}

void AbstractSyntaxTree::postOrder(AbstractSyntaxTree *node) {
    if (!node) return;
    postOrder(node->left);
    postOrder(node->right);
    fprintf(stdout, "%d ", node->value);
}

void AbstractSyntaxTree::printPreOrder() {
    preOrder(this);
    fprintf(stdout, "\n");
}

void AbstractSyntaxTree::printInOrder() {
    inOrder(this);
    fprintf(stdout, "\n");
}

void AbstractSyntaxTree::printPostOrder() {
    postOrder(this);
    fprintf(stdout, "\n");
}

void AbstractSyntaxTree::format(char *padding, char *formatted) {
    // private helper - TODO
    // https://gist.github.com/tomdaley92/507c3a99c56b779144d9c79c0a3900be
    int size;
    free(formatted);
    formatted = NULL;

    // this needs work....
    
    // get the size of the digits
    size = (int) (floor(log10(abs(value))) + 1) + 3;
    formatted = (char *) malloc(sizeof(char) * size);
    sprintf_s(formatted, size, "(%d)", value);
    

    //size = strlen(formatted) + strlen(padding) + 1;
    //formatted = (char *) realloc(sizeof(char) * size)
}

void AbstractSyntaxTree::toString(char *formatted) {
    // public toString() - TODO
    format(NULL, formatted);
}

void AbstractSyntaxTree::display() {
    // TODO
    //char *formatted = NULL;
    //format(formatted);
    //fprintf(stdout,"%s\n", formatted);
    //free (formatted);
}
