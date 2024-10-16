#include "Player.h"
#include<random>
#include<ncurses.h>
#include<unistd.h>

Player::Player():undoCount(0),coins(nullptr),moveCount(0),keyRow(0),keyCol(0),doorRow(0),doorCol(0),size(0),keyStatus(false),doorStatus(false),player(nullptr),score(0){}
Player::~Player(){
    Node* current = coins;
    while(current != nullptr){
        Node* temp = current;
        delete[] current->coords;
        current = current->right;
        delete temp;
    }
}
//initializes 2 random coords to the player
int* Player::initializePlayerCoords(int s) const{
    int *coords = new int[2];
    srand(time(0));
    coords[0] = (rand() % (s-2)) + 2;
    coords[1] = (rand() % (s-2)) + 2;
    return coords;
}

void Player::setKeyAndDoor(int kr, int kc, int dr, int dc, int s){
    keyRow = kr;
    keyCol = kc;
    doorRow = dr;
    doorCol = dc;
    size = s;
}


int Player::getMoveCount() const{
    return moveCount;
}

int Player::getUndoCount() const{
    return undoCount;
}

void Player::setMoveCount(int mc){
    moveCount = mc;
}

void Player::setUndoCount(int u){
    undoCount = u;
}

void Player::displayCoins(int r, int c) const{
    Node* current = coins;
    while(current != nullptr){
        if(current->coords != nullptr){
            mvprintw(r,c,"(%i,%i,%c),",current->coords[0],current->coords[1],current->data);
        }
        c += 10;
        current = current->right;
    }
}

bool Player::checkCoin(int r, int c) const{
    Node* current = coins;
    while(current != nullptr){
        if(current->coords[0] == r && current->coords[1] == c){
            return true;
        }
        current = current->right;
    }
    return false;
}

char Player::generateDrop(int dc) const{
    int val = rand() % 100; //generates a value from 0 to 99
    if(dc == 4){
        //75% chance of coin and 25% chance of bomb for easy difficulty
        return (val < 75)?'C':'B';
    }
    else if(dc == 8){
        //50% chance for coin 50% chance for bomb for medium difficulty
        return (val < 50)?'C':'B';
    }
    else if(dc == 10){
        //25% chance for coin and 75% chance for bomb for hard difficulty
        return (val < 25)?'C':'B';
    }
    //punishment for not giving correct dropCount
    return 'B';
}

void Player::giveCoinToPlayer(int r, int c){
    Node* newNode = new Node;
    newNode->data = 'C';
    newNode->coords = new int[2];
    newNode->coords[0] = r;
    newNode->coords[1] = c;
    if(coins == nullptr){
        coins = newNode;
    }
    else{
        Node* current = coins;
        while(current->right != nullptr){
            current = current->right;
        }
        current->right = newNode;
    }
}

void Player::undo(){
    if(undoCount != 0 && !moves.isEmpty()){
        char lastMove = moves.pop();
        //inverting the last move
        if(lastMove == 'w'){
            lastMove = 's';
        }
        else if(lastMove == 's'){
            lastMove = 'w';
        }
        else if(lastMove == 'd'){
            lastMove = 'a';
        }
        else if(lastMove == 'a'){
            lastMove = 'd';
        }
        //do the move for undo without pushing it to the stack or decreasing move count
        movePlayer(lastMove,false);
        undoCount--;
    }
}

void Player::placeOnGrid(Node* placer, int r, int c, char val){
    //runs till coinPlacer reaches the row and col
    while(placer->coords[0] != r || placer->coords[1] != c){
        //goes down if row generated is greater than current row
        if(placer->coords[0] < r){
            placer = placer->down;
        }
        //goes up if row generated is smaller than current row
        if(placer->coords[0] > r){
            placer = placer->up;
        }
        //goes right if row generated is greater than current row
        if(placer->coords[1] < c){
            placer = placer->right;
        }
        //goes left if row generated is smaller than current row
        if(placer->coords[1] > c){
            placer = placer->left;
        }
    }
    //place what ever is next at the position
    placer->data = val;
}

//controls player movement
void Player::movePlayer(char c, bool pushToStack){
    //checks if move is up and up is not boundary point
    if (c == 'w' && player->up->up != nullptr && (!pushToStack || moves.seek() != 's')){
        if(!getKeyStatus() && getDoorStatus()){
            player->data = 'D';
            changeDoorStatus();
        }
        else{
            player->data = '.';
        }
        player = player->up;
        //if player is on key change key status to true
        if(player->data == 'K'){
            changeKeyStatus();
        }
        else if(player->data == 'D'){
            changeDoorStatus();
        }
        else if(player->data == 'C'){
            score += 2;
            undoCount += 1;
            //removes coin from inventory
            inv.pickUpDrop(player->coords[0],player->coords[1]);
            //adds to player's coins list
            giveCoinToPlayer(player->coords[0],player->coords[1]);
            //get coords of old drop
            int* dropRows = inv.getDropRows();
            int* dropCols = inv.getDropCols();
            //remove from the next in line and add to current
            inv.insertInCurrent(inv.removeFromNext());
            //generate a new drop and insert in next in line list
            inv.insertInNext(generateDrop(inv.getDropCount()));

            int r = 0, c = 0;
            //generate new coords for drop
            inv.generateDropCoords(r,c,keyRow,keyCol,doorRow,doorCol,player->coords[0],player->coords[1],size,dropRows,dropCols,inv.getDropCount());
            inv.setDropCoords(r,c);
            delete[] dropRows;
            delete[] dropCols;
            //a pointer to place next in line on board
            Node* placer = player;
            //place on grid
            placeOnGrid(placer,r,c, inv.seekCurrentTail());
            //reset timer
            inv.resetTimer();
        }
        else if(player->data == 'B'){
            walkToBombAnimation();
            scaryLaughAnimation();
            //ques a rock paper scissors game with an arbitrary person
            if(!rockPaperScissorsAnimation()){
                //if user loses he exploads
                bombExplosionAnimation();
                return;
            }
            //remove bomb from the grid and place a new item
            else{
                Node* placer = player;
                //removes bomb from inventory
                inv.pickUpDrop(player->coords[0],player->coords[1]);
                //get coords of old drop
                int* dropRows = inv.getDropRows();
                int* dropCols = inv.getDropCols();
                //remove from the next in line and add to current
                inv.insertInCurrent(inv.removeFromNext());
                //generate a new drop and insert in next in line list
                inv.insertInNext(generateDrop(inv.getDropCount()));

                int r = 0, c = 0;
                //generate new coords for drop
                inv.generateDropCoords(r,c,keyRow,keyCol,doorRow,doorCol,player->coords[0],player->coords[1],size,dropRows,dropCols,inv.getDropCount());
                inv.setDropCoords(r,c);
                delete[] dropRows;
                delete[] dropCols;
                placeOnGrid(placer,r,c,inv.seekCurrentTail());
            }
            
        }
        if(inv.getTimer() == 2){
            //a pointer to place next in line on board
            Node* placer = player;
            inv.resetTimer();
            int* dropRows = inv.getDropRows();
            int* dropCols = inv.getDropCols();
            //replace '.' with what ever is on the head of current
            placeOnGrid(placer,dropRows[0],dropCols[0],'.');
            //delete memory allocated
            delete[] dropRows; delete[] dropCols;
            inv.removeFromCurrent();
            //get coords after removing item from current
            dropRows = inv.getDropRows();
            dropCols = inv.getDropCols();
            inv.insertInCurrent(inv.removeFromNext());
            inv.insertInNext(generateDrop(inv.getDropCount()));
            int r = 0, c = 0;
            inv.generateDropCoords(r,c,keyRow,keyCol,doorRow,doorCol,player->coords[0],player->coords[1],size,dropRows,dropCols,inv.getDropCount());
            inv.setDropCoords(r,c);
            delete[] dropRows; delete[] dropCols;
            //place on grid
            placeOnGrid(placer,r,c,inv.seekCurrentTail());
            
        }
        player->data = 'P';
        if(pushToStack){
            moveCount--;
            moves.push(c);
            inv.updateTimer();
        }
    }
    //checks if move is down and down is not boundary point
    else if (c == 's' && player->down->down != nullptr && (!pushToStack || moves.seek() != 'w')){
        if(!getKeyStatus() && getDoorStatus()){
            player->data = 'D';
            changeDoorStatus();
        }
        else{
            player->data = '.';
        }
        player = player->down;
        //if player is on key change key status to true
        if(player->data == 'K'){
            changeKeyStatus();
        }
        else if(player->data == 'D'){
            changeDoorStatus();
        }
        else if(player->data == 'C'){
            score += 2;
            undoCount += 1;
            //removes coin from inventory
            inv.pickUpDrop(player->coords[0],player->coords[1]);
            //adds to player's coins list
            giveCoinToPlayer(player->coords[0],player->coords[1]);
            //generate a new drop and insert in next in line list
            inv.insertInNext(generateDrop(inv.getDropCount()));
            //remove from the next in line and add to current
            inv.insertInCurrent(inv.removeFromNext());
            int r = 0, c = 0;
            //generate coords for new drop
            int* dropRows = inv.getDropRows();
            int* dropCols = inv.getDropCols();
            inv.generateDropCoords(r,c,keyRow,keyCol,doorRow,doorCol,player->coords[0],player->coords[1],size,dropRows,dropCols,inv.getDropCount());
            inv.setDropCoords(r,c);
            delete[] dropRows;
            delete[] dropCols;
            //a pointer to place next in line on board
            Node* placer = player;
            //place on grid
            placeOnGrid(placer,r,c,inv.seekCurrentTail());
            //resets the timer considering picking up a drop
            inv.resetTimer();
        }
        else if(player->data == 'B'){
            walkToBombAnimation();
            scaryLaughAnimation();
            //ques a rock paper scissors game with an arbitrary person
            if(!rockPaperScissorsAnimation()){
                //if user loses he exploads
                bombExplosionAnimation();
                return;
            }
            //remove bomb from the grid and place a new item
            else{
                Node* placer = player;
                //removes bomb from inventory
                inv.pickUpDrop(player->coords[0],player->coords[1]);
                //get coords of old drop
                int* dropRows = inv.getDropRows();
                int* dropCols = inv.getDropCols();
                //remove from the next in line and add to current
                inv.insertInCurrent(inv.removeFromNext());
                //generate a new drop and insert in next in line list
                inv.insertInNext(generateDrop(inv.getDropCount()));

                int r = 0, c = 0;
                //generate new coords for drop
                inv.generateDropCoords(r,c,keyRow,keyCol,doorRow,doorCol,player->coords[0],player->coords[1],size,dropRows,dropCols,inv.getDropCount());
                inv.setDropCoords(r,c);
                delete[] dropRows;
                delete[] dropCols;
                placeOnGrid(placer,r,c,inv.seekCurrentTail());
            }
            
        }
        if(inv.getTimer() == 2){
            //a pointer to place next in line on board
            Node* placer = player;
            inv.resetTimer();
            int* dropRows = inv.getDropRows();
            int* dropCols = inv.getDropCols();
            //replace '.' with what ever is on the head of current
            placeOnGrid(placer,dropRows[0],dropCols[0],'.');
            //delete memory allocated
            delete[] dropRows; delete[] dropCols;
            inv.removeFromCurrent();
            //get coords after removing item from current
            dropRows = inv.getDropRows();
            dropCols = inv.getDropCols();
            inv.insertInCurrent(inv.removeFromNext());
            inv.insertInNext(generateDrop(inv.getDropCount()));
            int r = 0, c = 0;
            inv.generateDropCoords(r,c,keyRow,keyCol,doorRow,doorCol,player->coords[0],player->coords[1],size,dropRows,dropCols,inv.getDropCount());
            inv.setDropCoords(r,c);
            delete[] dropRows; delete[] dropCols;
            //place on grid
            placeOnGrid(placer,r,c,inv.seekCurrentTail());
        }
        player->data = 'P';
        if(pushToStack){
            moveCount--;
            moves.push(c);
            inv.updateTimer();
        }
    }
    //checks if move is left and left is not boundary point
    if (c == 'a' && player->left->left != nullptr &&(!pushToStack || moves.seek() != 'd')){
        if(!getKeyStatus() && getDoorStatus()){
            player->data = 'D';
            changeDoorStatus();
        }
        else{
            player->data = '.';
        }
        player = player->left;
        //if player is on key change key status to true
        if(player->data == 'K'){
            changeKeyStatus();
        }
        else if(player->data == 'D'){
            changeDoorStatus();
        }
        else if(player->data == 'C'){
            score += 2;
            undoCount += 1;
            //removes coin from inventory
            inv.pickUpDrop(player->coords[0],player->coords[1]);
            //adds to player's coins list
            giveCoinToPlayer(player->coords[0],player->coords[1]);
            //generate a new drop and insert in next in line list
            inv.insertInNext(generateDrop(inv.getDropCount()));
            //remove from the next in line and add to current
            inv.insertInCurrent(inv.removeFromNext());
            int r = 0, c = 0;
            //generate coords for new drop
            int* dropRows = inv.getDropRows();
            int* dropCols = inv.getDropCols();
            inv.generateDropCoords(r,c,keyRow,keyCol,doorRow,doorCol,player->coords[0],player->coords[1],size,dropRows,dropCols,inv.getDropCount());
            inv.setDropCoords(r,c);
            delete[] dropRows;
            delete[] dropCols;
            //a pointer to place next in line on board
            Node* placer = player;
            //place on grid
            placeOnGrid(placer,r,c,inv.seekCurrentTail());
            //reset timer
            inv.resetTimer();
        }
        else if(player->data == 'B'){
            walkToBombAnimation();
            scaryLaughAnimation();
            //ques a rock paper scissors game with an arbitrary person
            if(!rockPaperScissorsAnimation()){
                //if user loses he exploads
                bombExplosionAnimation();
                return;
            }
            //remove bomb from the grid and place a new item
            else{
                Node* placer = player;
                //removes bomb from inventory
                inv.pickUpDrop(player->coords[0],player->coords[1]);
                //get coords of old drop
                int* dropRows = inv.getDropRows();
                int* dropCols = inv.getDropCols();
                //remove from the next in line and add to current
                inv.insertInCurrent(inv.removeFromNext());
                //generate a new drop and insert in next in line list
                inv.insertInNext(generateDrop(inv.getDropCount()));

                int r = 0, c = 0;
                //generate new coords for drop
                inv.generateDropCoords(r,c,keyRow,keyCol,doorRow,doorCol,player->coords[0],player->coords[1],size,dropRows,dropCols,inv.getDropCount());
                inv.setDropCoords(r,c);
                delete[] dropRows;
                delete[] dropCols;
                placeOnGrid(placer,r,c,inv.seekCurrentTail());
            }
            
        }
        if(inv.getTimer() == 2){
            //a pointer to place next in line on board
            Node* placer = player;
            inv.resetTimer();
            int* dropRows = inv.getDropRows();
            int* dropCols = inv.getDropCols();
            //replace '.' with what ever is on the head of current
            placeOnGrid(placer,dropRows[0],dropCols[0],'.');
            //delete memory allocated
            delete[] dropRows; delete[] dropCols;
            inv.removeFromCurrent();
            //get coords after removing item from current
            dropRows = inv.getDropRows();
            dropCols = inv.getDropCols();
            inv.insertInCurrent(inv.removeFromNext());
            inv.insertInNext(generateDrop(inv.getDropCount()));
            int r = 0, c = 0;
            inv.generateDropCoords(r,c,keyRow,keyCol,doorRow,doorCol,player->coords[0],player->coords[1],size,dropRows,dropCols,inv.getDropCount());
            inv.setDropCoords(r,c);
            delete[] dropRows; delete[] dropCols;
            //place on grid
            placeOnGrid(placer,r,c,inv.seekCurrentTail());
        }
        player->data = 'P';
        if(pushToStack){
            moveCount--;
            moves.push(c);
            inv.updateTimer();
        }
    }
    //checks if move is right and right is not boundary point
    if (c == 'd' && player->right->right != nullptr && (!pushToStack || moves.seek() != 'a')){
        if(!getKeyStatus() && getDoorStatus()){
            player->data = 'D';
            changeDoorStatus();
        }
        else{
            player->data = '.';
        }
        player = player->right;
        //if player is on key change key status to true
        if(player->data == 'K'){
            changeKeyStatus();
        }
        else if(player->data == 'D'){
            changeDoorStatus();
        }
        else if(player->data == 'C'){
            score += 2;
            undoCount += 1;
            //removes coin from inventory
            inv.pickUpDrop(player->coords[0],player->coords[1]);
            //adds to player's coins list
            giveCoinToPlayer(player->coords[0],player->coords[1]);
            //generate a new drop and insert in next in line list
            inv.insertInNext(generateDrop(inv.getDropCount()));
            //remove from the next in line and add to current
            inv.insertInCurrent(inv.removeFromNext());
            int r = 0, c = 0;
            //generate coords for new drop
            int* dropRows = inv.getDropRows();
            int* dropCols = inv.getDropCols();
            inv.generateDropCoords(r,c,keyRow,keyCol,doorRow,doorCol,player->coords[0],player->coords[1],size,dropRows,dropCols,inv.getDropCount());
            inv.setDropCoords(r,c);
            delete[] dropRows;
            delete[] dropCols;
            //a pointer to place next in line on board
            Node* placer = player;
            //place on grid
            placeOnGrid(placer,r,c,inv.seekCurrentTail());
            //reset timer
            inv.resetTimer();
        }
        else if(player->data == 'B'){
            walkToBombAnimation();
            scaryLaughAnimation();
            //ques a rock paper scissors game with an arbitrary person
            if(!rockPaperScissorsAnimation()){
                //if user loses he exploads
                bombExplosionAnimation();
                return;
            }
            //remove bomb from the grid and place a new item
            else{
                Node* placer = player;
                //removes bomb from inventory
                inv.pickUpDrop(player->coords[0],player->coords[1]);
                //get coords of old drop
                int* dropRows = inv.getDropRows();
                int* dropCols = inv.getDropCols();
                //remove from the next in line and add to current
                inv.insertInCurrent(inv.removeFromNext());
                //generate a new drop and insert in next in line list
                inv.insertInNext(generateDrop(inv.getDropCount()));

                int r = 0, c = 0;
                //generate new coords for drop
                inv.generateDropCoords(r,c,keyRow,keyCol,doorRow,doorCol,player->coords[0],player->coords[1],size,dropRows,dropCols,inv.getDropCount());
                inv.setDropCoords(r,c);
                delete[] dropRows;
                delete[] dropCols;
                placeOnGrid(placer,r,c,inv.seekCurrentTail());
            }
            
        }
        if(inv.getTimer() == 2){
            //a pointer to place next in line on board
            Node* placer = player;
            inv.resetTimer();
            int* dropRows = inv.getDropRows();
            int* dropCols = inv.getDropCols();
            //replace '.' with what ever is on the head of current
            placeOnGrid(placer,dropRows[0],dropCols[0],'.');
            //delete memory allocated
            delete[] dropRows; delete[] dropCols;
            inv.removeFromCurrent();
            //get coords after removing item from current
            dropRows = inv.getDropRows();
            dropCols = inv.getDropCols();
            inv.insertInCurrent(inv.removeFromNext());
            inv.insertInNext(generateDrop(inv.getDropCount()));
            int r = 0, c = 0;
            inv.generateDropCoords(r,c,keyRow,keyCol,doorRow,doorCol,player->coords[0],player->coords[1],size,dropRows,dropCols,inv.getDropCount());
            inv.setDropCoords(r,c);
            delete[] dropRows; delete[] dropCols;
            //place on grid
            placeOnGrid(placer,r,c,inv.seekCurrentTail());
        }
        player->data = 'P';
        if(pushToStack){
            moveCount--;
            moves.push(c);
            inv.updateTimer();
        }
    }
}

//changes key status when key is found
void Player::changeKeyStatus(){
    keyStatus = true;
}

bool Player::getKeyStatus() const{
    return keyStatus;
}

void Player::changeDoorStatus(){
    doorStatus = !doorStatus;
}

bool Player::getDoorStatus() const{
    return doorStatus;
}

int Player::getScore() const{
    return score;
}

void Player::setScore(int s){
    score = s;
}

char Player::getData() const{
    return player->data;
}

void Player::walkToBombAnimation() const{
    int row = 10, col = 30;
    char person = '@';
    char bomb = '*';

    clear();  // Clear the screen for the animation
    mvprintw(row, col, "%c", bomb);  // Draw the bomb at (row, col)
    
    // The person starts a few rows below the bomb and walks up step by step
    for (int i = 5; i < row; ++i) {
        mvprintw(i, col, "%c", person);  // Draw person in new position
        refresh();
        usleep(300000);  // Pause for 300 milliseconds for smooth movement

        if (i < row-1) {
            mvprintw(i, col, " ");  // Clear the old position of the person
        }
    }

    refresh();
    usleep(500000);  // Pause for dramatic effect
}

void Player::scaryLaughAnimation() const{
    int row = 5, col = 10;
    clear();
    // Draw initial scary face
    mvprintw(row, col, "    _____");
    mvprintw(row + 1, col, "   /     \\");
    mvprintw(row + 2, col, "  |  O O  |");
    mvprintw(row + 3, col, "   \\_____/");
    mvprintw(row + 4, col, "    | | |");

    // Add the first scary message
    mvprintw(row + 6, col, "Oh, you stepped on a bomb");
    refresh();
    usleep(1000000);  // Pause for a second

    mvprintw(row + 7, col, "HAHAHAHAHA!!!");
    refresh();
    usleep(1000000);  // Pause for another second

    refresh();
    usleep(500000);  // Quick pause before the transformation
    // Clear the initial message to update with the next part
    clear();
    // Change the character into a scarier look
    mvprintw(row, col, "    _____");
    mvprintw(row + 1, col, "   /XXXXX\\");
    mvprintw(row + 2, col, "  | > O < |");
    mvprintw(row + 3, col, "   \\_____/");
    mvprintw(row + 4, col, "    | | |");

    // Add the new scary message
    mvprintw(row + 6, col, "Let's have some fun!");
    refresh();
    usleep(1000000);  // Pause for effect

    mvprintw(row + 7, col, "HAHAHAHAHAHA!!!");
    refresh();
    usleep(1000000);  // Pause for another second

    // Final refresh after animation
    refresh();

}

bool Player::rockPaperScissorsAnimation() const {
    int row = 5, col = 10;
    clear();

    // Draw scary face
    mvprintw(row, col, "    _____");
    mvprintw(row + 1, col, "   /XXXXX\\");
    mvprintw(row + 2, col, "  | > O < |");
    mvprintw(row + 3, col, "   \\_____/");
    mvprintw(row + 4, col, "    | | |");

    // Add initial message
    mvprintw(row + 6, col, "Let's play Rock, Paper, Scissors!");
    refresh();
    usleep(1000000);  // Pause for a second

    // Display player choices
    mvprintw(row + 8, col, "Choose: 1) Rock 2) Paper 3) Scissors");
    refresh();

    // Seed for random choice
    srand(time(0));

    // Wait for user input
    int playerChoice = getch() - '0';  // Convert input from char to int
    if (playerChoice < 1 || playerChoice > 3) {
        playerChoice = 1;  // Default to Rock if invalid input
    }

    // Scary guy's random choice
    int scaryChoice = (rand() % 3) + 1;

    // Clear screen for results
    clear();
    // Draw scary face
    mvprintw(row, col, "    _____");
    mvprintw(row + 1, col, "   /XXXXX\\");
    mvprintw(row + 2, col, "  | > O < |");
    mvprintw(row + 3, col, "   \\_____/");
    mvprintw(row + 4, col, "    | | |");
    refresh();
    // usleep(500000);

    // Show both choices
    const char* choices[] = { "", "Rock", "Paper", "Scissors" };  // Index 0 is empty for convenience
    mvprintw(row + 8, col, "You chose: %s", choices[playerChoice]);
    mvprintw(row + 9, col, "Scary guy chose: %s", choices[scaryChoice]);
    refresh();
    usleep(1000000);  // Pause for effect

    // Determine the result
    if (playerChoice == scaryChoice) {
        mvprintw(row + 11, col, "It's a Draw!");
        refresh();
        usleep(2000000);  // Pause to show result
        return true;
    } else if ((playerChoice == 1 && scaryChoice == 3) ||  // Rock beats Scissors
               (playerChoice == 2 && scaryChoice == 1) ||  // Paper beats Rock
               (playerChoice == 3 && scaryChoice == 2)) {  // Scissors beats Paper
        mvprintw(row + 11, col, "You Win!");
        refresh();
        usleep(2000000);  // Pause to show result
        return true;
    } else {
        mvprintw(row + 11, col, "You Lose! HAHAHA!");
        refresh();
        usleep(2000000);  // Pause to show result
        return false;
    }

}

void Player::bombExplosionAnimation() const{
    int row = 10, col = 30;

    // Explosion animation: expand explosion with characters to simulate a blast
    char explosionChars[] = { '@', '*', '%', '#', ' ' };  // Starts with the person disintegrating
    clear();

    // Step 1: Show person standing on bomb
    mvprintw(row + 1, col, "*");  // Bomb
    mvprintw(row, col, "@");  // Person one row above the bomb
    refresh();
    usleep(500000);  // Pause for dramatic effect before explosion starts

    // Step 2: Move person onto the bomb
    mvprintw(row , col, " ");  // Clear old position of the person
    mvprintw(row + 1, col, "@");  // Person steps on the bomb
    refresh();
    usleep(500000);

    // Step 3: Explosion expands and disintegrates the person
    for (int step = 0; step < 5; ++step) {
        char blastChar = explosionChars[step];

        // Explosion expanding in all directions from the bomb
        mvprintw(row, col, "%c", blastChar);        // Person disintegrating into the bomb
        mvprintw(row, col - 1, "%c", blastChar);    // Left of bomb
        mvprintw(row, col + 1, "%c", blastChar);    // Right of bomb
        mvprintw(row - 1, col, "%c", blastChar);    // Above bomb
        mvprintw(row + 1, col, "%c", blastChar);    // Below bomb
        mvprintw(row - 1, col - 1, "%c", blastChar);  // Top-left
        mvprintw(row - 1, col + 1, "%c", blastChar);  // Top-right
        mvprintw(row + 1, col - 1, "%c", blastChar);  // Bottom-left
        mvprintw(row + 1, col + 1, "%c", blastChar);  // Bottom-right

        refresh();
        usleep(300000);  // Pause for smooth explosion transition
    }

    // Step 4: Final cleanup after explosion animation
    mvprintw(row, col - 1, " ");
    mvprintw(row, col + 1, " ");
    mvprintw(row - 1, col, " ");
    mvprintw(row + 1, col, " ");
    mvprintw(row - 1, col - 1, " ");
    mvprintw(row - 1, col + 1, " ");
    mvprintw(row + 1, col - 1, " ");
    mvprintw(row + 1, col + 1, " ");
    mvprintw(row, col, " ");  // Clear the bomb/person location

    refresh();

}