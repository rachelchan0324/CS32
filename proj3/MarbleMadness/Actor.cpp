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

void Avatar::doSomething(){
    int ch;
    if (getWorld()->getKey(ch)) {
        switch (ch) {
            case KEY_PRESS_LEFT:
                setDirection(left);
                if(getWorld()->emptySpace(getX()-1, getY()))
                   moveTo(getX() - 1, getY());
                break;
            case KEY_PRESS_RIGHT:
                setDirection(right);
                if(getWorld()->emptySpace(getX() + 1, getY()))
                   moveTo(getX() + 1, getY());
                break;
            case KEY_PRESS_UP:
                setDirection(up);
                if(getWorld()->emptySpace(getX(), getY() + 1))
                   moveTo(getX(), getY() + 1);
                break;
            case KEY_PRESS_DOWN:
                setDirection(down);
                if(getWorld()->emptySpace(getX(), getY() - 1))
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

void Wall::doSomething(){
}
