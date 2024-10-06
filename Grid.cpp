#include "Grid.h"
#include<ncurses.h>

Grid::Grid(int s){
    topLeft = nullptr;
    player = nullptr;
    Node* prevRow = nullptr;
    Node* rowStart = nullptr;
    Node* prev = nullptr;
    size = s;
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
            //assign current to previous for next iteration
            prev = current;
        }
        //give the previous row's start node to this pointer
        prevRow = rowStart;
        //reset prev pointer for current row
        prev = nullptr;
    }
}

void Grid::display() const{
    Node* currentRow = topLeft;
    int row = 0;
    while (currentRow != nullptr){
        Node* current = currentRow;
        Node* nextRow = current->down;
        int col = 0;
        while (current != nullptr){   
            //prints on the row/column of the terminal with current->data
            mvprintw(row,col,"%c",current->data);
            current = current->right;
            col += 2;
        }
        row += 1;
        currentRow = nextRow;
    }

    refresh();
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