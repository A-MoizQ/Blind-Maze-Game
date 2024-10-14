#include "Player.h"
#include<random>

Player::Player():undoCount(0),coins(nullptr),moveCount(0),keyStatus(false),doorStatus(false),player(nullptr),score(0){}

//initializes 2 random coords to the player
int* Player::initializePlayerCoords(int s) const{
    int *coords = new int[2];
    srand(time(0));
    coords[0] = (rand() % (s-2)) + 2;
    coords[1] = (rand() % (s-2)) + 2;
    return coords;
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
        player->data = 'P';
        if(pushToStack){
            moveCount--;
            moves.push(c);
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
        player->data = 'P';
        if(pushToStack){
            moveCount--;
            moves.push(c);
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
        player->data = 'P';
        if(pushToStack){
            moveCount--;
            moves.push(c);
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
        player->data = 'P';
        if(pushToStack){
            moveCount--;
            moves.push(c);
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
