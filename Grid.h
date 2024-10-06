#ifndef GRID_H
#define GRID_H
#include "Node.h"

class Grid{
    Node* player;
    Node* topLeft;
    int size;
public:
    Grid(int s);
    void display() const;
    ~Grid();
};



#endif