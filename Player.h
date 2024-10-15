#ifndef PLAYER_H
#define PLAYER_H
#include "Node.h"
#include "Stack.h"
#include "Drop.h"

class Player{
    int undoCount;      //stores the number of undos player can do
    Node* coins;   //stores the head of linked list storing coins
    bool keyStatus; //tells if player has key or not
    int keyRow;
    int keyCol;
    int doorRow;
    int doorCol;
    int size;
    bool doorStatus; //tells if player has found door
    Stack moves;   //stores all the moves made by player
    int moveCount; //stores moves left for the player
    int score;     //stores score of player
public:
    Node* player; //points to player's current position in grid
    Drop inv;      //stores player inventory
    Player();
    int* initializePlayerCoords(int s) const; //intializes player coordinates
    void movePlayer(char c, bool pushToStack = true);//handles player movement
    void undo();                              //undos last move
    bool getKeyStatus() const;                //returns key status
    bool getDoorStatus() const;               //returns door status
    void changeKeyStatus();                   //changes key status when player gets key
    void changeDoorStatus();                  //changes door status when player has key
    void setKeyAndDoor(int kr, int kc, int dr, int dc,int s);
    char generateDrop(int dc) const;          //generates a drop based on different percentages
    void giveCoinToPlayer(int r, int c);      //adds to the coins linked list
    void setUndoCount(int u);                 //sets undo count
    void setMoveCount(int mc);                //sets move count
    void setScore(int s);                     //sets score
    int getMoveCount() const;                 //gets move count
    int getUndoCount() const;                 //gets undo count
    int getScore() const;                     //gets scoreCount
};

#endif