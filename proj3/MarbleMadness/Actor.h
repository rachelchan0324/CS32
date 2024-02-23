#ifndef ACTOR_H_
#define ACTOR_H_

#include "GraphObject.h"

class StudentWorld;

class Actor : public GraphObject {
public:
    Actor(int ID, int startX, int startY, int startDir, StudentWorld* wrld);
    virtual ~Actor();
    
    StudentWorld* getWorld() {return wrld;}
    virtual void doSomething() = 0;
    virtual bool push(int dir) {return false;}
    virtual bool isAlive() const {return alive;}
    virtual void setDead() {alive = false;}
    
    // identifier methods
    virtual bool swallow() {return false;}
    virtual bool canBePushedOn() const {return false;}
    virtual bool canBePushed() const {return false;}
    virtual bool canBeAttacked() const {return false;}
    
    virtual void getNewCoordinates(int& x, int& y, int dir);
private:
    StudentWorld* wrld;
    bool alive;
};

class Avatar : public Actor {
public:
    Avatar(int startX, int startY, StudentWorld* wrld);
    ~Avatar();
    virtual void doSomething();
    virtual bool canBeAttacked() const {return true;}
private:
    int peas;
    int hitPoints;
    void shootPea();
};

class Wall : public Actor {
public:
    Wall(int startX, int startY, StudentWorld* wrld);
    ~Wall();
    virtual void doSomething();
private:
};

class Marble : public Actor {
public:
    Marble(int startX, int startY, StudentWorld* wrld);
    ~Marble();
    virtual void doSomething();
    virtual bool canBePushed() const;
    virtual bool push(int dir);
    virtual bool canBeAttacked() const {return true;}
    virtual bool swallow();
private:
    int hitPoints;
};

class Pit : public Actor{
public:
    Pit(int startX, int startY, StudentWorld* wrld);
    ~Pit();
    virtual void doSomething();
    virtual bool canBePushedOn() const;
private:
};

class Pea : public Actor{
public:
    Pea(int startX, int startY, int startDir, StudentWorld* wrld);
    ~Pea();
    virtual void doSomething();
private:
};

#endif
