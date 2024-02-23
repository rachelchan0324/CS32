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
: Actor(IID_PLAYER, startX, startY, right, wrld), peas(20), hitPoints(0) {
}

Avatar::~Avatar(){
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
    getWorld()->playSound(SOUND_PLAYER_FIRE);
    getWorld()->addPea(peaX, peaY, getDirection());
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

bool Marble::swallow(){
    setDead();
    return true;
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
    if(getWorld()->swallowObjectAt(getX(), getY()))
        setDead();
}

// MARK: PEA
Pea::Pea(int startX, int startY, int startDir, StudentWorld* wrld)
: Actor(IID_PEA, startX, startY, startDir, wrld){
}

Pea::~Pea() {
}

void Pea::doSomething(){
    if(!isAlive())
        return;
}