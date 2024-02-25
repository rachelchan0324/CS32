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
    
    string getStatusLine() const;
    int getBonus() const {return bonus;}
    
    bool emptySpace(int x, int y) const;
    Actor* getPlayer() const {return m_player;}
    Actor* actorAt (int x, int y) const;
    Actor* actorAtSamePlace (Actor*) const;
    void addPea(int x, int y, int dir);
    bool obstacleAt (int x, int y) const;
    
    // actor-specific functions
    bool doSomethingToActorsHitByPea(Actor*);
    bool obstaclesBetweenActorAndPlayer(Actor*) const;
private:
    list <Actor*> actors;
    Avatar* m_player;
    int bonus;
};

#endif // STUDENTWORLD_H_
