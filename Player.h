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
    int score;     //stores score of player
public:
    Node* player; //points to player's current position in grid
    Player();
    int* initializePlayerCoords(int s) const; //intializes player coordinates
    void movePlayer(char c);                  //handles player movement
    void undo();                              //undos last move
    bool getKeyStatus() const;                //returns key status
    void changeKeyStatus();                   //changes key status when player gets key
    void setUndoCount(int u);                 //sets undo count
    void setMoveCount(int mc);                //sets move count
    void setScore(int s);                     //sets score
    int getMoveCount() const;                 //gets move count
    int getUndoCount() const;                 //gets undo count
    int getScore() const;                     //gets scoreCount
};

#endif