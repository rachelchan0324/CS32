#ifndef STUDENTWORLD_H_
#define STUDENTWORLD_H_

#include "GameWorld.h"
#include "Level.h"
#include <string>
#include <list>
#include "Actor.h"

using namespace std;

class StudentWorld : public GameWorld {
public:
    StudentWorld(std::string assetPath);
    ~StudentWorld();
    virtual int init();
    virtual int move();
    virtual void cleanUp();
    
    bool emptySpace(int x, int y);
    Actor* getPlayer() {return m_player;}
    Actor* actorAt(int x, int y);
    Actor* actorAtSamePlace(Actor*);
    void addPea(int x, int y, int dir);
    bool obstacleAt(int x, int y);
    
    // actor-specific functions
    bool doSomethingToActorsHitByPea(Actor*);
    bool obstaclesBetweenActorAndPlayer(Actor*);
private:
    list <Actor*> actors;
    Actor* m_player;
};

#endif // STUDENTWORLD_H_
