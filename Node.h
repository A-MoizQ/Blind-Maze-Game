#ifndef NODE_H
#define NODE_H

struct Node{
    char data;
    int* coords;
    Node* up;
    Node* down;
    Node* right;
    Node* left;
    Node();
};



#endif