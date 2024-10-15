#include "Drop.h"
#include<random>

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
    dropCount++;
}

char Drop::seekCurrentTail() const{
    if(currentDropTail != nullptr){
        return currentDropTail->data;
    }
    else{
        return ' ';
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

// deletes from head
char Drop::removeFromCurrent(){
    if (currentDropHead == nullptr){
        return ' ';
    }
    Node* temp = currentDropHead;
    currentDropHead = currentDropHead->right;
    if(currentDropHead == nullptr){
        currentDropTail = nullptr;
    }
    else{
        currentDropHead->left = nullptr;
    }
    dropCount--;
    char data = temp->data;
    delete[] temp->coords;
    delete temp;
    return data;
}

char Drop::removeFromNext(){
    if(nextInLineHead == nullptr){
        return ' ';
    }
    Node* temp = nextInLineHead;
    nextInLineHead = nextInLineHead->right;
    if(nextInLineHead == nullptr){
        nextInLineTail = nullptr;
    }
    else{
        nextInLineHead->left = nullptr;
    }
    char data = temp->data;
    delete[] temp->coords;
    delete temp;
    return data;
}

void Drop::generateDropCoords(int& dropRow, int& dropCol, int& keyRow, int& keyCol, int& doorRow, int& doorCol, int& playerRow, int& playerCol, int size, int* dropRows, int* dropCols, int dropCount){
    bool valid;
    do{
        valid = true;
        dropRow = (rand() % (size-2)) + 2;
        dropCol = (rand() % (size-2)) + 2;
        //check with door,player and key
        if((dropRow == playerRow && dropCol == playerCol) || (dropRow == keyRow && dropCol == keyCol) || (dropRow == doorRow && dropCol == doorCol)){
            valid = false;
        }
        //check with previous entries
        for(int j = 0; j < dropCount; j++){
            if(dropRow == dropRows[j] && dropCol == dropCols[j]){
                valid = false;
                break;
            }
        }
    }while(!valid);
}
    
void Drop::pickUpDrop(int r, int c){
    Node* current = currentDropHead;
    while(current != nullptr){
        if(current->coords[0] == r && current->coords[1] == c){
            //checking for head
            if(current == currentDropHead){
                currentDropHead = current->right;
                if(currentDropHead != nullptr){
                    currentDropHead->left = nullptr;
                }  
            }
            else{
                current->left->right = current->right;
            }
            //checking for tail
            if(current == currentDropTail){
                currentDropTail = current->left;
                if(currentDropTail != nullptr){
                    currentDropTail->right = nullptr;
                }
            }
            else{
                current->right->left = current->left;
            }
            dropCount--;
            delete[] current->coords;
            delete current;
            break;
        }
        current = current->right;
    }
}

int* Drop::getDropRows() const{
    int *drop = new int[dropCount];
    Node* current = currentDropHead;
    int i = 0;
    while(i < dropCount  && current != nullptr && current->coords != nullptr){
        drop[i] = current->coords[0];
        i++; 
        current = current->right;
    }
    return drop;
}

int* Drop::getDropCols() const{
    int *drop = new int[dropCount];
    Node* current = currentDropHead;
    int i = 0;
    while(i < dropCount && current != nullptr && current->coords != nullptr){
        drop[i] = current->coords[1];
        i++; 
        current = current->right;
    }
    return drop;
}

void Drop::setDropCoords(int r, int c){
    if(currentDropTail != nullptr){
        if(currentDropTail->coords == nullptr){
            currentDropTail->coords = new int[2];
        }
        currentDropTail->coords[0] = r;
        currentDropTail->coords[1] = c;
    }
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