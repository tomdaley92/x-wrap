#ifndef ABSTRACT_SYNTAX_TREE_H
#define ABSTRACT_SYNTAX_TREE_H

class AbstractSyntaxTree {
    private:
        void preOrder(AbstractSyntaxTree *node);
        void inOrder(AbstractSyntaxTree *node);
        void postOrder(AbstractSyntaxTree *node);
        void format(char **padding, char **formatted);

    public:
        int value;
        AbstractSyntaxTree *left;
        AbstractSyntaxTree *right;

        AbstractSyntaxTree(int value);

        AbstractSyntaxTree(
            int value, 
            AbstractSyntaxTree *left, 
            AbstractSyntaxTree *right
        );
        
        ~AbstractSyntaxTree();

        void printPreOrder();
        void printInOrder();
        void printPostOrder();

        void toString(char *formatted);
        void display();
};

#endif
