#include "Actor.h"
#include "StudentWorld.h"
#include "GameConstants.h"
#include "GraphObject.h"

//MARK: ACTOR
Actor::Actor(int ID, int startX, int startY, int startDir, int hitPoints, StudentWorld* wrld)
: GraphObject(ID, startX, startY, startDir), wrld(wrld), alive(true), hitPoints(hitPoints){
    setVisible(true);
}

void Actor::getNewCoordinates(int& x, int& y, int dir){
    if(dir == right)
        x++;
    else if(dir == left)
        x--;
    else if(dir == up)
        y++;
    else
        y--;
}

void Actor::shootPea(){
    int peaX = getX();
    int peaY = getY();
    getNewCoordinates(peaX, peaY, getDirection());
    getWorld()->addPea(peaX, peaY, getDirection());
}


// MARK: AVATAR
Avatar::Avatar(int startX, int startY, StudentWorld* wrld)
: Actor(IID_PLAYER, startX, startY, right, 20, wrld), peas(20) {
}

void Avatar::doSomething(){
    int ch;
    if (getWorld()->getKey(ch)) {
        switch(ch){
            case KEY_PRESS_SPACE:
                if(peas > 0){
                    shootPea();
                    getWorld()->playSound(SOUND_PLAYER_FIRE);
                    peas--;
                }
                break;
            case KEY_PRESS_RIGHT:
            case KEY_PRESS_LEFT:
            case KEY_PRESS_UP:
            case KEY_PRESS_DOWN:
                Actor* objInDest;
                int newX = getX();
                int newY = getY();
                if(ch == KEY_PRESS_RIGHT)
                    setDirection(right);
                else if(ch == KEY_PRESS_LEFT)
                    setDirection(left);
                else if(ch == KEY_PRESS_UP)
                    setDirection(up);
                else
                    setDirection(down);
                getNewCoordinates(newX, newY, getDirection());
                objInDest = getWorld()->actorAt(newX, newY);
                if(getWorld()->emptySpace(newX, newY))
                    moveTo(newX, newY);
                else if(objInDest != nullptr && objInDest->canBePushed() && objInDest->push(getDirection()))
                    moveTo(newX, newY);
        }
    }
}

bool Avatar::getDamaged(){
    decHitPoints(2);
    if(getHitPoints() <= 0){
        setDead();
        getWorld()->playSound(SOUND_PLAYER_DIE);
    }
    else
        getWorld()->playSound(SOUND_PLAYER_IMPACT);
    return true;
}

// MARK: WALL
Wall::Wall(int startX, int startY, StudentWorld* wrld)
: Actor(IID_WALL, startX, startY, none, -1, wrld) {
}

// MARK: MARBLE
Marble::Marble(int startX, int startY, StudentWorld* wrld)
: Actor(IID_MARBLE, startX, startY, none, 10, wrld){
}

void Marble::doSomething(){
}

bool Marble::getDamaged(){
    decHitPoints(2);
    if(getHitPoints() <= 0)
        setDead();
    return true;
}

bool Marble::push(int dir){
    int newX = getX();
    int newY = getY();
    getNewCoordinates(newX, newY, dir);
    if(getWorld()->emptySpace(newX, newY) || !(getWorld()->actorAt(newX, newY))->blocksMovement()){
        moveTo(newX, newY);
        return true;
    }
    return false;
}

// MARK: PIT
Pit::Pit(int startX, int startY, StudentWorld* wrld)
: Actor(IID_PIT, startX, startY, none, -1, wrld) {
}

void Pit::doSomething(){
    if(!isAlive())
        return;
    Actor* obj = getWorld()->actorAtSamePlace(this);
    if(obj != nullptr && obj->canBePushed()){
        obj->setDead();
        setDead();
    }
}

// MARK: PEA
Pea::Pea(int startX, int startY, int startDir, StudentWorld* wrld)
: Actor(IID_PEA, startX, startY, startDir, -1, wrld), moved(false){
}

void Pea::doSomething(){
    if(moved){
        if(!isAlive())
            return;
        
        if(getWorld()->doSomethingToActorsHitByPea(this))
            return;
        
        // move 1 in the specified direction
        int newX = getX();
        int newY = getY();
        getNewCoordinates(newX, newY, getDirection());
        moveTo(newX, newY);
        
        // check new square
        getWorld()->doSomethingToActorsHitByPea(this);
    }
    moved = true;
}

//MARK: RAGE BOT
RageBot::RageBot(int startX, int startY, int startDir, StudentWorld* wrld)
: Actor(IID_RAGEBOT, startX, startY, startDir, 10, wrld), currTick(1){
    ticks = (28 - getWorld()->getLevel()) / 4;
    if (ticks < 3)
        ticks = 3;
}

void RageBot::doSomething(){
    if(!isAlive())
        return;
    if(currTick == ticks){
        if((getWorld()->getPlayer()->getX() == getX() || getWorld()->getPlayer()->getY() == getY()) && facingPlayer() && !getWorld()->obstaclesBetweenActorAndPlayer(this)){
            shootPea();
            getWorld()->playSound(SOUND_ENEMY_FIRE);
        }
        else{
            int newX = getX();
            int newY = getY();
            getNewCoordinates(newX, newY, getDirection());
            if(!getWorld()->obstacleAt(newX, newY))
                moveTo(newX, newY);
            else{
                if(getDirection() == right)
                    setDirection(left);
                else if(getDirection() == left)
                    setDirection(right);
                else if(getDirection() == up)
                    setDirection(down);
                else
                    setDirection(up);
            }
        }
        currTick = 1;
    }
    else
        currTick++;
}

bool RageBot::facingPlayer(){
    int playerX = getWorld()->getPlayer()->getX();
    int playerY = getWorld()->getPlayer()->getY();
    if(getDirection() == right && getY() == playerY && getX() < playerX)
        return true;
    if(getDirection() == left && getY() == playerY && getX() > playerX)
        return true;
    if(getDirection() == up && getX() == playerX && getY() < playerY)
        return true;
    if(getDirection() == down && getX() == playerX && getY() > playerY)
        return true;
    return false;
}

bool RageBot::getDamaged(){
    decHitPoints(2);
    if(getHitPoints() <= 0){
        setDead();
        getWorld()->playSound(SOUND_ROBOT_DIE);
    }
    else{
        getWorld()->playSound(SOUND_ROBOT_IMPACT);
        // TODO: Inform the StudentWorld object that the user is to receive 100 more points.
    }
    return true;
}
