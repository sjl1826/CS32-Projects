#include "Actor.h"
#include "StudentWorld.h"
// Students:  Add code to this file, Actor.h, StudentWorld.h, and StudentWorld.cpp


///////////////////////BASE ACTOR CLASS
Actor::Actor(const int imageID, double startX, double startY, StudentWorld*sworld, double size, int depth, int dir) : GraphObject(imageID, startX, startY, dir, size, depth)
{
    m_world = sworld;
    m_life = true;
}

Actor::~Actor()
{}
///////////////////////CODE FOR OTHER CLASSES
void Actor::increaseTorpedoes(int amt){}
void Actor::sufferDamage(double amt, int cause){}
double Actor::hitPoints() const{return 0;}
void Actor::increaseHitPoints(double amt){}
bool Actor:: isAlien() const
{return false;} //Default not aliens, aliens inherit this function and return true
///////////////////////CODE FOR OTHER CLASSES

///////////////////////KEEPS TRACK OF LIFE
bool Actor::isAlive() const
{return m_life;} //Keep track of life
void Actor::kill()
{m_life = false;}
///////////////////////KEEPS TRACK OF LIFE

///////////////////////ACCESSORS and ACTIONS
StudentWorld* Actor::getWorld()
{return m_world;}

void Actor::moveTo(int x, int y)
{GraphObject::moveTo(x,y);}

bool Actor::collision()
{
    NachenBlaster* temp = getWorld()->getCollidingPlayer(this);  //get a pointer to the blaster if it collides
    if(temp != nullptr) //return true if it is valid
    {
        return true;
    }
    return false;
}



///////////////////////STAR CLASS
Star::Star(double x, double y, double size, StudentWorld*sworld, int depth, int dir) : Actor(IID_STAR, x, y, sworld, size, depth, dir)
{}
void Star::doSomething()
{moveTo(getX()-1, getY());}



///////////////////////EXPLOSION CLASS
Explosion::Explosion(StudentWorld* w, double startX, double startY): Actor(IID_EXPLOSION, startX, startY, w)
{ explosiontrack = 0; }

void Explosion::doSomething()
{
    if(explosiontrack<4) //Explosion lasts for only 4 ticks
    {
    double size = 1.5 * getSize(); //Increase size by 1.5
    setSize(size);
    explosiontrack++; //keep track of the 4 ticks
    }
    else
    {
        explosiontrack = 0;
        kill();
    }
}



///////////////////////DAMAGEABLE OBJECTS CLASS
DamageableObject::DamageableObject(StudentWorld* w, double startX, double startY, int imageID, int startDir, double size, int depth, double hitPoints): Actor(imageID, startX, startY, w, size, depth, startDir)
{
    if(hitPoints == 1) //Hitpoint values 1 and 2 are set to match specific alien Hitpoints
    {
        double hp = (5*(1+(getWorld()->getLevel()-1)*0.1)); //If it is a Smallgon or a Smoregon, this is the Starting Hitpoints
        m_HP = hp;
    }
    else if(hitPoints == 2)
    {
        double dp = (10*(1+(getWorld()->getLevel()-1)*0.1)); //Snagglegons have more Hitpoints
        m_HP = dp;
    }
    else
        m_HP = hitPoints; //Set Hitpoints equal to the Hitpoint value if passed in, not containing signal values 1 and 2
}



///////////////////////ACCESSORS/ACTIONS
double DamageableObject::hitPoints() const
{return m_HP;}
void DamageableObject::increaseHitPoints(double amt)
{m_HP+=amt;}
void DamageableObject::sufferDamage(double amt, int cause)
{m_HP-=amt;}



///////////////////////NACHENBLASTER CLASS
NachenBlaster::NachenBlaster(StudentWorld*sworld): DamageableObject(sworld, 0, 128, IID_NACHENBLASTER, 0, 1.0, 0, 50)
{
    m_torpedoes= 0;
    m_cabenergy = 30;
}

void NachenBlaster::doSomething()
{
    if(isAlive()) //Only do Something if it is alive
    {
        int key = 0;
        if(m_cabenergy<30) //Refill Cabbage Energy
            m_cabenergy+=1;
        if(getWorld()->getKey(key))
        {
            switch(key)
            {
                case KEY_PRESS_SPACE: //Shoot Cabbages
                    if(m_cabenergy>=5)
                    {
                        m_cabenergy-=5;
                        getWorld()->addActor(new Cabbage(getWorld(), getX()+12, getY()));
                        getWorld()->playSound(SOUND_PLAYER_SHOOT);
                    }
                        break;
                case KEY_PRESS_LEFT: //Move left
                    if(getX()>6)
                        moveTo(getX()-6, getY());
                    break;
                case KEY_PRESS_RIGHT: //Move right
                    if(getX()<=VIEW_WIDTH)
                        moveTo(getX()+6, getY());
                    break;
                case KEY_PRESS_UP: //Move up
                    if(getY()<=VIEW_HEIGHT)
                        moveTo(getX(), getY()+6);
                    break;
                case KEY_PRESS_DOWN: //move down
                    if(getY()>=0)
                        moveTo(getX(), getY()-6);
                    break;
                case KEY_PRESS_TAB: //TORPEDOES
                    if(m_torpedoes>0)
                    {
                        m_torpedoes-=1;
                        getWorld()->addActor(new PlayerLaunchedTorpedo(getWorld(), getX()+12, getY()));
                        getWorld()->playSound(SOUND_TORPEDO);
                    }
                    break;
                default: return;
        }
        }
    }
}

void NachenBlaster::increaseHitPoints(double amt)
{
    if(hitPoints()>=40)
    {
        double d = 50-hitPoints();
        DamageableObject::increaseHitPoints(d);
    }
    else
        DamageableObject::increaseHitPoints(amt);
}
void NachenBlaster::sufferDamage(double amt, int cause) //subtract the damage and check if Hitpoints are lower than 0
{
    DamageableObject::sufferDamage(amt, cause);
    if(hitPoints()<=0)
    {
        kill(); //kill if dead
    }
}

void NachenBlaster::increaseTorpedoes(int amt)
{
    m_torpedoes+=amt;
}
int NachenBlaster::healthPct() const
{
    double health = DamageableObject::hitPoints(); //Take the health percentage
    health/=50;
    health*=100;
    int finalHealth = health;
    return finalHealth;
}
int NachenBlaster::cabbagePct() const
{//Calculating the cabbage percentage
    int pct = m_cabenergy*100;
    pct /=30;
    return pct;
}
int NachenBlaster::numTorpedoes() const
{
    return m_torpedoes;
}



///////////////////////ALIEN CLASS
Alien::Alien(StudentWorld* w, double startX, double startY, int imageID, double speed, double alt, double HP): DamageableObject(w, startX, startY, imageID, 0, 1.5, 1, HP)
{
    m_travelspeed = speed;
    m_travellength = 0;
    alienType = alt;
}
bool Alien::isAlien() const
{ //Aliens are always aliens
    return true;
}
void Alien::sufferDamage(double amt, int cause)
{
    DamageableObject::sufferDamage(amt, cause); //Cause Damage and subtract Hitpoints
    if(cause == HIT_BY_PROJECTILE) //If hit by projectile
    {
        if(hitPoints()<=0) //Check for low hitpoints
        {
            getWorld()->recordAlienDestroyed(); //increment alien destruction counter
            if(alienType == 0) //Check to see what type of alien it is
            {
                getWorld()->increaseScore(250); //Increase score
                kill();
                getWorld()->playSound(SOUND_DEATH);
                getWorld()->addActor(new Explosion(getWorld(), getX(), getY())); //Adding an explosion
            }
            else if(alienType == 1)
            {
                getWorld()->increaseScore(250);
                kill();
                getWorld()->playSound(SOUND_DEATH);
                getWorld()->addActor(new Explosion(getWorld(), getX(), getY()));
                int n = randInt(1, 3); //Random Probability
                if(n == 1)
                {
                    int g = randInt(1,2);
                    if(g == 1)
                    {
                        getWorld()->addActor(new TorpedoGoodie(getWorld(), getX(), getY()));
                    }
                    else
                        getWorld()->addActor(new RepairGoodie(getWorld(), getX(), getY()));
                }
            }
            else if(alienType == 2)
            {
                getWorld()->increaseScore(1000);
                kill();
                getWorld()->playSound(SOUND_DEATH);
                getWorld()->addActor(new Explosion(getWorld(), getX(), getY()));
                int t = randInt(1, 6);
                if(t == 1)
                {
                    getWorld()->addActor(new ExtraLifeGoodie(getWorld(), getX(), getY()));
                }
            }
        }
        else
            getWorld()->playSound(SOUND_BLAST);
    }
    else if(cause == HIT_BY_SHIP)
    {
        getWorld()->recordAlienDestroyed();
        if(alienType==0)
        {
            kill();
            getWorld()->increaseScore(250);
            getWorld()->playSound(SOUND_DEATH);
            getWorld()->addActor(new Explosion(getWorld(), getX(), getY()));
        }
        if(alienType==1)
        {
            kill();
            getWorld()->increaseScore(250);
            getWorld()->playSound(SOUND_DEATH);
            getWorld()->addActor(new Explosion(getWorld(), getX(), getY()));
            int d = randInt(1, 3);
            if(d == 1)
            {
                int f = randInt(1,2);
                if(f == 1)
                {
                    getWorld()->addActor(new TorpedoGoodie(getWorld(), getX(), getY()));
                }
                else
                    getWorld()->addActor(new RepairGoodie(getWorld(), getX(), getY()));
            }
        }
        if(alienType == 2)
        {
            kill();
            getWorld()->increaseScore(1000);
            getWorld()->playSound(SOUND_DEATH);
            getWorld()->addActor(new Explosion(getWorld(), getX(), getY()));
            int l = randInt(1, 6);
            if(l == 1)
            {
                getWorld()->addActor(new ExtraLifeGoodie(getWorld(), getX(), getY()));
            }
        }
    }
}


void Alien::setSpeed(double speed)
{
    m_travelspeed = speed;
}

bool Alien::damageCollidingPlayer(double amt)
{
    NachenBlaster* coll = getWorld()->getCollidingPlayer(this);
    if(coll != nullptr)
    {
        coll->sufferDamage(amt, HIT_BY_SHIP);
        sufferDamage(amt, HIT_BY_SHIP);
        return true;
    }
    return false;
}

double Alien::getLength()
{
    return m_travellength;
}

void Alien::setDir(double direction)
{
    m_dir = direction;
}

double Alien::travel()
{
    return m_travellength;
}

void Alien::setLength(double length)
{
    m_travellength = length;
}

double Alien::getAlienType()
{
    return alienType;
}

double Alien::getaldir()
{
    return m_dir;
}

double Alien::getSpeed()
{
    return m_travelspeed;
}

NachenBlaster* Alien::getBlaster()
{
    return m_blaster;
}



///////////////////////SMALLGON ALIEN CLASS
Smallgon::Smallgon(StudentWorld* w, double startX, double startY): Alien(w, startX, startY, IID_SMALLGON, 2, 0, 1)
{}


void Smallgon::doSomething()
{
    if(!isAlive())
        return;
    if(damageCollidingPlayer(5))
        return;
    if(getY()>= VIEW_HEIGHT-1)
        setDir(225);
    if(getY() <= 0)
        setDir(135);
    if(travel()==0)
    {
        int n = randInt(1,3);
        if(n == 1)
            setDir(180);
        if(n ==2)
            setDir(135);
        if(n==3)
            setDir(225);
    }
    double r = randInt(1, 32);
    setLength(r);
    if(getWorld()->playerInLineOfFire(this))
    {
        int g = randInt(1, (20/((getWorld()->getLevel())))+5);
        if(g == 1)
        {
            getWorld()->addActor(new Turnip(getWorld(), getX()-14, getY()));
            getWorld()->playSound(SOUND_ALIEN_SHOOT);
            return;
        }
    }
    setLength(getLength()-1);
    if(getaldir()==135){moveTo(getX()-getSpeed(), getY()+getSpeed());}
    if(getaldir()==225){moveTo(getX()-getSpeed(), getY()-getSpeed());}
    if(getaldir()==180){moveTo(getX()-getSpeed(), getY());}
    damageCollidingPlayer(5);
                
}


///////////////////////SMOREGON ALIEN CLASS
Smoregon::Smoregon(StudentWorld* w, double startX, double startY): Alien(w, startX, startY, IID_SMOREGON, 2, 1, 1)
{
}

void Smoregon::doSomething()
{
    if(!isAlive())
        return;
    damageCollidingPlayer(5);
    if(getY()>=VIEW_HEIGHT-1)
        setDir(225);
    else if(getY()<=0)
        setDir(135);
    else if(getLength() == 0)
    {
        int n = randInt(1,3);
        if(n == 1)
            setDir(180);
        if(n ==2)
            setDir(135);
        if(n==3)
            setDir(225);
    }
    if(getSpeed()!=5)
    {
    double r = randInt(1, 32);
    setLength(r);
    }
    if(getWorld()->playerInLineOfFire(this))
    {
        int g = randInt(1, (20/(getWorld()->getLevel()))+5);
        if(g == 5)
        {
            getWorld()->addActor(new Turnip(getWorld(), getX()-14, getY()));
            getWorld()->playSound(SOUND_ALIEN_SHOOT);
            return;
        }
        if(g == 2)
        {
            setDir(180);
            setLength(VIEW_WIDTH);
            setSpeed(5);
            return;
        }
    }
    setLength(getLength()-1);
    if(getaldir()==135){moveTo(getX()-getSpeed(), getY()+getSpeed());}
    if(getaldir()==225){moveTo(getX()-getSpeed(), getY()-getSpeed());}
    if(getaldir()==180){moveTo(getX()-getSpeed(), getY());}
    damageCollidingPlayer(5);
}



///////////////////////SNAGGLEGON ALIEN CLASS
Snagglegon::Snagglegon(StudentWorld* w, double startX, double startY):  Alien(w, startX, startY, IID_SNAGGLEGON, 1.75, 2, 2)
{
    setDir(225);
}

void Snagglegon::doSomething()
{
    if(!isAlive())
        return;
    damageCollidingPlayer(15);
    if(getY()>=VIEW_HEIGHT-1)
        setDir(225);
    else if(getY()<=0)
        setDir(135);
    if(getWorld()->playerInLineOfFire(this))
    {
        int g = randInt(1, (20/(getWorld()->getLevel()))+5);
        if(g == 1)
        {
            getWorld()->addActor(new AlienLaunchedTorpedo(getWorld(), getX()-14, getY()));
            getWorld()->playSound(SOUND_TORPEDO);
            return;
        }
    }
    if(getaldir()==135){moveTo(getX()-getSpeed(), getY()+getSpeed());}
    if(getaldir()==225){moveTo(getX()-getSpeed(), getY()-getSpeed());}
    damageCollidingPlayer(15);
    
}



///////////////////////PROJECTILE CLASS
Projectile::Projectile(StudentWorld* w, double startX, double startY, int imageID, double damageAmt, double deltaX, bool rotates, int imageDir): Actor(imageID, startX, startY, w, 0.5, 1, imageDir)
{
    m_damage = damageAmt;
    m_deltX = deltaX;
    m_rotate = rotates;
}

bool Projectile::collision(double damage)
{
    Actor* temp1 = getWorld()->getOneCollidingAlien(this);
    if(temp1 != nullptr)
    {
        temp1->sufferDamage(damage, HIT_BY_PROJECTILE);
        kill();
        return true;
    }
    return false;
}

double Projectile::getDamage()
{
    return m_damage;
}

void Projectile::projMovement(double pixel, bool rot)
{
    moveTo(getX()+pixel,getY());
    if(rot)
        setDirection(getDirection()+20);
}

double Projectile::getDeltX()
{
    return m_deltX;
}

bool Projectile::getRotate()
{
    return m_rotate;
}


///////////////////////CABBAGE CLASS
Cabbage::Cabbage(StudentWorld* w, double startX, double startY): Projectile(w, startX, startY, IID_CABBAGE, 2, 8, true, 0)
{
}

void Cabbage::doSomething()
{
    if(isAlive())
    {
        if(getX()>=VIEW_WIDTH)
        {
            kill();
            return;
        }
        else
        {
            if(collision(getDamage()))
                return;
        }
        projMovement(getDeltX(), getRotate());
        collision(getDamage());
    }
}


///////////////////////TURNIP CLASS
Turnip::Turnip(StudentWorld* w, double startX, double startY): Projectile(w, startX, startY, IID_TURNIP, 2, -6, true, 0)
{
}

void Turnip::doSomething()
{
    if(isAlive())
    {
        if(getX()<0)
        {
            kill();
            return;
        }
        NachenBlaster* temp1 = getWorld()->getCollidingPlayer(this);
        if(temp1 != nullptr)
        {
            temp1->sufferDamage(getDamage(), HIT_BY_PROJECTILE);
            kill();
            return;
        }
        projMovement(getDeltX(), getRotate());
        NachenBlaster* temp2 = getWorld()->getCollidingPlayer(this);
        if(temp2 != nullptr)
        {
            temp2->sufferDamage(getDamage(), HIT_BY_PROJECTILE);
            kill();
            return;
        }
    }
}


///////////////////////TORPEDO CLASS
Torpedo::Torpedo(StudentWorld* w, double startX, double startY, double deltaX, int imageDir): Projectile(w, startX, startY, IID_TORPEDO, 8, deltaX, false, imageDir)
{}


///////////////////////PLAYER LAUNCHED TORPEDO CLASS
PlayerLaunchedTorpedo::PlayerLaunchedTorpedo(StudentWorld* w, double startX, double startY): Torpedo(w, startX, startY, 8, 0)
{
}
void PlayerLaunchedTorpedo::doSomething()
{
    if(isAlive())
    {
        if(getX()<0 || getX()>=VIEW_WIDTH)
        {
            kill();
            return;
        }
        if(collision(getDamage()))
            return;
        projMovement(getDeltX(), getRotate());
        
    }
}


///////////////////////ALIEN LAUNCHED TORPEDO CLASS
AlienLaunchedTorpedo::AlienLaunchedTorpedo(StudentWorld* w, double startX, double startY): Torpedo(w, startX, startY, -8, 180)
{}

bool AlienLaunchedTorpedo::collision()
{
    NachenBlaster* temp1 = getWorld()->getCollidingPlayer(this);
    if(temp1 != nullptr)
    {
        temp1->sufferDamage(getDamage(), HIT_BY_PROJECTILE);
        kill();
        return true;
    }
    return false;
}

void AlienLaunchedTorpedo::doSomething()
{
    if(isAlive())
    {
        if(getX()<0 || getX()>=VIEW_WIDTH)
        {
            kill();
            return;
        }
        if(collision())
            return;
        projMovement(getDeltX(), getRotate());
        collision();
    }
}


///////////////////////GOODIE CLASS
Goodie::Goodie(StudentWorld* w, double startX, double startY, int imageID): Actor(imageID, startX, startY, w, 0.5)
{}

bool Goodie::onScreen()
{
    if(getX()<=0 || getX()>256 || getY()<=0 || getY()>256)
    {
        return false;
    }
    return true;
}

bool Goodie::collisCircumstance()
{
    if(collision())
    {
        getWorld()->increaseScore(100);
        kill();
        getWorld()->playSound(SOUND_GOODIE);
        return true;
    }
    return false;
}


///////////////////////EXTRA LIFE CLASS
ExtraLifeGoodie::ExtraLifeGoodie(StudentWorld* w, double startX, double startY): Goodie(w, startX, startY, IID_LIFE_GOODIE)
{
}


void ExtraLifeGoodie::doSomething()
{
    if(isAlive())
    {
        if(!onScreen())
        {
            kill();
            return;
        }
        if(collisCircumstance())
        {getWorld()->incLives();
            return;}
        moveTo(getX()-0.75, getY()-0.75);
        if(collisCircumstance())
        {
            getWorld()->incLives();
        }
    }
}


///////////////////////REPAIR CLASS
RepairGoodie::RepairGoodie(StudentWorld* w, double startX, double startY): Goodie(w, startX, startY, IID_REPAIR_GOODIE)
{
}

void RepairGoodie::doSomething()
{
    if(isAlive())
    {
        if(!onScreen())
        {
            kill();
            return;
        }
        if(collisCircumstance())
        {
            Actor* temp = getWorld()->getCollidingPlayer(this);
            temp->increaseHitPoints(10);
            return;
        }
        moveTo(getX()-0.75, getY()-0.75);
        if(collisCircumstance())
        {
            Actor* temp = getWorld()->getCollidingPlayer(this);
            temp->increaseHitPoints(10);
            return;
        }
    }
}


///////////////////////TORPEDO CLASS
TorpedoGoodie::TorpedoGoodie(StudentWorld* w, double startX, double startY): Goodie(w, startX, startY, IID_TORPEDO_GOODIE)
{
}

void TorpedoGoodie::doSomething()
{
    if(isAlive())
    {
        if(!onScreen())
        {
            kill();
            return; 
        }
        if(collisCircumstance())
        {
            Actor* temp = getWorld()->getCollidingPlayer(this);
            temp->increaseTorpedoes(5);
            return;
        }
        moveTo(getX()-0.75, getY()-0.75);
        if(collisCircumstance())
        {
            Actor* temp = getWorld()->getCollidingPlayer(this);
            temp->increaseTorpedoes(5);
            return;
        }
    }
}


