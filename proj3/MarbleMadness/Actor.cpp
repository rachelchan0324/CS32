#include "Actor.h"
#include "StudentWorld.h"
#include "GameConstants.h"
#include "GraphObject.h"

// Students:  Add code to this file, Actor.h, StudentWorld.h, and StudentWorld.cpp

Actor::Actor(int ID, int startX, int startY, int startDir, StudentWorld* wrld)
: GraphObject(ID, startX, startY, startDir), wrld(wrld){
    setVisible(true);
}

Actor::~Actor(){
}

StudentWorld* Actor::getWorld(){
    return wrld;
}

Avatar::Avatar(int startX, int startY, StudentWorld* wrld)
: Actor(IID_PLAYER, startX, startY, right, wrld), peas(0), hitPoints(0) {
}

Avatar::~Avatar(){
}

bool Avatar::push(int dir){
    return false;
}

void Avatar::doSomething(){
    int ch;
    if (getWorld()->getKey(ch)) {
        switch (ch) {
            case KEY_PRESS_LEFT:
                setDirection(left);
                if(getWorld()->emptySpace(getX()-1, getY()))
                    moveTo(getX() - 1, getY());
                else if((*(getWorld()->actorAt(getX()-1, getY())))->push(left))
                    moveTo(getX() - 1, getY());
                break;
            case KEY_PRESS_RIGHT:
                setDirection(right);
                if(getWorld()->emptySpace(getX() + 1, getY()))
                    moveTo(getX() + 1, getY());
                else if((*(getWorld()->actorAt(getX() + 1, getY())))->push(right))
                    moveTo(getX() + 1, getY());
                break;
            case KEY_PRESS_UP:
                setDirection(up);
                if(getWorld()->emptySpace(getX(), getY() + 1))
                   moveTo(getX(), getY() + 1);
                else if((*(getWorld()->actorAt(getX(), getY() + 1)))->push(up))
                    moveTo(getX(), getY() + 1);
                break;
            case KEY_PRESS_DOWN:
                setDirection(down);
                if(getWorld()->emptySpace(getX(), getY() - 1))
                    moveTo(getX(), getY() - 1);
                else if((*(getWorld()->actorAt(getX(), getY() - 1)))->push(down))
                    moveTo(getX(), getY() - 1);
                break;
        }
    }
}

Wall::Wall(int startX, int startY, StudentWorld* wrld)
: Actor(IID_WALL, startX, startY, none, wrld) {
}

Wall::~Wall(){
}

bool Wall::push(int dir){
    return false;
}

void Wall::doSomething(){
}

Marble::Marble(int startX, int startY, StudentWorld* wrld)
: Actor(IID_MARBLE, startX, startY, none, wrld) {
}

void Marble::doSomething(){
}

Marble::~Marble(){
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
    if(getWorld()->emptySpace(newX, newY)){ // TODO: OR PIT
        moveTo(newX, newY);
        return true;
    }
    return false;
}

Pit::Pit(int startX, int startY, StudentWorld* wrld)
: Actor(IID_PIT, startX, startY, none, wrld) {
}

Pit::~Pit(){
}

bool Pit::push(int dir){
    return false;
}

void Pit::doSomething(){
    if(!alive)
        return;
}
