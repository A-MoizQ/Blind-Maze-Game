#include "Stack.h"

Stack::Stack():head(nullptr){}

// empty the stack
Stack::~Stack(){
    while (!isEmpty()){
        pop();
    }
}

//check if stack is empty
bool Stack::isEmpty() const{
    return (head == nullptr);
}

//gives the top of stack
char Stack::seek() const{
    return head->data;
}

//push to the stack
void Stack::push(char data){
    Node* newNode = new Node;
    newNode->data = data;
    newNode->right = head;
    head = newNode;
}

//remove top value from stack and return it
char Stack::pop(){
    Node* temp = head;
    head = head->right;
    char data = temp->data;
    delete temp;
    return data;
}