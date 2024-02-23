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
