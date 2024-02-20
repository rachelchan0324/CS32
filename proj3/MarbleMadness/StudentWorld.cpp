#include "StudentWorld.h"
#include "GameConstants.h"
#include "Level.h"
#include <string>
using namespace std;

GameWorld* createStudentWorld(string assetPath) {
	return new StudentWorld(assetPath);
}

// Students:  Add code to this file, StudentWorld.h, Actor.h, and Actor.cpp

StudentWorld::StudentWorld(string assetPath)
: GameWorld(assetPath){
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
            if(item == Level::player)
                actors.push_back(new Avatar(c, r, this));
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
    // This code is here merely to allow the game to build, run, and terminate after you type q
    setGameStatText("Game will end when you type q");
    list<Actor*>::iterator it = actors.begin();
    while(it != actors.end()){
        (*it)->doSomething();
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

bool StudentWorld::isAndPushMarble(int x, int y, int dir){
    list<Actor*>::iterator it = actors.begin();
    while(it != actors.end()){
        if((*it)->getX() == x && (*it)->getY() == y){
            if((*it)->getID() == IID_MARBLE)
                return pushMarble(it, x, y, dir);
            return false;
        }
        it++;
    }
    return false;
}

bool StudentWorld::pushMarble(list<Actor*>::iterator it, int x, int y, int dir){
    if(dir == Actor::left)
        x--;
    else if(dir == Actor::right)
        x++;
    else if(dir == Actor::up)
        y++;
    else
        y--;
    if(!emptySpace(x, y) && !isPit(x, y))
        return false;
    (*it)->moveTo(x, y);
    return true;
}

bool StudentWorld::isPit(int x, int y){
    list<Actor*>::iterator it = actors.begin();
    while(it != actors.end()){
        if((*it)->getX() == x && (*it)->getY() == y)
            return (*it)->getID() == IID_PIT;
        it++;
    }
    return false;
}

void StudentWorld::cleanUp(){
    list<Actor*>::iterator it = actors.begin();
    while(it != actors.end()){
        delete *it;
        *it = nullptr;
        it++;
    }
}
