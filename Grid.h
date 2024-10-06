#ifndef GRID_H
#define GRID_H
#include "Node.h"
#include "Player.h"


class Grid{
    Node* topLeft;
    int size;
public:
    Player p;
    Grid(int s);
    void display() const;
    ~Grid();
};



#endif