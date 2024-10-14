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
    void insertInNext(char data);    //for inserting in next in line
    char removeFromCurrent(); //removing from current
    char removeFromNext();   //removing from next in line
    void updateTimer();       //updates the timer 
    int getTimer() const;     //returns timer
    void resetTimer();        //resets the timer
    void setDropCount(int dc);//sets dropcount
    int getDropCount() const; //gets dropCount
    void displayNextInLine(int r, int c) const; //displays the next in line queue
    ~Drop();                  //handles deallocation of memory

};


#endif