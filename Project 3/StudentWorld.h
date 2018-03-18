#ifndef STUDENTWORLD_H_
#define STUDENTWORLD_H_

#include "GameWorld.h"
#include <string>
#include <list>
using namespace std;
// Students:  Add code to this file, StudentWorld.cpp, Actor.h, and Actor.cpp
class Actor;
class NachenBlaster;

class StudentWorld : public GameWorld
{
public:
    StudentWorld(std::string assetDir);
    ~StudentWorld();
    virtual int init();
    virtual int move();
    virtual void cleanUp();
    // If there's at least one alien that's collided with a, return
    // a pointer to one of them; otherwise, return a null pointer.
    Actor* getOneCollidingAlien(const Actor* a) const;
    
    // If the player has collided with a, return a pointer to the player;
    // otherwise, return a null pointer.
    NachenBlaster* getCollidingPlayer(const Actor* a) const;
    
    // Is the player in the line of fire of a, which might cause a to attack?
    bool playerInLineOfFire(const Actor* a) const;
    
    // Add an actor to the world.
    void addActor(Actor* a);
    
    // Record that one more alien on the current level has been destroyed.
    void recordAlienDestroyed();
    
    double euclidean_dist(double x1, double y1, double x2, double y2) const;
    
    

private:
    list<Actor*> m_actors;
    NachenBlaster* m_blaster;
    int numAlDestroy;
    int numAliens;
};

#endif // STUDENTWORLD_H_

