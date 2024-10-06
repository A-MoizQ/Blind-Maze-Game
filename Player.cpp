#include "Player.h"
#include<random>

Player::Player(int u, int mc):undoCount(u),coins(nullptr),moveCount(mc),keyStatus(false),player(nullptr){}

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
        moveCount++; //not considering undo as a move
        moves.pop(); //also removing this undo move from moves stack
        undoCount--;
    }
}

//controls player movement
void Player::movePlayer(char c){
    //checks if move is up and up is not boundary point
    if (c == 'w' && player->up->up != nullptr){
        player->data = '.';
        player = player->up;
        player->data = 'P';
        moveCount--;
        moves.push(c);
    }
    //checks if move is down and down is not boundary point
    else if (c == 's' && player->down->down != nullptr){
        player->data = '.';
        player = player->down;
        player->data = 'P';
        moveCount--;
        moves.push(c);
    }
    //checks if move is left and left is not boundary point
    if (c == 'a' && player->left->left != nullptr){
        player->data = '.';
        player = player->left;
        player->data = 'P';
        moveCount--;
        moves.push(c);
    }
    //checks if move is right and right is not boundary point
    if (c == 'd' && player->right->right != nullptr){
        player->data = '.';
        player = player->right;
        player->data = 'P';
        moveCount--;
        moves.push(c);
    }
}

Player::~Player(){
    
}