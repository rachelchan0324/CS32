#ifndef STUDENTWORLD_H_
#define STUDENTWORLD_H_

#include "GameWorld.h"
#include "Level.h"
#include "Actor.h"

#include <list>
#include <string>

using namespace std;

class Actor;
class Agent;
class Player;

// Note:  A convention used in a number of interfaces is to represent a
// direction with the adjustments to x and y needed to move one step in
// that direction:
// left:   dx is -1, dy is  0
// right:  dx is  1, dy is  0
// up:     dx is  0, dy is  1
// down:   dx is  0, dy is -1

class StudentWorld : public GameWorld {
public:
    StudentWorld(std::string assetDir);
    virtual ~StudentWorld();
    
    virtual int init();
    virtual int move();
    virtual void cleanUp();
    
    bool canAgentMoveTo(Agent* agent, int x, int y, int dx, int dy) const;
    bool canMarbleMoveTo(int x, int y) const;
    bool isPlayerColocatedWith(int x, int y) const;
    
    bool damageSomething(Actor* a, int damageAmt);
    bool swallowSwallowable(Actor* a);
    
    bool existsClearShotToPlayer(int x, int y, int dx, int dy) const;
    
    // If a factory is at x,y, how many items of the type that should be
    // counted are in the rectangle bounded by x-distance,y-distance and
    // x+distance,y+distance?  Set count to that number and return true,
    // unless an item is on the factory itself, in which case return false
    // and don't care about count.  (The items counted are only ever going
    // ThiefBots.)
    // bool doFactoryCensus(int x, int y, int distance, int& count) const;
    
    // If an item that can be stolen is at x,y, return a pointer to it;
    // otherwise, return a null pointer.  (Stealable items are only ever
    // going be goodies.)
    // Actor* getColocatedStealable(int x, int y) const;
    
    // Restore player's health to the full amount.
    // void restorePlayerHealth();
    
    void increaseAmmo() {m_player->increaseAmmo();}
    bool anyCrystals() const {return crystals > 0;}
    // TODO: made this non-const and void
    void decCrystals() {crystals--;}
    
    void setLevelFinished() {levelFinished = true;}
    void addActor(Actor* a);
    
private:
    list <Actor*> actors;
    Player* m_player;
    
    int bonus;
    int crystals;
    bool levelFinished;
};

#endif // STUDENTWORLD_H_
