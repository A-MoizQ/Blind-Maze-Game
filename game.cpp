#include "Grid.h"
#include<ncurses.h>

using namespace std;

int main(){
    //initialize ncurses screen
    initscr();
    //removes cursor
    curs_set(0);  
    //does not print user entered characters onto the terminal            
    noecho();
    //removes line breaks                 
    cbreak();    
    //refreshes the terminal onto ncurses mode           
    refresh();
    //gameloop (for now)                
    char input;
    mvprintw(0,0,"Enter Difficulty (easy:e,medium:m,hard:h): ");
    input = getch();
    Grid g;
    char *mode;
    //for easy game mode
    if(input == 'e'){
        g.initializeGame(10);
        mode = "easy";
    }
    //for difficult mode
    else if(input == 'm'){
        g.initializeGame(15);
        mode = "medium";
    }
    //for hard mode
    else if(input == 'h'){
        g.initializeGame(20);
        mode = "hard";
    }
    //if user enters invalid then he is forced to play hard mode HEHEHEHEHEHE
    else{
        g.initializeGame(20);
        mode = "hard";
    }
    clear();
    do{
        int row = 0,col = 10;
        //prints game mode
        mvprintw(row,col,"Game Mode: %s",mode);
        //reset column for next row
        col = 0;
        //move row forward
        row += 2;
        //print move count
        mvprintw(row,col,"Move Count: %i",g.p.getMoveCount());
        //move column forward
        col += 20;
        //print undo count
        mvprintw(row,col,"Undos: %i",g.p.getUndoCount());
        //move row forward
        row += 2;
        //reset col for next row
        col = 0;
        //prints score
        mvprintw(row,col,"Score: %i",g.p.getScore());
        //move cols forward
        col += 20;
        //prints key status based on bool value
        mvprintw(row,col,"Key Status: %s",g.p.getKeyStatus()?"true":"false");
        //move row down
        row += 2;

        //reset col for displaying grid
        col = col/4;
        //displays grid
        g.display(row,col);
        //takes user input
        input = getch();
        //for undo
        if(input == 'u'){
            g.p.undo();
        }
        else{
            g.p.movePlayer(input);
        }
        clear();
    }while(input != 'q' && g.p.getMoveCount() >= 0);
    
    //clears up allocated memory by ncurses  
    endwin();
    return 0;
}