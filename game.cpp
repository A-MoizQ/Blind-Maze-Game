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
    Grid g(10);
    do{
        g.display();
        input = getch();
        g.movePlayer(input);
    }while(input != 'q');
    
    //clears up allocated memory by ncurses  
    endwin();
    return 0;
}