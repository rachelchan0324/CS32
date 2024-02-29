#include "Actor.h"
#include "StudentWorld.h"
#include "GameConstants.h"
#include "GraphObject.h"

//MARK: Actor
Actor::Actor(StudentWorld* world, int startX, int startY, int imageID, int hitPoints, int startDir)
: GraphObject(imageID, startX, startY, startDir), alive(true), hitPoints(hitPoints), world(world) {
    setVisible(true);
}

void Actor::getDxDy(int& dx, int& dy){
    dx = 0;
    dy = 0;
    if(getDirection() == right)
        dx = 1;
    else if(getDirection() == left)
        dx = -1;
    else if(getDirection() == up)
        dy = 1;
    else
        dy = -1;
}

// MARK: Agent
Agent::Agent(StudentWorld* world, int startX, int startY, int imageID, int hitPoints, int startDir)
: Actor(world, startX, startY, imageID, hitPoints, startDir){
}

bool Agent::moveIfPossible(){
    int dx, dy;
    getDxDy(dx, dy);
    if(getWorld()->canAgentMoveTo(this, getX() + dx, getY() + dy, dx, dy)){
        moveTo(getX() + dx, getY() + dy);
        return true;
    }
    return false;
}

// MARK: Player
Player::Player(StudentWorld* world, int startX, int startY)
: Agent(world, startX, startY, IID_PLAYER, 20, right), ammo(20){
}

void Player::doSomething(){
    int ch;
    if (getWorld()->getKey(ch)) {
        switch(ch) {
            case KEY_PRESS_SPACE:
                if(ammo > 0){
                    ammo--;
                    int dx, dy;
                    getDxDy(dx, dy);
                    getWorld()->addActor(new Pea(getWorld(), getX() + dx, getY() + dy, getDirection()));
                    getWorld()->playSound(SOUND_PLAYER_FIRE);
                }
                break;
            case KEY_PRESS_RIGHT:
                setDirection(right);
                moveIfPossible();
                break;
            case KEY_PRESS_LEFT:
                setDirection(left);
                moveIfPossible();
                break;
            case KEY_PRESS_UP:
                setDirection(up);
                moveIfPossible();
                break;
            case KEY_PRESS_DOWN:
                setDirection(down);
                moveIfPossible();
                break;
        }
    }
}

void Player::damage(int damageAmt){
    decHitPoints(damageAmt);
    if(getHitPoints() <= 0){
        setDead();
        getWorld()->decLives();
        getWorld()->playSound(SOUND_PLAYER_DIE);
    }
}

// MARK: Wall
Wall::Wall(StudentWorld* world, int startX, int startY)
: Actor(world, startX, startY, IID_WALL, 0, none){
}

// MARK: Marble
Marble::Marble(StudentWorld* world, int startX, int startY)
: Actor(world, startX, startY, IID_MARBLE, 10, none){
}

bool Marble::bePushedBy(Agent* a, int x, int y){
    if(a->canPushMarbles() && getWorld()->canMarbleMoveTo(x, y)){
        moveTo(x, y);
        return true;
    }
    return false;
}

void Marble::damage(int damageAmt){
    decHitPoints(damageAmt);
    if(getHitPoints() <= 0)
        setDead();
}

// MARK: Pit
Pit::Pit(StudentWorld* world, int startX, int startY)
: Actor(world, startX, startY, IID_PIT, 0, none){
}

void Pit::doSomething(){
    if(!isAlive())
        return;
    if(getWorld()->swallowSwallowable(this))
        setDead();
}

// MARK: Pea
Pea::Pea(StudentWorld* world, int startX, int startY, int startDir)
: Actor(world, startX, startY, IID_PEA, 0, startDir){
}

void Pea::doSomething(){
    if(moved){
        if(!isAlive())
            return;
        if(getWorld()->damageSomething(this, 2)){
            setDead();
            return;
        }
        
        // move 1 in the specified direction
        int dx, dy;
        getDxDy(dx, dy);
        moveTo(getX() + dx, getY() + dy);
        
        // check new square
        if(getWorld()->damageSomething(this, 2))
            setDead();
    }
    else
        moved = true;
}

// MARK: Robot
Robot::Robot(StudentWorld* world, int startX, int startY, int imageID, int hitPoints, int score, int startDir)
: Agent(world, startX, startY, imageID, hitPoints, startDir), currTick(1), score(score){
    ticks = (28 - getWorld()->getLevel()) / 4;
    if (ticks < 3)
        ticks = 3;
}

void Robot::damage(int damageAmt){
    decHitPoints(damageAmt);
    if(getHitPoints() <= 0){
        setDead();
        getWorld()->increaseScore(score);
        getWorld()->playSound(SOUND_ROBOT_DIE);
    }
    else
        getWorld()->playSound(SOUND_ROBOT_IMPACT);
}

bool Robot::rest(){
    if(currTick == ticks){
        currTick = 1; // reset ticks
        return false;
    }
    else{
        currTick++;
        return true;
    }
}

// MARK: RageBot
RageBot::RageBot(StudentWorld* world, int startX, int startY, int startDir)
// TODO: what to do with robot score?
: Robot(world, startX, startY, IID_RAGEBOT, 10, 100, startDir) {
}

void RageBot::doSomething(){
    if(!isAlive())
        return;
    if(!rest()){
        int dx, dy;
        getDxDy(dx, dy);
        if(getWorld()->existsClearShotToPlayer(getX() + dx, getY() + dy, dx, dy)){ // check if pea has clear shot to player
            getWorld()->addActor(new Pea(getWorld(), getX() + dx, getY() + dy, getDirection()));
            getWorld()->playSound(SOUND_ENEMY_FIRE);
        }
        else {
            if(getWorld()->canAgentMoveTo(this, getX() + dx, getY() + dy, dx, dy))
                moveTo(getX() + dx, getY() + dy);
            else{
                if(getDirection() == right)
                    setDirection(left);
                else if(getDirection() == left)
                    setDirection(right);
                else if(getDirection() == up)
                    setDirection(down);
                else
                    setDirection(up);
            }
        }
    }
}

// MARK: Pickupable Item
PickupableItem::PickupableItem(StudentWorld* world, int startX, int startY, int imageID, int score)
: Actor(world, startX, startY, imageID, 0, none), score(score){
}

void PickupableItem::doSomething(){
    if(!isAlive())
        return;
    if(getWorld()->isPlayerColocatedWith(getX(), getY())){
        doSpecificPickableItemStuff();
        getWorld()->increaseScore(score);
        setDead();
        getWorld()->playSound(SOUND_GOT_GOODIE);
    }
}

// MARK: Crystal
Crystal::Crystal(StudentWorld* world, int startX, int startY)
: PickupableItem(world, startX, startY, IID_CRYSTAL, 50){
}

void Crystal::doSpecificPickableItemStuff(){
    getWorld()->decCrystals();
}

// MARK: Exit
Exit::Exit(StudentWorld* world, int startX, int startY)
: Actor(world, startX, startY, IID_EXIT, 0, none), revealed(false){
    setVisible(false);
}

void Exit::doSomething(){
    if(!revealed && !getWorld()->anyCrystals()){
        setVisible(true);
        getWorld()->playSound(SOUND_REVEAL_EXIT);
        revealed = true;
    }
    if(revealed && getWorld()->isPlayerColocatedWith(getX(), getY())){
        getWorld()->playSound(SOUND_FINISHED_LEVEL);
        getWorld()->increaseScore(2000);
        getWorld()->setLevelFinished();
    }
}

// MARK: Goodie
Goodie::Goodie(StudentWorld* world, int startX, int startY, int imageID, int score)
: PickupableItem(world, startX, startY, imageID, score){
}

void Goodie::doSomething(){
    PickupableItem::doSomething();
    if(getWorld()->isPlayerColocatedWith(getX(), getY()))
       doGoodieSpecificStuff();
}

// MARK: Ammo Goodie
AmmoGoodie::AmmoGoodie(StudentWorld* world, int startX, int startY)
: Goodie(world, startX, startY, IID_AMMO, 100){
}

void AmmoGoodie::doGoodieSpecificStuff(){
    for(int i = 0; i < 20; i++)
        getWorld()->increaseAmmo();
}
