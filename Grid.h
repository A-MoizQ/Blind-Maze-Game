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
    int* initializePlayerCoords(int s) const;
    void movePlayer(char c);
    ~Grid();
};



#endif