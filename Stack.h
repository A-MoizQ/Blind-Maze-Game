#ifndef STACK_H
#define STACK_H
#include "Node.h"

class Stack{
    Node* head;
public:
    Stack();
    bool isEmpty() const;
    void push(char data);
    char pop();
    char seek() const;
    ~Stack();
};



#endif