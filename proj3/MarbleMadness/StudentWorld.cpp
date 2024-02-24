#include "StudentWorld.h"
#include "GameConstants.h"
#include "Level.h"
#include <string>
using namespace std;

GameWorld* createStudentWorld(string assetPath) {
    return new StudentWorld(assetPath);
}

StudentWorld::StudentWorld(string assetPath)
: GameWorld(assetPath), m_player(nullptr){
}

StudentWorld::~StudentWorld(){
    cleanUp();
}

int StudentWorld::init() {
    string levelStr = to_string(getLevel());
    if(getLevel() < 10)
        levelStr += "0";
    string levelFilename = "level" + levelStr + ".txt";
    Level lev(assetPath());
    
    Level::LoadResult result = lev.loadLevel(levelFilename);
    if (result == Level::load_fail_file_not_found || result == Level:: load_fail_bad_format)
        return GWSTATUS_LEVEL_ERROR;
    
    for(int r = 0; r < VIEW_HEIGHT; r++){
        for(int c = 0; c < VIEW_WIDTH; c++){
            Level::MazeEntry item = lev.getContentsOf(c, r);
            if(item == Level::player){
                m_player = new Avatar(c, r, this);
                actors.push_back(m_player);
            }
            else if(item == Level::wall)
                actors.push_back(new Wall(c, r, this));
            else if(item == Level::marble)
                actors.push_back(new Marble(c, r, this));
            else if(item == Level::pit)
                actors.push_back(new Pit(c, r, this));
            else if(item == Level::horiz_ragebot)
                actors.push_back(new RageBot(c, r, Actor::right, this));
            else if(item == Level::vert_ragebot)
                actors.push_back(new RageBot(c, r, Actor::down, this));
        }
    }
    return GWSTATUS_CONTINUE_GAME;
}

int StudentWorld::move() {
    setGameStatText("Game will end when you type q");
    list<Actor*>::iterator it = actors.begin();
    while(it != actors.end()){
        (*it)->doSomething();
        it++;
    }
    it = actors.begin();
    while(it != actors.end()){
        if(!(*it)->isAlive()){
            delete *it; // remove object
            *it = nullptr; // not necessary
            it = actors.erase(it); // remove from the list
        }
        else
            it++;
    }
    return GWSTATUS_CONTINUE_GAME;
}

bool StudentWorld::emptySpace(int x, int y){
    list<Actor*>::iterator it = actors.begin();
    while(it != actors.end()){
        if((*it)->getX() == x && (*it)->getY() == y)
            return false;
        it++;
    }
    return true;
}

Actor* StudentWorld::actorAt(int x, int y){
    list<Actor*>::iterator it = actors.begin();
    while(it != actors.end()){
        if((*it)->getX() == x && (*it)->getY() == y)
            return *it;
        it++;
    }
    return nullptr;
}

Actor* StudentWorld::actorAtSamePlace(Actor* ptr){
    list<Actor*>::iterator it = actors.begin();
    while(it != actors.end()){
        if((*it)->getX() == ptr->getX() && (*it)->getY() == ptr->getY() && (*it) != ptr)
            return *it;
        it++;
    }
    return nullptr;
}

// returns a boolean telling whether or not the pea dies
bool StudentWorld::doSomethingToActorsHitByPea(Actor* ptr){
    list<Actor*>::iterator it = actors.begin();
    while(it != actors.end()){
        // damage all actors that CAN be damaged
        if((*it)->getX() == ptr->getX() && (*it)->getY() == ptr->getY() && (*it) != ptr && (*it)->getDamaged()){
            ptr->setDead();
            return true;
        }
        // if it hits a wall
        if((*it)->getX() == ptr->getX() && (*it)->getY() == ptr->getY() && (*it)->blocksMovement()){
            ptr->setDead();
            return true;
        }
        it++;
    }
    return false;
}

bool StudentWorld::obstaclesBetweenActorAndPlayer(Actor* ptr){
    // find range between actor and player
    int startX, endX, startY, endY;
    if(ptr->getDirection() == Actor::right){
        startY = endY = ptr->getY();
        startX = ptr->getX() + 1;
        endX = getPlayer()->getX() - 1;
    }
    else if(ptr->getDirection() == Actor::left){
        startY = endY = ptr->getY();
        startX = getPlayer()->getX() + 1;
        endX = ptr->getX() - 1;
    }
    else if(ptr->getDirection() == Actor::up){
        startX = endX = ptr->getX();
        startY = ptr->getY() + 1;
        endY = getPlayer()->getY() - 1;
    }
    else{
        startX = endX = ptr->getX();
        startY = getPlayer()->getY() + 1;
        endY = ptr->getY() - 1;
    }
    
    // check to see if any actors in that range block movement
    list<Actor*>::iterator it = actors.begin();
    while(it != actors.end()){
        if((*it)->getX() >= startX && (*it)->getX() <= endX && (*it)->getY() >= startY && (*it)->getY() <= endY && (*it)->blocksMovement())
            return true;
        it++;
    }
    return false;
}

bool StudentWorld::obstacleAt(int x, int y){
    list<Actor*>::iterator it = actors.begin();
    while(it != actors.end()){
        if((*it)->getX() == x && (*it)->getY() == y && (*it)->blocksMovement())
            return true;
        it++;
    }
    return false;
}

void StudentWorld::addPea(int x, int y, int dir){
    actors.push_back(new Pea(x, y, dir, this));
}

void StudentWorld::cleanUp(){
    list<Actor*>::iterator it = actors.begin();
    while(it != actors.end()){
        delete *it;
        *it = nullptr;
        it++;
    }
}
