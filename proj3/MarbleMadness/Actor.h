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
    void getDxDy(int& dx, int &dy);
    
    void decHitPoints(int amt) {hitPoints -= amt;}
    StudentWorld* getWorld() const {return world;}
    
    virtual bool allowsAgentColocation() const {return false;}
    virtual bool allowsMarbleColocation() const {return false;}
    virtual bool countsInFactoryCensus() const {return false;}
    virtual bool stopsPea() const {return false;}
    virtual bool isDamageable() const {return false;}
    virtual void damage(int damageAmt) {};
    virtual bool bePushedBy(Agent* a, int x, int y) {return false;}
    virtual bool isSwallowable() const {return false;}
    virtual bool isStealable() const {return false;}
    virtual int getHitPoints() const {return hitPoints;}
    virtual void setHitPoints(int amt) {hitPoints = amt;}
    virtual void setStolen(bool b) {};
    virtual bool isStolen() {return true;}
    
    int randInt(int min, int max);
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
};

class Player : public Agent {
public:
    Player(StudentWorld* world, int startX, int startY);
    virtual void doSomething();
    virtual bool isDamageable() const {return true;}
    virtual void damage(int damageAmt);
    virtual bool canPushMarbles() const {return true;}
    
    int getHealthPct() const {return getHitPoints() * 5;}
    int getAmmo() const {return ammo;}
    void restoreHealth() {setHitPoints(20);}
    void increaseAmmo() {ammo++;}
private:
    int ammo;
};


class Robot : public Agent {
public:
    Robot(StudentWorld* world, int startX, int startY, int imageID, int hitPoints, int score, int startDir);
    virtual bool isDamageable() const {return true;}
    virtual void damage(int damageAmt);
    bool rest();
private:
    int ticks;
    int currTick;
    int score;
};

class RageBot : public Robot {
public:
    RageBot(StudentWorld* world, int startX, int startY, int startDir);
    virtual void doSomething();
};

class ThiefBot : public Robot {
public:
    ThiefBot(StudentWorld* world, int startX, int startY, int imageID, int hitPoints, int score);
    virtual void doSomething();
    virtual bool countsInFactoryCensus() const {return true;}
    virtual void damage(int damageAmt);
private:
    bool hasGoodie;
    int distanceBeforeTurning;
    int stepsInCurrentDirection;
    Actor* goodie;
    void getDxDyFromRandInt(int dir, int& dx, int& dy);
};

class RegularThiefBot : public ThiefBot {
public:
    RegularThiefBot(StudentWorld* world, int startX, int startY);
    virtual void doSomething();
    virtual bool isShootingRobot() const {return false;}
};

class MeanThiefBot : public ThiefBot {
public:
    MeanThiefBot(StudentWorld* world, int startX, int startY);
    virtual void doSomething();
};

class Exit : public Actor {
public:
    Exit(StudentWorld* world, int startX, int startY);
    virtual void doSomething();
    virtual bool allowsAgentColocation() const {return true;}
private:
    bool revealed;
};

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

class ThiefBotFactory : public Actor {
public:
    enum ProductType { REGULAR, MEAN };
    
    ThiefBotFactory(StudentWorld* world, int startX, int startY, ProductType type);
    virtual void doSomething();
    virtual bool stopsPea() const {return true;}
private:
    ProductType type;
};

class PickupableItem : public Actor {
public:
    PickupableItem(StudentWorld* world, int startX, int startY, int imageID, int score);
    virtual void doSomething();
    virtual bool allowsAgentColocation() const {return true;};
private:
    int score;
    virtual void doSpecificPickableItemStuff() = 0;
};

class Crystal : public PickupableItem {
public:
    Crystal(StudentWorld* world, int startX, int startY);
private:
    virtual void doSpecificPickableItemStuff();
};

class Goodie : public PickupableItem {
public:
    Goodie(StudentWorld* world, int startX, int startY, int imageID, int score);
    virtual void doSomething();
    virtual bool isStealable() const {return true;}
    virtual void setStolen(bool status) {stolen = status;}
    virtual bool isStolen() {return stolen;}
private:
    virtual void doGoodieSpecificStuff() = 0;
    virtual void doSpecificPickableItemStuff() {doGoodieSpecificStuff();}
    bool stolen;
};

class ExtraLifeGoodie : public Goodie {
public:
    ExtraLifeGoodie(StudentWorld* world, int startX, int startY);
private:
    virtual void doGoodieSpecificStuff();
};

class RestoreHealthGoodie : public Goodie {
public:
    RestoreHealthGoodie(StudentWorld* world, int startX, int startY);
private:
    virtual void doGoodieSpecificStuff();
};

class AmmoGoodie : public Goodie {
public:
    AmmoGoodie(StudentWorld* world, int startX, int startY);
private:
    virtual void doGoodieSpecificStuff();
};

#endif // ACTOR_H_
