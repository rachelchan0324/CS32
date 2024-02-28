#ifndef ACTOR_H_
#define ACTOR_H_

#include "GraphObject.h"

class StudentWorld;
class Agent;

class Actor : public GraphObject {
public:
    Actor(StudentWorld* world, int startX, int startY, int imageID, int hitPoints, int startDir);
    
    virtual void doSomething() = 0;
    bool isAlive() const {return alive;};
    void setDead() {alive = false;}
    
    void decHitPoints(int amt) {hitPoints -= amt;}
    StudentWorld* getWorld() const {return world;}
    
    virtual bool allowsAgentColocation() const {return false;}
    virtual bool allowsMarbleColocation() const {return false;}
    // Does this actor count when a factory counts items near it?
    // virtual bool countsInFactoryCensus() const;
    virtual bool stopsPea() const {return false;}
    virtual bool isDamageable() const {return false;}
    virtual void damage(int damageAmt) {};
    virtual bool bePushedBy(Agent* a, int x, int y) {return false;}
    virtual bool isSwallowable() const {return false;}
    // Can this actor be picked up by a ThiefBot?
    // virtual bool isStealable() const;
    virtual int getHitPoints() const {return hitPoints;}
    // virtual void setHitPoints(int amt) {hitPoints = amt;}
    
    // Make the actor sustain damage.  Return true if this kills the actor, and false otherwise.
    
    // TODO: function i created to get dx and dy
    void getDxDy(int& dx, int &dy);
private:
    bool alive;
    int hitPoints;
    StudentWorld* world;
};

class Agent : public Actor {
public:
    Agent(StudentWorld* world, int startX, int startY, int imageID, int hitPoints, int startDir);
    
    bool moveIfPossible();
    virtual bool canPushMarbles() const {return false;}
    // Return true if this agent doesn't shoot unless there's an unobstructed path to the player.
    // virtual bool needsClearShot() const {return true;}
};

class Player : public Agent {
public:
    Player(StudentWorld* world, int startX, int startY);
    virtual void doSomething();
    virtual bool isDamageable() const {return true;}
    virtual void damage(int damageAmt);
    virtual bool canPushMarbles() const {return true;}
    // virtual bool needsClearShot() const {return false;}
    
    // int getHealthPct() const {return getHitPoints() * 5;}
    int getAmmo() const {return ammo;}
    // void restoreHealth() {setHitPoints(20);}
    void increaseAmmo() {ammo++;}
private:
    int ammo;
};

class Robot : public Agent {
public:
    Robot(StudentWorld* world, int startX, int startY, int imageID, int hitPoints, int score, int startDir);
    // virtual void doSomething() const {};
    virtual bool isDamageable() const {return true;}
    virtual void damage(int damageAmt);
    // virtual bool canPushMarbles() const {return false;}
    virtual bool needsClearShot() const {return true;}
    virtual bool isShootingRobot() const {return true;}
    
    // TODO: my own functions i created to manage ticking
    bool rest();
private:
    int ticks;
    int currTick;
};

class RageBot : public Robot {
public:
    RageBot(StudentWorld* world, int startX, int startY, int startDir);
    virtual void doSomething();
};

//class ThiefBot : public Robot {
//public:
//    ThiefBot(StudentWorld* world, int startX, int startY, int imageID,
//             int hitPoints, int score);
//    virtual void doSomething();
//    virtual bool countsInFactoryCensus() const;
//    virtual void damage(int damageAmt);
//};
//
//class RegularThiefBot : public ThiefBot {
//public:
//    RegularThiefBot(StudentWorld* world, int startX, int startY);
//    virtual void doSomething();
//    virtual bool isShootingRobot() const {return false;}
//};
//
//class MeanThiefBot : public ThiefBot {
//public:
//    MeanThiefBot(StudentWorld* world, int startX, int startY);
//    virtual void doSomething();
//};
//
//class Exit : public Actor {
//public:
//    Exit(StudentWorld* world, int startX, int startY);
//    virtual void doSomething();
//    virtual bool allowsAgentColocation() const;
//};
//
class Wall : public Actor {
public:
    Wall(StudentWorld* world, int startX, int startY);
    virtual void doSomething() {};
    virtual bool stopsPea() const {return true;}
};

class Marble : public Actor {
public:
    Marble(StudentWorld* world, int startX, int startY);
    virtual void doSomething() {};
    virtual bool isDamageable() const {return true;}
    virtual void damage(int damageAmt);
    virtual bool isSwallowable() const {return true;}
    virtual bool bePushedBy(Agent* a, int x, int y);
};

class Pit : public Actor {
public:
    Pit(StudentWorld* world, int startX, int startY);
    virtual void doSomething();
    virtual bool allowsMarbleColocation() const {return true;}
};

class Pea : public Actor {
public:
    Pea(StudentWorld* world, int startX, int startY, int startDir);
    virtual void doSomething();
    virtual bool allowsAgentColocation() const {return true;}
private:
    bool moved;
};

//class ThiefBotFactory : public Actor {
//public:
//    enum ProductType { REGULAR, MEAN };
//    
//    ThiefBotFactory(StudentWorld* world, int startX, int startY, ProductType type);
//    virtual void doSomething();
//    virtual bool stopsPea() const;
//};
//
//class PickupableItem : public Actor {
//public:
//    PickupableItem(StudentWorld* world, int startX, int startY, int imageID, int score);
//    virtual void doSomething();
//    virtual bool allowsAgentColocation() const;
//};
//
//class Crystal : public PickupableItem {
//public:
//    Crystal(StudentWorld* world, int startX, int startY);
//};
//
//class Goodie : public PickupableItem {
//public:
//    Goodie(StudentWorld* world, int startX, int startY, int imageID, int score);
//    virtual void doSomething();
//    virtual bool isStealable() const;
//    
//    // Set whether this goodie is currently stolen.
//    void setStolen(bool status);
//};
//
//class ExtraLifeGoodie : public Goodie {
//public:
//    ExtraLifeGoodie(StudentWorld* world, int startX, int startY);
//};
//
//class RestoreHealthGoodie : public Goodie {
//public:
//    RestoreHealthGoodie(StudentWorld* world, int startX, int startY);
//};
//
//class AmmoGoodie : public Goodie {
//public:
//    AmmoGoodie(StudentWorld* world, int startX, int startY);
//};

#endif // ACTOR_H_
