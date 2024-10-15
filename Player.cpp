#include "Player.h"
#include<random>

Player::Player():undoCount(0),coins(nullptr),moveCount(0),keyRow(0),keyCol(0),doorRow(0),doorCol(0),size(0),keyStatus(false),doorStatus(false),player(nullptr),score(0){}

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
            inv.pickUpCoin(player->coords[0],player->coords[1]);
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
            //a pointer to place coin on board
            Node* coinPlacer = player;
            //runs till coinPlacer reaches the row and col
            while(coinPlacer->coords[0] != r || coinPlacer->coords[1] != c){
                //goes down if row generated is greater than current row
                if(coinPlacer->coords[0] < r){
                    coinPlacer = coinPlacer->down;
                }
                //goes up if row generated is smaller than current row
                if(coinPlacer->coords[0] > r){
                    coinPlacer = coinPlacer->up;
                }
                //goes right if row generated is greater than current row
                if(coinPlacer->coords[1] < c){
                    coinPlacer = coinPlacer->right;
                }
                //goes left if row generated is smaller than current row
                if(coinPlacer->coords[1] > c){
                    coinPlacer = coinPlacer->left;
                }
            }
            //place what ever is next at the position
            coinPlacer->data = inv.seekCurrentTail();
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
            inv.pickUpCoin(player->coords[0],player->coords[1]);
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
            //a pointer to place coin on board
            Node* coinPlacer = player;
            //runs till coinPlacer reaches the row and col
            while(coinPlacer->coords[0] != r || coinPlacer->coords[1] != c){
                //goes down if row generated is greater than current row
                if(coinPlacer->coords[0] < r){
                    coinPlacer = coinPlacer->down;
                }
                //goes up if row generated is smaller than current row
                if(coinPlacer->coords[0] > r){
                    coinPlacer = coinPlacer->up;
                }
                //goes right if row generated is greater than current row
                if(coinPlacer->coords[1] < c){
                    coinPlacer = coinPlacer->right;
                }
                //goes left if row generated is smaller than current row
                if(coinPlacer->coords[1] > c){
                    coinPlacer = coinPlacer->left;
                }
            }
            //place whatever is next at the position
            coinPlacer->data = inv.seekCurrentTail();
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
            inv.pickUpCoin(player->coords[0],player->coords[1]);
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
            //a pointer to place coin on board
            Node* coinPlacer = player;
            //runs till coinPlacer reaches the row and col
            while(coinPlacer->coords[0] != r || coinPlacer->coords[1] != c){
                //goes down if row generated is greater than current row
                if(coinPlacer->coords[0] < r){
                    coinPlacer = coinPlacer->down;
                }
                //goes up if row generated is smaller than current row
                if(coinPlacer->coords[0] > r){
                    coinPlacer = coinPlacer->up;
                }
                //goes right if row generated is greater than current row
                if(coinPlacer->coords[1] < c){
                    coinPlacer = coinPlacer->right;
                }
                //goes left if row generated is smaller than current row
                if(coinPlacer->coords[1] > c){
                    coinPlacer = coinPlacer->left;
                }
            }
            //place whatever is next at the position
            coinPlacer->data = inv.seekCurrentTail();
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
            inv.pickUpCoin(player->coords[0],player->coords[1]);
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
            //a pointer to place coin on board
            Node* coinPlacer = player;
            //runs till coinPlacer reaches the row and col
            while(coinPlacer->coords[0] != r || coinPlacer->coords[1] != c){
                //goes down if row generated is greater than current row
                if(coinPlacer->coords[0] < r){
                    coinPlacer = coinPlacer->down;
                }
                //goes up if row generated is smaller than current row
                if(coinPlacer->coords[0] > r){
                    coinPlacer = coinPlacer->up;
                }
                //goes right if row generated is greater than current row
                if(coinPlacer->coords[1] < c){
                    coinPlacer = coinPlacer->right;
                }
                //goes left if row generated is smaller than current row
                if(coinPlacer->coords[1] > c){
                    coinPlacer = coinPlacer->left;
                }
            }
            //place whatever is next at the position
            coinPlacer->data = inv.seekCurrentTail();
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
