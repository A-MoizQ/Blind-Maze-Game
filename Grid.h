#ifndef GRID_H
#define GRID_H
#include "Node.h"
#include "Player.h"
#include "Drop.h"


class Grid{
    Node* topLeft;
    Node* key;
    Node* door;
    int size;
public:
    Player p;
    Grid();
    void initializeGame(int s);                     //sets up the entire game
    int calculateDistance(Node* A, Node* B) const;  //calculates city block distance between A and B
    Node* getKey() const;                           //returns key pointer
    Node* getDoor() const;                          //returns door pointer
    bool closer(int dist1, int dist2) const;        //checks if player got closer
    void display(int r,int c) const;                //displays the grid
    void displayEndGame(int r, int c) const;              //displays end game grid
    ~Grid();
};



#endif