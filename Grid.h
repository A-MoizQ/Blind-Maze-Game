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
    Node* getKey() const;
    Node* getDoor() const;
    bool closer(int dist1, int dist2) const;
    void display(int r,int c) const;
    ~Grid();
};



#endif