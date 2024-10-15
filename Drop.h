#ifndef DROP_H
#define DROP_H
#include "Node.h"
#include<ncurses.h>

class Drop{
    //que for items in grid
    Node* currentDropHead;
    Node* currentDropTail;
    //que for items next in line
    Node* nextInLineHead;
    Node* nextInLineTail;
    //timer to remove previous drop and add new
    int timer;
    //difference between level difficulties
    int dropCount;
public:
    Drop();
    void insertInCurrent(char data); //for inserting in grid
    char seekCurrentTail() const;     //returns value at tail of current
    void insertInNext(char data);    //for inserting in next in line
    char removeFromCurrent(); //removing from current
    char removeFromNext();   //removing from next in line
    void setDropCoords(int r, int c);
    //generate random drop coordinates 
    void generateDropCoords(int& dropRow, int& dropCol, int& keyRow, int& keyCol, int& doorRow, int& doorCol, int& playerRow, int& playerCol,int size, int* dropRows, int* dropCols, int dropCount);
    void pickUpCoin(int r, int c);     //to remove the coin picked up by user
    int* getDropRows() const;
    int* getDropCols() const;
    void updateTimer();       //updates the timer 
    int getTimer() const;     //returns timer
    void resetTimer();        //resets the timer
    void setDropCount(int dc);//sets dropcount
    int getDropCount() const; //gets dropCount
    void displayNextInLine(int r, int c) const; //displays the next in line queue
    ~Drop();                  //handles deallocation of memory

};


#endif