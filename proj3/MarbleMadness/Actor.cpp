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

void Avatar::doSomething(){
    int ch;
    if (getWorld()->getKey(ch)) {
        Actor* objInDest;
        switch (ch) {
            case KEY_PRESS_LEFT:
                setDirection(left);
                objInDest = getWorld()->actorAt(getX()-1, getY());
                if(getWorld()->emptySpace(getX()-1, getY()))
                    moveTo(getX() - 1, getY());
                else if(objInDest != nullptr && objInDest->canBePushed() && objInDest->push(left))
                    moveTo(getX() - 1, getY());
                break;
            case KEY_PRESS_RIGHT:
                objInDest = getWorld()->actorAt(getX() + 1, getY());
                setDirection(right);
                if(getWorld()->emptySpace(getX() + 1, getY()))
                    moveTo(getX() + 1, getY());
                else if(objInDest != nullptr && objInDest->canBePushed() && objInDest->push(right))
                    moveTo(getX() + 1, getY());
                break;
            case KEY_PRESS_UP:
                setDirection(up);
                objInDest = getWorld()->actorAt(getX(), getY() + 1);
                if(getWorld()->emptySpace(getX(), getY() + 1))
                   moveTo(getX(), getY() + 1);
                else if(objInDest != nullptr && objInDest->canBePushed() && objInDest->push(up))
                    moveTo(getX(), getY() + 1);
                break;
            case KEY_PRESS_DOWN:
                setDirection(down);
                objInDest = getWorld()->actorAt(getX(), getY() - 1);
                if(getWorld()->emptySpace(getX(), getY() - 1))
                    moveTo(getX(), getY() - 1);
                else if(objInDest != nullptr && objInDest->canBePushed() && objInDest->push(down))
                    moveTo(getX(), getY() - 1);
                break;
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
    if(dir == right)
        newX++;
    else if(dir == left)
        newX--;
    else if(dir == up)
        newY++;
    else if(dir == down)
        newY--;
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
    Actor* objAtPit = getWorld()->actorAt(getX(), getY());
    if(objAtPit->canBePushed()){
        setDead();
        objAtPit->setDead();
    }
}
