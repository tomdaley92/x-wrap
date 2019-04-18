#ifndef STACK_H
#define STACK_H

#define LIMIT 50

class Stack {
    private:
        int size;
        int limit;
        char *data;
        char **stack;
        int bottom;

    public:
        Stack();
        Stack(char *data);
        ~Stack();

        void push(char *data);
        char *pop();
        char *peek();
        int len();
};

#endif
