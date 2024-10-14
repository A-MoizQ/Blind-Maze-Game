#include "Drop.h"

Drop::Drop():currentDropHead(nullptr),currentDropTail(nullptr),nextInLineHead(nullptr),nextInLineTail(nullptr),timer(0),dropCount(0){}

Drop::~Drop(){
    while(removeFromCurrent() != ' ');
    while(removeFromNext() != ' ');
}

// inserts at tail
void Drop::insertInCurrent(char data){
    Node* newNode = new Node;
    newNode->data = data;
    if(currentDropHead == nullptr && currentDropTail == nullptr){
        currentDropHead = newNode;
        currentDropTail = newNode;
    }
    else{
        currentDropTail->right = newNode;
        newNode->left = currentDropTail;
        currentDropTail = newNode;
    }
}

void Drop::insertInNext(char data){
    Node* newNode = new Node;
    newNode->data = data;
    if(nextInLineHead == nullptr && nextInLineTail == nullptr){
        nextInLineHead = newNode;
        nextInLineTail = newNode;
    }
    else{
        nextInLineTail->right = newNode;
        newNode->left = nextInLineTail;
        nextInLineTail = newNode;
    }
}

// deletes from tail
char Drop::removeFromCurrent(){
    if (currentDropTail == nullptr){
        return ' ';
    }
    Node* temp = currentDropTail;
    currentDropTail = currentDropTail->left;
    if(currentDropTail == nullptr){
        currentDropHead = nullptr;
    }
    else{
        currentDropTail->right = nullptr;
    }
    char data = temp->data;
    delete temp;
    return data;
}

char Drop::removeFromNext(){
    if(nextInLineTail == nullptr){
        return ' ';
    }
    Node* temp = nextInLineTail;
    nextInLineTail = nextInLineTail->left;
    if(nextInLineTail == nullptr){
        nextInLineHead = nullptr;
    }
    else{
        nextInLineTail->right = nullptr;
    }
    char data = temp->data;
    delete temp;
    return data;
}


void Drop::updateTimer(){
    timer++;
}

void Drop::resetTimer(){
    timer = 0;
}

int Drop::getTimer() const{
    return timer;
}

void Drop::setDropCount(int dc){
    dropCount = dc;
}

int Drop::getDropCount() const{
    return dropCount;
}

void Drop::displayNextInLine(int r, int c) const{
    Node* current = nextInLineHead;
    while(current != nullptr){
        mvprintw(r,c,"%c",current->data);
        c += 2;
        current = current->right;
    }
}