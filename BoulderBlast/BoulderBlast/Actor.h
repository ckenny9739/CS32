#ifndef ACTOR_H_
#define ACTOR_H_

#include "GraphObject.h"
#include "StudentWorld.h"

// Students:  Add code to this file, Actor.cpp, StudentWorld.h, and StudentWorld.cpp

// Actor - Base class for all actors

class Actor : public GraphObject
{
public:
    virtual ~Actor() {};
    Actor(int imageID, int x, int y, StudentWorld* sw, Direction dir = none, bool walk = false);
    
    virtual bool doSomething() = 0;
    
    StudentWorld* getWorld() const;
    bool getWalkable() const;
    bool getAlive() const;
    int getImageID() const;
    void setDead();
    
private:
    StudentWorld* m_World;
    bool m_walkable;
    bool m_alive;
    int m_imageID;
};


// Pickupable - Accounts for Jewel, RestoreHealthGoodie, ExtraLifeGoodie, and AmmoGoodie

class Pickupable : public Actor
{
public:
    Pickupable(int imageID, int x, int y, StudentWorld* sw);
    
    virtual bool doSomething();
private:
};


// ActorWithHealth - Accounts for Player, Boulder and assorted Bots

class ActorWithHealth : public Actor
{
public:
    ActorWithHealth(int imageID, int x, int y, StudentWorld* sw, int health, Direction dir = none, bool walk = false);
    
    int getHealth() const;
    void decHealth();
    void setHealth(int health);
    
    virtual bool objInWay(int dx, int dy, int& i) const;
    virtual bool getAttacked();
    virtual bool shoot();
private:
    int m_health;
};

class Player : public ActorWithHealth
{
public:
    Player(int startX, int startY, StudentWorld* sw);
    
    virtual bool doSomething();
    virtual bool objInWay(int dx, int dy, int& i);
    virtual bool getAttacked();
    virtual bool shoot();
    
    void restoreHealth();
    int getAmmo() const;
    void increaseAmmo();
private:
    int m_ammo;
};

class Boulder : public ActorWithHealth
{
public:
    Boulder(int x, int y, StudentWorld* sw);
    
    virtual bool doSomething() {return Actor::getAlive();}
    
    void push(int dx, int dy);
private:
    int m_health;
};


// Bot - Accounts for SnarlBots and KleptoBots

class Bot : public ActorWithHealth
{
public:
    Bot(int imageID, int x, int y, StudentWorld* sw, int health, Direction d);
    
    virtual bool doSomething();
    virtual bool getAttacked();
    virtual bool shoot();
    
    void resetTicks();
    
private:
    int m_ticks;
};

class SnarlBot : public Bot
{
public:
    SnarlBot(int x, int y, StudentWorld* sw, Direction d);
    
    virtual bool doSomething();
    virtual bool objInWay(int dx, int dy, int& i);
    virtual bool getAttacked();
private:
};

class KleptoBot : public Bot
{
public:
    KleptoBot(int x, int y, StudentWorld* sw, int health = 5, int imageID = IID_KLEPTOBOT);
    
    virtual bool doSomething();
    virtual bool getAttacked();
    
    bool move();
    void pickDirection(int d);
private:
    int m_distanceBeforeTurning;
    int m_distanceMoved;
    int m_stolenGoodie;
};

class AngryKleptoBot : public KleptoBot
{
public:
    AngryKleptoBot(int x, int y, StudentWorld* sw);
    
    virtual bool doSomething();
private:
};


// Other - Account for other objects that derive only from Actor

class Wall : public Actor
{
public:
    Wall(int x, int y, StudentWorld* sw);
    
    virtual bool doSomething() {return true;}
private:
};

class Hole : public Actor
{
public:
    Hole(int x, int y, StudentWorld* sw);
    
    virtual bool doSomething();
private:
};

class Bullet : public Actor
{
public:
    Bullet(int x, int y, StudentWorld* sw, Direction d);
    
    virtual bool doSomething();
    
    bool checkCollide();
private:    
};

class Exit : public Actor
{
public:
    Exit(int x, int y, StudentWorld* sw);
    
    virtual bool doSomething();
private:
    bool m_revealed;
};

class KleptoBotFactory : public Actor
{
public:
    KleptoBotFactory(int x, int y, StudentWorld* sw, bool ProducesAngryKleptoBots);
    
    virtual bool doSomething();
private:
    bool m_ProducesAngryKleptoBots;
};

#endif // ACTOR_H_
