#ifndef ACTOR_H_
#define ACTOR_H_

#include "GraphObject.h"

// Students:  Add code to this file, Actor.cpp, StudentWorld.h, and StudentWorld.cpp

const int HIT_BY_SHIP = 0;
const int HIT_BY_PROJECTILE = 1;

class StudentWorld;


class Actor : public GraphObject
{
public:
    Actor(const int imageID, double startX, double startY, StudentWorld*sworld, double size = 1.0, int depth = 0, int dir = 0);
    virtual ~Actor();
    virtual void doSomething() = 0;
    virtual void sufferDamage(double amt, int cause);
    bool isAlive() const;
    void kill();
    StudentWorld* getWorld();
    virtual bool isAlien() const;
    virtual void moveTo(int x, int y);
    virtual bool collision();
    virtual double hitPoints() const;
    virtual void increaseHitPoints(double amt);
    virtual void increaseTorpedoes(int amt);
private:
    bool m_life;
    StudentWorld* m_world;
};

class Star: public Actor
{
public:
    Star(double x, double y, double size, StudentWorld*sworld, int depth = 3, int dir = 0);
    virtual void doSomething();
};

class Explosion : public Actor
{
public:
    Explosion(StudentWorld* w, double startX, double startY);
    virtual void doSomething();
private:
    int explosiontrack;
};

class DamageableObject : public Actor
{
public:
    DamageableObject(StudentWorld* w, double startX, double startY, int imageID, int startDir, double size, int depth, double hitPoints);
    virtual double hitPoints() const;
    virtual void increaseHitPoints(double amt);
    virtual void sufferDamage(double amt, int cause);
private:
    double m_HP;
};

class NachenBlaster: public DamageableObject
{
public:
    NachenBlaster(StudentWorld* sworld);
    virtual void doSomething();
    virtual void increaseHitPoints(double amt);
    virtual void sufferDamage(double amt, int cause);
    virtual void increaseTorpedoes(int amt);
    int healthPct() const;
    int cabbagePct() const;
    int numTorpedoes() const;
private:
    int m_cabenergy;
    int m_torpedoes;
};

class Alien : public DamageableObject
{
public:
    Alien(StudentWorld* w, double startX, double startY, int imageID, double speed, double alt, double HP);
    virtual bool isAlien() const;
    virtual void sufferDamage(double amt, int cause);
    void setSpeed(double speed);
    virtual bool damageCollidingPlayer(double amt);
    NachenBlaster* getBlaster();
    double getLength();
    void setDir(double direction);
    double travel();
    void setLength(double length);
    double getAlienType();
    double getaldir();
    double getSpeed();
private:
    double m_travellength;
    NachenBlaster* m_blaster;
    double m_travelspeed;
    double m_dir;
    double alienType;
};

class Smallgon : public Alien
{
public:
    Smallgon(StudentWorld* w, double startX, double startY);
    virtual void doSomething();
};

class Smoregon : public Alien
{
public:
    Smoregon(StudentWorld* w, double startX, double startY);
    virtual void doSomething();
};

class Snagglegon : public Alien
{
public:
    Snagglegon(StudentWorld* w, double startX, double startY);
    virtual void doSomething();
};

class Projectile : public Actor
{
public:
    Projectile(StudentWorld* w, double startX, double startY, int imageID,
               double damageAmt, double deltaX, bool rotates, int imageDir);
    virtual bool collision(double damage);
    void projMovement(double pixel, bool rot);
    double getDamage();
    double getDeltX();
    bool getRotate();
private:
    double m_damage;
    double m_deltX;
    bool m_rotate;
};

class Cabbage : public Projectile
{
public:
    Cabbage(StudentWorld* w, double startX, double startY);
    virtual void doSomething();
};

class Turnip : public Projectile
{
public:
    Turnip(StudentWorld* w, double startX, double startY);
    virtual void doSomething();
};

class Torpedo : public Projectile
{
public:
    Torpedo(StudentWorld* w, double startX, double startY, double deltaX,
            int imageDir);
};

class PlayerLaunchedTorpedo : public Torpedo
{
public:
    PlayerLaunchedTorpedo(StudentWorld* w, double startX, double startY);
    virtual void doSomething();
};

class AlienLaunchedTorpedo : public Torpedo
{
public:
    AlienLaunchedTorpedo(StudentWorld* w, double startX, double startY);
    virtual void doSomething();
    virtual bool collision();
};

class Goodie : public Actor
{
public:
    Goodie(StudentWorld* w, double startX, double startY, int imageID);
    bool onScreen();
    bool collisCircumstance();
};

class ExtraLifeGoodie : public Goodie
{
public:
    ExtraLifeGoodie(StudentWorld* w, double startX, double startY);
    virtual void doSomething();
};

class RepairGoodie : public Goodie
{
public:
    RepairGoodie(StudentWorld* w, double startX, double startY);
    virtual void doSomething();
};

class TorpedoGoodie : public Goodie
{
public:
    TorpedoGoodie(StudentWorld* w, double startX, double startY);
    virtual void doSomething();
};







#endif // ACTOR_H_
