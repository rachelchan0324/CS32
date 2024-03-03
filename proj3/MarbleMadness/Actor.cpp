#include "Actor.h"
#include "StudentWorld.h"
#include "GameConstants.h"
#include "GraphObject.h"

#include <random>

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

// Return a uniformly distributed random int from min to max, inclusive
int Actor::randInt(int min, int max) {
    if (max < min)
        swap(max, min);
    static random_device rdv;
    static default_random_engine generator(rdv());
    uniform_int_distribution<> distro(min, max);
    return distro(generator);
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
            case KEY_PRESS_ESCAPE:
                setDead();
                getWorld()->decLives();
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
    else
        getWorld()->playSound(SOUND_PLAYER_IMPACT);
}

// MARK: Wall
Wall::Wall(StudentWorld* world, int startX, int startY)
: Actor(world, startX, startY, IID_WALL, -1, none){
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
: Actor(world, startX, startY, IID_PIT, -1, none){
}

void Pit::doSomething(){
    if(!isAlive())
        return;
    if(getWorld()->swallowSwallowable(this))
        setDead();
}

// MARK: Pea
Pea::Pea(StudentWorld* world, int startX, int startY, int startDir)
: Actor(world, startX, startY, IID_PEA, -1, startDir){
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
: Robot(world, startX, startY, IID_RAGEBOT, 10, 100, startDir) {
}

void RageBot::doSomething(){
    if(!isAlive())
        return;
    if(!rest()){
        // attempt to shoot at player
        int dx, dy;
        getDxDy(dx, dy);
        if(getWorld()->existsClearShotToPlayer(getX() + dx, getY() + dy, dx, dy)){ // check if pea has clear shot to player
            getWorld()->addActor(new Pea(getWorld(), getX() + dx, getY() + dy, getDirection()));
            getWorld()->playSound(SOUND_ENEMY_FIRE);
            return;
        }
        if(getWorld()->canAgentMoveTo(this, getX() + dx, getY() + dy, dx, dy)){
            moveTo(getX() + dx, getY() + dy);
            return;
        }
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

// MARK: Thief Bot
ThiefBot::ThiefBot(StudentWorld* world, int startX, int startY, int imageID, int hitPoints, int score)
: Robot(world, startX, startY, imageID, hitPoints, score, right), hasGoodie(false), distanceBeforeTurning(randInt(1, 6)), stepsInCurrentDirection(0), goodie(nullptr){
}

void ThiefBot::doSomething(){
    if(!hasGoodie && getWorld()->getColocatedStealable(getX(), getY()) != nullptr && !getWorld()->getColocatedStealable(getX(), getY())->isStolen() && randInt(1, 10) == 1){
        goodie = getWorld()->getColocatedStealable(getX(), getY());
        goodie->setStolen(true);
        hasGoodie = true;
        getWorld()->playSound(SOUND_ROBOT_MUNCH);
        return;
    }
    if(stepsInCurrentDirection < distanceBeforeTurning){
        int dx, dy;
        getDxDy(dx, dy);
        if(getWorld()->canAgentMoveTo(this, getX() + dx, getY() + dy, dx, dy))
            moveTo(getX() + dx, getY() + dy);
        stepsInCurrentDirection++;
        return;
    }
    stepsInCurrentDirection = 0;
    distanceBeforeTurning = randInt(1, 6);
    // at this point, the current path has an obstruction or the thief bot has reached max steps in current direction, try to move in a NEW direction

    // first direction
    int d = randInt(1, 4);
    
    int currD = d;
    for(int i = 0; i < 4; i++){
        int dx, dy;
        getDxDyFromRandInt(currD, dx, dy);
        if(getWorld()->canAgentMoveTo(this, getX() + dx, getY() + dy, dx, dy)){
            moveTo(getX() + dx, getY() + dy);
            d = currD;
            stepsInCurrentDirection++;
            break;
        }
        // switch directions
        currD++;
        if(currD > 4)
            currD = 1;
    }
    if(d == 1)
        setDirection(right);
    else if(d == 2)
        setDirection(left);
    else if(d == 3)
        setDirection(up);
    else
        setDirection(down);
}

void ThiefBot::damage(int damageAmt){
    Robot::damage(damageAmt);
    if(getHitPoints() <= 0 && hasGoodie){
        goodie->setStolen(false);
        goodie->moveTo(getX(), getY());
        hasGoodie = false;
        goodie = nullptr;
    }
}

void ThiefBot::getDxDyFromRandInt(int dir, int& dx, int& dy){
    dx = dy = 0;
    if(dir == 1)
        dx = 1;
    else if(dir == 2)
        dx = -1;
    else if(dir == 3)
        dy = 1;
    else
        dy = -1;
}

// MARK: Regular Thief Bot

RegularThiefBot::RegularThiefBot(StudentWorld* world, int startX, int startY)
: ThiefBot(world, startX, startY, IID_THIEFBOT, 5, 10){
}

void RegularThiefBot::doSomething(){
    if(!isAlive())
        return;
    if(!rest()){
        ThiefBot::doSomething();
    }
}

// MARK: Mean Thief Bot

MeanThiefBot::MeanThiefBot(StudentWorld* world, int startX, int startY)
: ThiefBot(world, startX, startY, IID_MEAN_THIEFBOT, 8, 20){
}

void MeanThiefBot::doSomething(){
    if(!isAlive())
        return;
    if(!rest()){
        // sees if it can shoot first
        int dx, dy;
        getDxDy(dx, dy);
        if(getWorld()->existsClearShotToPlayer(getX() + dx, getY() + dy, dx, dy)){ // check if pea has clear shot to player
            getWorld()->addActor(new Pea(getWorld(), getX() + dx, getY() + dy, getDirection()));
            getWorld()->playSound(SOUND_ENEMY_FIRE);
            return;
        }
        // if not, act as a normal thief bot
        ThiefBot::doSomething();
    }
}


// MARK: Thief Bot Factory
ThiefBotFactory::ThiefBotFactory(StudentWorld* world, int startX, int startY, ProductType type)
: Actor(world, startX, startY, IID_ROBOT_FACTORY, -1, none), type(type){
}

void ThiefBotFactory::doSomething(){
    int count;
    if(getWorld()->doFactoryCensus(getX(), getY(), 3, count) && count < 3 && randInt(1, 50) == 1){
        if(type == REGULAR)
            getWorld()->addActor(new RegularThiefBot(getWorld(), getX(), getY()));
        else
            getWorld()->addActor(new MeanThiefBot(getWorld(), getX(), getY()));
        getWorld()->playSound(SOUND_ROBOT_BORN);
    }
}

// MARK: Pickupable Item
PickupableItem::PickupableItem(StudentWorld* world, int startX, int startY, int imageID, int score)
: Actor(world, startX, startY, imageID, -1, none), score(score){
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
: Actor(world, startX, startY, IID_EXIT, -1, none), revealed(false){
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
: PickupableItem(world, startX, startY, imageID, score), stolen(false){
}

void Goodie::doSomething(){
    if(stolen){
        setVisible(false);
        return;
    }
    setVisible(true);
    PickupableItem::doSomething();
}

// MARK: Extra Life Goodie
ExtraLifeGoodie::ExtraLifeGoodie(StudentWorld* world, int startX, int startY)
: Goodie(world, startX, startY, IID_EXTRA_LIFE, 1000){
}

void ExtraLifeGoodie::doGoodieSpecificStuff(){
    getWorld()->incLives();
}

// MARK: Restore Health Goodie
RestoreHealthGoodie::RestoreHealthGoodie(StudentWorld* world, int startX, int startY)
: Goodie(world, startX, startY, IID_RESTORE_HEALTH, 500){
}

void RestoreHealthGoodie::doGoodieSpecificStuff(){
    getWorld()->restorePlayerHealth();
}

// MARK: Ammo Goodie
AmmoGoodie::AmmoGoodie(StudentWorld* world, int startX, int startY)
: Goodie(world, startX, startY, IID_AMMO, 100){
}

void AmmoGoodie::doGoodieSpecificStuff(){
    for(int i = 0; i < 20; i++)
        getWorld()->increaseAmmo();
}
