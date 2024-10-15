#include "Grid.h"
#include<random>
#include<ncurses.h>

Grid::Grid():topLeft(nullptr),key(nullptr),door(nullptr),size(0){}

void Grid::initializeGame(int s){
    Node* prevRow = nullptr;
    Node* rowStart = nullptr;
    Node* prev = nullptr;
    size = s;
    int dropCount;
    if(size == 10){
        dropCount = 4;
        p.inv.setDropCount(dropCount);
    }
    else if( size == 15){
        dropCount = 8;
        p.inv.setDropCount(dropCount);
    }
    else{
        dropCount = 10;
        p.inv.setDropCount(dropCount);
    }
    int *playerCoords = p.initializePlayerCoords(size); //initialize the coords to set player at
    srand(time(0));
    int keyRow,keyCol,doorRow,doorCol;
    int dropRow[dropCount],dropCol[dropCount];
    //set rows and cols of key and door to not be on each another
    do{
        keyRow = (rand() % (size-2)) + 2;
        keyCol = (rand() % (size-2)) + 2;
        doorRow = (rand() % (size-2)) + 2;
        doorCol = (rand() % (size-2)) + 2;
    }while((keyRow == playerCoords[0] && keyCol == playerCoords[1]) || (doorRow == keyRow && doorCol == keyCol ) || (doorRow == playerCoords[0] && doorCol == playerCoords[1]));
    //for generating new coords for drops in player class 
    p.setKeyAndDoor(keyRow,keyCol,doorRow,doorCol,size);
    //setting drop coordinates
    for(int i = 0; i < dropCount; i++){
        p.inv.generateDropCoords(dropRow[i],dropCol[i],keyRow,keyCol,doorRow,doorCol,playerCoords[0],playerCoords[1],size,dropRow,dropCol,i);
    }

    //start filling the grid
    for(int i = 1; i <= size; i++){
        for(int j = 1; j <= size; j++){
            Node* current = new Node;
            current->coords = new int[2];
            //store the coords of current node
            current->coords[0] = i;
            current->coords[1] = j;
            //make top and bottom boundary
            if( i == 1 || i == size){
                current->data = '#';
            }
            //make left and right boundary
            if(j == 1 || j == size){
                current->data = '#';
            }
            //check if its the first node of the list and keep a reference to it in top left pointer (denotes starting point of list)
            if (i == 1 && j == 1){
                topLeft = current;
            }
            //checks if the current node is rows starting node
            if (j == 1){
                rowStart = current;
            }
            //connect current node to previous if previous node exists
            if (prev != nullptr){
                prev->right = current;
                current->left = prev;
            }
            //connecting current row with upper row if row exists above
            if (prevRow != nullptr){
                prevRow->down = current;
                current->up = prevRow;
                prevRow = prevRow->right;
            }
            //if i and j match player row and col then set player pointer
            if( i == playerCoords[0] && j == playerCoords[1]){
                //point player pointer to current note to denote player is here
                p.player = current;
                current->data = 'P';
            }
            //put key on grid
            if(keyRow == i && keyCol == j){
                current->data = 'K';
                key = current;
            }
            //put door on grid
            if(doorRow == i && doorCol == j){
                current->data = 'D';
                door = current;
            }
            for(int k = 0 ; k < dropCount; k++){
                if(dropRow[k] == i && dropCol[k] == j){
                    current->data = p.generateDrop(dropCount);
                    p.inv.insertInCurrent(current->data);
                    p.inv.setDropCoords(i,j);
                }
            }

            //assign current to previous for next iteration
            prev = current;
        }
        //give the previous row's start node to this pointer
        prevRow = rowStart;
        //reset prev pointer for current row
        prev = nullptr;
    }
    //delete dynamically allocated memory to store playerCoords
    delete[] playerCoords;
    //setting move count according to player's distance from key and keys distance from door
    p.setMoveCount(calculateDistance(p.player,key) + calculateDistance(key,door));
    //setting undo count and adding additional moves based on size (which is correlated to difficulty)
    if(size == 10){
        p.setUndoCount(6);
        p.setMoveCount(p.getMoveCount() + 6);
    }
    else if(size == 15){
        p.setUndoCount(4);
        p.setMoveCount(p.getMoveCount() + 2);
    }
    else if(size == 20){
        p.setUndoCount(1);
    }
    for(int k = 0; k < dropCount ;k++){
        p.inv.insertInNext(p.generateDrop(dropCount));
    }
}

int Grid::calculateDistance(Node* A, Node* B) const{
    return (abs(A->coords[0] - B->coords[0]) + abs(A->coords[1] - B->coords[1]));
}

void Grid::display(int r, int c) const{
    Node* currentRow = topLeft;
    int row = r;
    while (currentRow != nullptr){
        Node* current = currentRow;
        Node* nextRow = current->down;
        int col = c;
        while (current != nullptr){
            //to hide key, door and bomb from printing
            if(current->data == 'K' || current->data == 'D' || current->data == 'B'){
                mvprintw(row,col,".");
            }   
            //prints on the row/column of the terminal with current->data
            else{
                mvprintw(row,col,"%c",current->data);
            }
            current = current->right;
            col += 2;
        }
        row += 1;
        currentRow = nextRow;
    }

    refresh();
}

void Grid::displayEndGame(int r, int c) const{
    Node* currentRow = topLeft;
    while(currentRow != nullptr){
        Node* current = currentRow;
        while(current != nullptr){
            if(p.checkCoin(r,c)){
                mvprintw(r,c,"C");
            }
            else if(key->coords[0] == r && key->coords[1] == c){
                mvprintw(r,c,"K");   
            }
            else if(door->coords[0] == r && door->coords[1] == c){
                mvprintw(r,c,"D");
            }
            else{
                mvprintw(r,c,"%c",current->data);
            }
            c += 2;
            current = current->right;
        }
        r += 1;
        c = 0;
        currentRow = currentRow->down;
    }
}


Node* Grid::getKey() const{
    return key;
}

Node* Grid::getDoor() const{
    return door;
}

bool Grid::closer(int dist1, int dist2) const{
    return dist1 < dist2;
}


Grid::~Grid(){
    //getting first row start node
    Node* currentRow = topLeft;
    while (currentRow != nullptr){
        //assigning first row start node to current
        Node* current = currentRow;
        //getting first node of next row
        Node* nextRow = current->down;
        //deleting an entire row
        while (current != nullptr){
            Node* next = current->right;
            delete[] current->coords;
            delete current;
            current = next;
        }
        //moving on to next row
        currentRow = nextRow;
    }
}