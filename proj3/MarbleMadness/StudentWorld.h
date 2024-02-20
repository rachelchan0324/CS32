#ifndef STUDENTWORLD_H_
#define STUDENTWORLD_H_

#include "GameWorld.h"
#include "Level.h"
#include <string>
#include <list>
#include "Actor.h"

using namespace std;

// Students:  Add code to this file, StudentWorld.cpp, Actor.h, and Actor.cpp

class StudentWorld : public GameWorld {
public:
    StudentWorld(std::string assetPath);
    ~StudentWorld();
    virtual int init();
    virtual int move();
    virtual void cleanUp();
    bool emptySpace(int x, int y);
    bool isAndPushMarble(int x, int y, int dir);
    bool pushMarble(list<Actor*>::iterator it, int x, int y, int dir);
    bool isPit(int x, int y);
private:
    list <Actor*> actors;
};

#endif // STUDENTWORLD_H_
