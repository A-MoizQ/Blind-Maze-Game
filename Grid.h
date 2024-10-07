#ifndef GRID_H
#define GRID_H
#include "Node.h"
#include "Player.h"


class Grid{
    Node* topLeft;
    Node* key;
    Node* door;
    int size;
public:
    Player p;
    Grid();
    void initializeGame(int s);
    int calculateDistance(Node* A, Node* B) const;
    void display(int r,int c) const;
    ~Grid();
};



#endif