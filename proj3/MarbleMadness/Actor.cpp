#include "Actor.h"
#include "StudentWorld.h"
#include "GameConstants.h"
#include "GraphObject.h"

//MARK: ACTOR
Actor::Actor(int ID, int startX, int startY, int startDir, StudentWorld* wrld)
: GraphObject(ID, startX, startY, startDir), wrld(wrld), alive(true){
    setVisible(true);
}

Actor::~Actor(){
}

// MARK: AVATAR
Avatar::Avatar(int startX, int startY, StudentWorld* wrld)
: Actor(IID_PLAYER, startX, startY, right, wrld), peas(0), hitPoints(0) {
}

Avatar::~Avatar(){
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

void Avatar::doSomething(){
    int ch;
    if (getWorld()->getKey(ch)) {
        switch(ch){
            case KEY_PRESS_SPACE:
                /* if(peas > 0){
                    shootPea();
                    peas--;
                } */
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

// MARK: WALL
Wall::Wall(int startX, int startY, StudentWorld* wrld)
: Actor(IID_WALL, startX, startY, none, wrld) {
}

Wall::~Wall(){
}

void Wall::doSomething(){
}

// MARK: MARBLE
Marble::Marble(int startX, int startY, StudentWorld* wrld)
: Actor(IID_MARBLE, startX, startY, none, wrld){
}

void Marble::doSomething(){
}

Marble::~Marble(){
}

bool Marble::canBePushed() const{
    return true;
}

bool Marble::push(int dir){
    int newX = getX();
    int newY = getY();
    getNewCoordinates(newX, newY, dir);
    if(getWorld()->emptySpace(newX, newY) || (getWorld()->actorAt(newX, newY))->canBePushedOn()){
        moveTo(newX, newY);
        return true;
    }
    return false;
}

// MARK: PIT
Pit::Pit(int startX, int startY, StudentWorld* wrld)
: Actor(IID_PIT, startX, startY, none, wrld) {
}

Pit::~Pit() {
}

bool Pit::canBePushedOn() const{
    return true;
}

void Pit::doSomething(){
    if(!isAlive())
        return;
    Actor* obj = getWorld()->actorAtSamePlace(getX(), getY(), this);
    if(obj != nullptr && obj->canBePushed()){
        obj->setDead();
        setDead();
    }
}
