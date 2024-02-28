#include "StudentWorld.h"
#include "GameConstants.h"
#include "Level.h"
#include <string>
#include <iostream> // defines the overloads of the << operator
#include <sstream>  // defines the type std::ostringstream

using namespace std;

GameWorld* createStudentWorld(string assetPath) {
    return new StudentWorld(assetPath);
}

StudentWorld::StudentWorld(string assetPath)
: GameWorld(assetPath), m_player(nullptr) {
}

StudentWorld::~StudentWorld(){
    cleanUp();
}

void StudentWorld::cleanUp(){
    list<Actor*>::iterator it = actors.begin();
    while(it != actors.end()){
        delete *it;
        *it = nullptr;
        it++;
    }
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
                m_player = new Player(this, c, r);
                addActor(m_player);
            }
            else if(item == Level::wall)
                addActor(new Wall(this, c, r));
            else if(item == Level::marble)
                addActor(new Marble(this, c, r));
            else if(item == Level::pit)
                addActor(new Pit(this, c, r));
            else if(item == Level::horiz_ragebot)
                addActor(new RageBot(this, c, r, Actor::right));
            else if(item == Level::vert_ragebot)
                addActor(new RageBot(this, c, r, Actor::down));
//            else if(item == Level::crystal)
//                addActor(new Crystal(this, c, r));
        }
    }
    return GWSTATUS_CONTINUE_GAME;
}

int StudentWorld::move(){
    setGameStatText("Game will end when you type q");
    
    // ask all actors to do something during the tick
    list<Actor*>::iterator it = actors.begin();
    while(it != actors.end()){
        (*it)->doSomething();
        if(!m_player->isAlive())
            return GWSTATUS_PLAYER_DIED;
        it++;
    }
    
    // remove dead actors
    it = actors.begin();
    while(it != actors.end()){
        if(!(*it)->isAlive()){
            delete *it;
            *it = nullptr;
            it = actors.erase(it);
        }
        else
            it++;
    }
    
    return GWSTATUS_CONTINUE_GAME;
}

void StudentWorld::addActor(Actor* a){
    actors.push_back(a);
}

bool StudentWorld::canAgentMoveTo(Agent* agent, int x, int y, int dx, int dy) const{
    list<Actor*>::const_iterator it = actors.begin();
    while(it != actors.end()){
        if((*it)->getX() == x && (*it)->getY() == y && !(*it)->allowsAgentColocation()){
            // TODO: should i return true immediately? what other objects can be on a marble?
            if((*it)->bePushedBy(agent, x + dx, y + dy)) // exception: marble can be pushed away
                return true;
            else
                return false;
        }
        it++;
    }
    return true;
}

bool StudentWorld::canMarbleMoveTo(int x, int y) const{
    list<Actor*>::const_iterator it = actors.begin();
    while(it != actors.end()){
        if((*it)->getX() == x && (*it)->getY() == y && !(*it)->allowsMarbleColocation())
            return false;
        it++;
    }
    return true;
}

bool StudentWorld::swallowSwallowable(Actor* a){
    list<Actor*>::iterator it = actors.begin();
    while(it != actors.end()){
        if((*it)->getX() == a->getX() && (*it)->getY() == a->getY() && *it != a){
            if((*it)->isSwallowable()){
                (*it)->setDead(); // set swallowable object to dead
                return true;
            }
        }
        it++;
    }
    return false;
}

bool StudentWorld::damageSomething(Actor* a, int damageAmt){
    list<Actor*>::iterator it = actors.begin();
    while(it != actors.end()){
        if((*it)->getX() == a->getX() && (*it)->getY() == a->getY() && *it != a){
            if((*it)->isDamageable()){
                (*it)->damage(damageAmt);
                return true;
            }
            if((*it)->stopsPea()){
                return true;
            }
        }
        it++;
    }
    return false;
}

bool StudentWorld::existsClearShotToPlayer(int x, int y, int dx, int dy) const{
    if(x != m_player->getX() && y != m_player->getY())
        return false; // not in same column or row
    
    // establish range between actor and player
    int startX, startY, endX, endY;
    startX = endX = x;
    startY = endY = y;
    
    if(dx == 1 && dy == 0) {
        if(x > m_player->getX() || x == m_player->getX()) // facing right BUT player is to the left OR not same x
           return false;
        if(x + 1 == m_player->getX())
            // TODO: return true if the actor is right next to player BUT what if there is something on player's spot as well?
            return true;
        startX++;
        endX = m_player->getX() - 1;
    }
    else if(dx == -1 && dy == 0){
        if(x < m_player->getX()  || x == m_player->getX())
            return false; // facing left BUT player is to the right OR not same x
        if(x - 1 == m_player->getX())
            return true;
        endX--;
        startX = m_player->getX() + 1;
    }
    else if(dx == 0 && dy == 1){
       if(y > m_player->getY() || y == m_player->getY()) // facing up BUT player is south OR not same y
           return false;
        if(y + 1 == m_player->getY())
            return true;
        startY++;
        endY = m_player->getY() - 1;
    }
    else{
        if(y < m_player->getY() || y == m_player->getX()) // facing down BUT player is north OR not same y
            return false;
        if(y - 1 == m_player->getY())
            return true;
        endY--;
        startY = m_player->getY() + 1;
    }
    
    list<Actor*>::const_iterator it = actors.begin();
    while(it != actors.end()){
        if((*it)->getX() >= startX && (*it)->getX() <= endX && (*it)->getY() >= startY && (*it)->getY() <= endY && (*it)->stopsPea())
            return false;
        it++;
    }
    return true;
}
