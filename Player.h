#ifndef PLAYER_H
#define PLAYER_H
#include "Node.h"
#include "Stack.h"

class Player{
    int undoCount;      //stores the number of undos player can do
    Node* coins;   //stores the head of linked list storing coins
    bool keyStatus; //tells if player has key or not
    Stack moves;   //stores all the moves made by player
    int moveCount; //stores moves left for the player
public:
    Node* player; //points to player's current position in grid
    Player(int u, int mc);
    int* initializePlayerCoords(int s) const;
    void movePlayer(char c);
    void undo();
    int getMoveCount() const;
    ~Player();
};

#endif