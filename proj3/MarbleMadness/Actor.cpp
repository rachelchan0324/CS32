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

void Avatar::shootPea(){
    int peaX = getX();
    int peaY = getY();
    getNewCoordinates(peaX, peaY, getDirection());
    getWorld()->addPea(peaX, peaY, getDirection());
    getWorld()->playSound(SOUND_PLAYER_FIRE);
}

bool Avatar::damage(){
    decHitPoints(2);
    if(getHitPoints() <= 0)
        setDead();
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

bool Marble::damage(){
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
        
        Actor* obj = getWorld()->actorAtSamePlace(this);
        // damage marble, robot or player
        if(obj != nullptr && (obj->canBePushed() || obj==getWorld()->getPlayer()) && obj->damage()){
            setDead();
            return;
        }
        // pea dies if it hits a wall or robot factory
        if(obj != nullptr && obj->blocksMovement()){
            setDead();
            return;
        }
        // move 1 in the specified direction
        int newX = getX();
        int newY = getY();
        getNewCoordinates(newX, newY, getDirection());
        moveTo(newX, newY);
        
        /** check new square */
        obj = getWorld()->actorAtSamePlace(this);
        if(obj != nullptr && (obj->canBePushed() || obj==getWorld()->getPlayer()) && obj->damage())
            setDead();
        else if(obj != nullptr && obj->blocksMovement())
            setDead();
    }
    moved = true;
}
