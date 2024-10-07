#include "Player.h"
#include<random>

Player::Player():undoCount(0),coins(nullptr),moveCount(0),keyStatus(false),player(nullptr){}

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

        movePlayer(lastMove);
        moveCount += 2; //not considering undo as a move and giving back player the move he undo
        moves.pop(); //also removing this undo move from moves stack
        undoCount--;
    }
}

//controls player movement
void Player::movePlayer(char c){
    //checks if move is up and up is not boundary point
    if (c == 'w' && player->up->up != nullptr && moves.seek() != 's'){
        player->data = '.';
        player = player->up;
        //if player is on key change key status to true
        if(player->data == 'K'){
            changeKeyStatus();
        }
        player->data = 'P';
        moveCount--;
        moves.push(c);
    }
    //checks if move is down and down is not boundary point
    else if (c == 's' && player->down->down != nullptr && moves.seek() != 'w'){
        player->data = '.';
        player = player->down;
        //if player is on key change key status to true
        if(player->data == 'K'){
            changeKeyStatus();
        }
        player->data = 'P';
        moveCount--;
        moves.push(c);
    }
    //checks if move is left and left is not boundary point
    if (c == 'a' && player->left->left != nullptr && moves.seek() != 'd'){
        player->data = '.';
        player = player->left;
        //if player is on key change key status to true
        if(player->data == 'K'){
            changeKeyStatus();
        }
        player->data = 'P';
        moveCount--;
        moves.push(c);
    }
    //checks if move is right and right is not boundary point
    if (c == 'd' && player->right->right != nullptr && moves.seek() != 'a'){
        player->data = '.';
        player = player->right;
        //if player is on key change key status to true
        if(player->data == 'K'){
            changeKeyStatus();
        }
        player->data = 'P';
        moveCount--;
        moves.push(c);
    }
}

//changes key status when key is found
void Player::changeKeyStatus(){
    keyStatus = true;
}

bool Player::getKeyStatus() const{
    return keyStatus;
}


int Player::getScore() const{
    return score;
}

void Player::setScore(int s){
    score = s;
}
