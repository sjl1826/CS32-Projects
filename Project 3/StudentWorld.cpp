#include "StudentWorld.h"
#include "GameConstants.h"
#include "Actor.h"
#include <iostream>
#include <sstream>  // defines the type std::ostringstream
#include <iomanip>  // defines the manipulator setw
#include <string>
using namespace std;



GameWorld* createStudentWorld(string assetDir)
{
    return new StudentWorld(assetDir);
}

// Students:  Add code to this file, StudentWorld.h, Actor.h and Actor.cpp

StudentWorld::StudentWorld(string assetDir)
: GameWorld(assetDir)
{
    m_blaster = nullptr;
    numAlDestroy = 0;
    numAliens = 0;
}

StudentWorld::~StudentWorld()
{
    cleanUp();
}

int StudentWorld::init()
{
    numAlDestroy = 0;
    numAliens = 0;
    m_blaster = new NachenBlaster(this);
    for(int i = 0;i<30;i++)
    {
        double x = randInt(0, VIEW_WIDTH-1);
        double y = randInt(0, VIEW_HEIGHT-1);
        double size = randInt(5, 50);
        size/= 100;
        m_actors.push_back(new Star(x, y, size, this));
    }
    return GWSTATUS_CONTINUE_GAME;
}

int StudentWorld::move()
{
    // This code is here merely to allow the game to build, run, and terminate after you hit enter.
    // Notice that the return value GWSTATUS_PLAYER_DIED will cause our framework to end the current level.
    ostringstream oss;
    oss.setf(ios::fixed);
    oss.precision(2);
    oss << "Lives: " << getLives() << "  Health: "<< m_blaster->healthPct() <<"%  Score: "<< getScore() <<"  Level: "<<getLevel()<<"  Cabbages: "<<m_blaster->cabbagePct()<<"%  Torpedoes: "<<m_blaster->numTorpedoes();
    string s = oss.str();
    
    setGameStatText(s);
    int D = numAlDestroy;
    int remaining = (6+(4*getLevel()))-D;
    int M = 4 + (0.5*getLevel());
    if(remaining<M)
        M = remaining;
    if(numAliens<M)
    {
        int S1 = 60; int S2 = 20 + (getLevel()*5); int S3 = 5 + (getLevel()*10); int S = S1+S2+S3;
        int random = randInt(1, S);
        double rand = randInt(0, VIEW_HEIGHT-1);
        if(random<61)
        {
            m_actors.push_back(new Smallgon(this, VIEW_WIDTH-1, rand));
            numAliens++;
        }
        else if(random>=61 && random<(S-S3))
        {
            m_actors.push_back(new Smoregon(this, VIEW_WIDTH-1, rand));
            numAliens++;
        }
        else if(random>(S1+S2))
        {
            m_actors.push_back(new Snagglegon(this, VIEW_WIDTH-1, rand));
            numAliens++;
        }
    }
    
    list<Actor*>::iterator it = m_actors.begin();
    while(it!=m_actors.end())
    {
        if((*it)->getX() < 0)
        {
            (*it)->kill();
            it++;
            continue;
        }
        (*it)->doSomething();
            if(!m_blaster->isAlive())
            {
                decLives();
                return GWSTATUS_PLAYER_DIED;
            }
            if(numAlDestroy == (6+(4*getLevel())))
                return GWSTATUS_FINISHED_LEVEL;
            it++;
    }
    
    m_blaster->doSomething();
    
    int n = 0;
    n = randInt(1, 15);
    if(n == 5)
    {
        double x = VIEW_WIDTH-1;
        double y = randInt(0, VIEW_HEIGHT-1);
        double size = randInt(5, 50);
        size/= 100;
        m_actors.push_back(new Star(x, y, size, this));
    }
    
    
    list<Actor*>::iterator itr = m_actors.begin();
    while(itr!=m_actors.end())
    {
        if(!((*itr)->isAlive()))
        {
            if((*itr)->isAlien())
            {
                numAliens--;
            }
            list<Actor*>::iterator temp = itr;
            itr++;
            delete (*temp);
            m_actors.erase(temp);
        }
        else
            itr++;
    }
    
    return GWSTATUS_CONTINUE_GAME;
}

void StudentWorld::cleanUp()
{
    delete m_blaster;
    m_blaster = nullptr;
    list<Actor*>::iterator it = m_actors.begin();
    while(it!=m_actors.end())
    {
        list<Actor*>::iterator temp = it;
        it++;
        delete (*temp);
        m_actors.erase(temp);
    }
}

double StudentWorld::euclidean_dist(double x1, double y1, double x2, double y2) const
{
    double xpart = x1 - x2;
    double ypart = y1 - y2;
    double eucliddist;
    eucliddist = pow(xpart, 2) + pow(ypart, 2);
    eucliddist = sqrt(eucliddist);
    return eucliddist;
}

// If there's at least one alien that's collided with a, return
// a pointer to one of them; otherwise, return a null pointer.
Actor* StudentWorld::getOneCollidingAlien(const Actor* a) const
{
    list<Actor*>::const_iterator it = m_actors.begin();
    while(it!=m_actors.end())
    {
        if((*it)->isAlien())
        {
        if(euclidean_dist(a->getX(), a->getY(), (*it)->getX(), (*it)->getY())< (0.75*(a->getRadius()+(*it)->getRadius())))
            return (*it);
        }
        it++;
    }
    return nullptr;
}

// If the player has collided with a, return a pointer to the player;
// otherwise, return a null pointer.
NachenBlaster* StudentWorld::getCollidingPlayer(const Actor* a) const
{
    if(euclidean_dist(a->getX(), a->getY(), m_blaster->getX(), m_blaster->getY())< (0.75*(a->getRadius()+m_blaster->getRadius())))
        return m_blaster;
    return nullptr;
}

// Is the player in the line of fire of a, which might cause a to attack?
bool StudentWorld::playerInLineOfFire(const Actor* a) const
{
    if(m_blaster->getX() < a->getX() && (a->getY()>=m_blaster->getY()-4 && a->getY()<=m_blaster->getY()+4))
        return true;
    return false;
}

// Add an actor to the world.
void StudentWorld::addActor(Actor* a)
{
    m_actors.push_back(a);
}

// Record that one more alien on the current level has been destroyed.
void StudentWorld::recordAlienDestroyed()
{
    numAlDestroy++;
}




