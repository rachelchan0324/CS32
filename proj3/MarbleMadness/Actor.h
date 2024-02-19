#ifndef ACTOR_H_
#define ACTOR_H_

#include "GraphObject.h"

class StudentWorld;

// Students:  Add code to this file, Actor.cpp, StudentWorld.h, and StudentWorld.cpp

class Actor : public GraphObject {
public:
    Actor(int ID, int startX, int startY, int startDir, StudentWorld* wrld);
    virtual ~Actor();
    StudentWorld* getWorld();
    virtual void doSomething() = 0;
private:
    StudentWorld* wrld;
};

class Avatar : public Actor {
public:
    Avatar(int startX, int startY, StudentWorld* wrld);
    ~Avatar();
    virtual void doSomething();
private:
    int peas;
    int hitPoints;
};

class Wall : public Actor {
public:
    Wall(int startX, int startY, StudentWorld* wrld);
    ~Wall();
    virtual void doSomething();
private:
};

#endif
