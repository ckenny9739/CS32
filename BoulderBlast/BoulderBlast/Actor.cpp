#include <cstdlib>
#include <cmath>
#include "Actor.h"
#include "StudentWorld.h"
#include "Level.h"

// Students:  Add code to this file (if you wish), Actor.h, StudentWorld.h, and StudentWorld.cpp

// Actor

Actor::Actor(int imageID, int x, int y, StudentWorld* sw, Direction dir, bool walk)
: GraphObject(imageID, x, y, dir), m_World(sw), m_alive(true), m_walkable(walk), m_imageID(imageID)
{
    setVisible(true);
}

StudentWorld* Actor::getWorld() const
{
    return m_World;
}

bool Actor::getWalkable() const
{
    return m_walkable;
}

bool Actor::getAlive() const
{
    return m_alive;
}

int Actor::getImageID() const
{
    return m_imageID;
}

void Actor::setDead()
{
    m_alive = false;
}




// Pickupable

Pickupable::Pickupable(int imageID, int x, int y, StudentWorld* sw)
:Actor(imageID, x, y, sw, none, true)
{}

bool Pickupable::doSomething()
{
    if(!Actor::getAlive())
        return false;
    
    // If Player is on same position as Pickupable
    
    if (getWorld()->getPlayer()->getX() == getX() && getWorld()->getPlayer()->getY() == getY())
    {
        getWorld()->playSound(SOUND_GOT_GOODIE);
        
        switch (getImageID())
        {
            case IID_JEWEL:
                getWorld()->increaseScore(50);
                break;
            case IID_EXTRA_LIFE:
                getWorld()->increaseScore(1000);
                getWorld()->incLives();
                break;
            case IID_AMMO:
                getWorld()->increaseScore(100);
                getWorld()->getPlayer()->increaseAmmo();
                break;
            case IID_RESTORE_HEALTH:
                getWorld()->increaseScore(500);
                getWorld()->getPlayer()->restoreHealth();
                break;
        }
        setDead();
        return true;
    }
    return false;
}



// ActorWithHealth

ActorWithHealth::ActorWithHealth(int imageID, int x, int y, StudentWorld* sw, int health, Direction dir, bool walk)
:Actor(imageID, x, y, sw, dir, walk), m_health(health)
{}

int ActorWithHealth::getHealth() const
{
    return m_health;
}

void ActorWithHealth::decHealth()
{
    m_health -= 2;
}

void ActorWithHealth::setHealth(int health)
{
    m_health = health;
}

// If object, which is non-walkable, is in the next position the Actor is facing --> return true
bool ActorWithHealth::objInWay(int dx, int dy, int& i) const
{
    for (i = 0; i < getWorld()->getActors().size(); i++)
        if (!(getWorld()->getActors()[i]->getWalkable()) &&
            getWorld()->getActors()[i]->getX() == (getX() + dx) &&
            getWorld()->getActors()[i]->getY() == (getY() + dy))
            return true;
    return false;
}

// If dead, return false
bool ActorWithHealth::getAttacked()
{
    decHealth();
    if (getHealth() <= 0)       // If health < 0, set to dead + return false
    {
        setDead();
        return false;
    }
    return true;
}

bool ActorWithHealth::shoot()
{
    int xc = getX();
    int yc = getY();
    switch (getDirection())
    {
        case left:      xc--;       break;
        case right:     xc++;       break;
        case down:      yc--;       break;
        case up:        yc++;       break;
    }
    getWorld()->addBullet(xc, yc, getDirection());      // Shoots bullet in correct direction
    return true;
}


// Player

Player::Player(int startX, int startY, StudentWorld* sw)
: ActorWithHealth(IID_PLAYER, startX, startY, sw, 20, right), m_ammo(20)
{}

bool Player::doSomething()
{
    if(!Actor::getAlive())
        return false;
    
    int ch, i = 0;
    if (getWorld()->getKey(ch))
    {
        // user hit a key this tick!
        switch (ch)
        {
            case KEY_PRESS_ESCAPE:
                setDead();
                break;
            case KEY_PRESS_SPACE:
                if (m_ammo > 0)
                    shoot();
                break;
            case KEY_PRESS_LEFT:
                setDirection(left);
                if (!objInWay(-1, 0, i))
                    moveTo(getX()-1, getY());
                break;
            case KEY_PRESS_RIGHT:
                setDirection(right);
                if (!objInWay(1, 0, i))
                    moveTo(getX()+1, getY());
                break;
            case KEY_PRESS_DOWN:
                setDirection(down);
                if(!objInWay(0, -1, i))
                    moveTo(getX(), getY()-1);
                break;
            case KEY_PRESS_UP:
                setDirection(up);
                if(!objInWay(0, 1, i))
                    moveTo(getX(), getY()+1);
                break;
        }
    }
    return true;
}

// If object is in way, return true. (If boulder, call push member function)
bool Player::objInWay(int dx, int dy, int& i)
{
    if (ActorWithHealth::objInWay(dx, dy, i))
    {
        if (getWorld()->getActors()[i]->getImageID() == IID_BOULDER)
            dynamic_cast<Boulder*>(getWorld()->getActors()[i])->push(dx, dy);
        return true;
    }
    return false;
}

bool Player::getAttacked()
{
    if(!ActorWithHealth::getAttacked())         // If player has died
    {
        getWorld()->playSound(SOUND_PLAYER_DIE);
        return false;
    }
    getWorld()->playSound(SOUND_PLAYER_IMPACT);
    return true;
}

bool Player::shoot()
{
    ActorWithHealth::shoot();
    getWorld()->playSound(SOUND_PLAYER_FIRE);
    m_ammo--;
    return true;
}

void Player::restoreHealth()
{
    setHealth(20);              // Restore health to 20 (full health)
}

int Player::getAmmo() const
{
    return m_ammo;
}

void Player::increaseAmmo()
{
    m_ammo += 20;
}


// Boulder

Boulder::Boulder(int x, int y, StudentWorld* sw)
:ActorWithHealth(IID_BOULDER, x, y, sw, 10)
{}

// If object is not a hole, but is in direction of push, do nothing.
// Else (if is a hole), move player + boulder accordingly
void Boulder::push(int dx, int dy)
{
    for (int k = 0; k < getWorld()->getActors().size(); k++)
        if(getWorld()->getActors()[k]->getImageID() != IID_HOLE &&
           getWorld()->getActors()[k]->getX() == getX() + dx &&
           getWorld()->getActors()[k]->getY() == getY() + dy)
            return;
    
    getWorld()->getPlayer()->moveTo(getX(), getY());
    moveTo(getX() + dx, getY() + dy);
}


// Bot - Accounts for SnarlBots and KleptoBots


Bot::Bot(int imageID, int x, int y, StudentWorld* sw, int health, Direction d)
:ActorWithHealth(imageID, x, y, sw, health, d), m_ticks(1)
{}

// Sets ticks according to algorithm
bool Bot::doSomething()
{
    if(!Actor::getAlive())
        return false;
    
    if (m_ticks == 1)
    {
        m_ticks = (28 - getWorld()->getLevel()) / 4;
        if (m_ticks < 3)
            m_ticks = 3; // no Bot moves more frequently than this
        return true;
    }
    else
    {
        m_ticks--;
        return false;
    }
}

bool Bot::getAttacked()
{
    if(ActorWithHealth::getAttacked())          // If Bot is alive
    {
        getWorld()->playSound(SOUND_ROBOT_IMPACT);
        return true;
    }
    getWorld()->playSound(SOUND_ROBOT_DIE);
    return false;
}

// Check to see if player is in line of sight with nothing blocking.
// If something is in way, do nothing. If not, shoot in that direction.
bool Bot::shoot()
{
    bool objInView = false;
    switch (getDirection())
    {
        case left:
            if (getWorld()->getPlayer()->getY() == getY() && getWorld()->getPlayer()->getX() < getX())
            {
                for (int i = 0; i < getWorld()->getActors().size(); i++)
                {
                    if (!getWorld()->getActors()[i]->getWalkable() &&
                        getWorld()->getActors()[i]->getImageID() != IID_HOLE &&
                        getWorld()->getActors()[i]->getY() == getY() &&
                        getWorld()->getActors()[i]->getX() < getX() &&
                        getWorld()->getActors()[i]->getX() > getWorld()->getPlayer()->getX())
                    {
                        objInView = true;
                        break;
                    }
                }
                if (!objInView)
                {
                    ActorWithHealth::shoot();
                    getWorld()->playSound(SOUND_ENEMY_FIRE);
                    return true;
                }
            }
            break;
        case right:
            if (getWorld()->getPlayer()->getY() == getY() && getWorld()->getPlayer()->getX() > getX())
            {
                for (int i = 0; i < getWorld()->getActors().size(); i++)
                {
                    if (!getWorld()->getActors()[i]->getWalkable() &&
                        getWorld()->getActors()[i]->getImageID() != IID_HOLE &&
                        getWorld()->getActors()[i]->getY() == getY() &&
                        getWorld()->getActors()[i]->getX() > getX() &&
                        getWorld()->getActors()[i]->getX() < getWorld()->getPlayer()->getX())
                    {
                        objInView = true;
                        break;
                    }
                }
                if (!objInView)
                {
                    ActorWithHealth::shoot();
                    getWorld()->playSound(SOUND_ENEMY_FIRE);
                    return true;
                }
            }
            break;
        case up:
            if (getWorld()->getPlayer()->getX() == getX() && getWorld()->getPlayer()->getY() > getY())
            {
                for (int i = 0; i < getWorld()->getActors().size(); i++)
                {
                    if (!getWorld()->getActors()[i]->getWalkable() &&
                        getWorld()->getActors()[i]->getImageID() != IID_HOLE &&
                        getWorld()->getActors()[i]->getX() == getX() &&
                        getWorld()->getActors()[i]->getY() > getY() &&
                        getWorld()->getActors()[i]->getY() < getWorld()->getPlayer()->getY())
                    {
                        objInView = true;
                        break;
                    }
                }
                if (!objInView)
                {
                    ActorWithHealth::shoot();
                    getWorld()->playSound(SOUND_ENEMY_FIRE);
                    return true;
                }
            }
            break;
        case down:
            if (getWorld()->getPlayer()->getX() == getX() && getWorld()->getPlayer()->getY() < getY())
            {
                for (int i = 0; i < getWorld()->getActors().size(); i++)
                {
                    if (!getWorld()->getActors()[i]->getWalkable() &&
                        getWorld()->getActors()[i]->getImageID() != IID_HOLE &&
                        getWorld()->getActors()[i]->getX() == getX() &&
                        getWorld()->getActors()[i]->getY() < getY() &&
                        getWorld()->getActors()[i]->getY() > getWorld()->getPlayer()->getY())
                    {
                        objInView = true;
                        break;
                    }
                }
                if (!objInView)
                {
                    ActorWithHealth::shoot();
                    getWorld()->playSound(SOUND_ENEMY_FIRE);
                    return true;
                }
            }
            break;
    }
    return false;
}

void Bot::resetTicks()
{
    m_ticks = 1;
}


// Snarlbot - Horizontal and Vertical


SnarlBot::SnarlBot(int x, int y, StudentWorld* sw, Direction d)
:Bot(IID_SNARLBOT, x, y, sw, 10, d)
{}

bool SnarlBot::doSomething()
{
    if(!Bot::doSomething())
        return false;
    
    if (shoot())
        return true;
    
    int dx, dy, i;
    dx = dy = i = 0;
    switch (getDirection())             // Move in correct direction
    {
        case left: dx = -1; dy = 0; break;
        case right: dx = 1; dy = 0; break;
        case up: dx = 0; dy = 1; break;
        case down: dx = 0; dy = -1; break;
    }
    
    if (!objInWay(dx, dy, i))
        moveTo(getX() + dx, getY() + dy);
    
    return false;
}

// Changes direction if it hits something in the way
bool SnarlBot::objInWay(int dx, int dy, int& i)
{
    if(ActorWithHealth::objInWay(dx, dy, i))
    {
        switch (getDirection())
        {
            case left: setDirection(right); break;
            case right: setDirection(left); break;
            case up: setDirection(down); break;
            case down: setDirection(up); break;
        }
        return true;
    }
    return false;
}

bool SnarlBot::getAttacked()
{
    if(!Bot::getAttacked())
    {
        getWorld()->increaseScore(100);
        return false;
    }
    return true;
}

// KleptoBot
KleptoBot::KleptoBot(int x, int y, StudentWorld* sw, int health, int imageID)
:Bot(imageID, x, y, sw, health, right), m_distanceBeforeTurning(rand() % 6 + 1), m_distanceMoved(0), m_stolenGoodie(0)
{}

// If KleptoBot steps on Goodie (not including Jewel), it has 10% chance to pick it up
bool KleptoBot::doSomething()
{
    if(!Bot::doSomething())
        return false;
    
    for (int i = 0; i < getWorld()->getActors().size(); i++)
    {
        if (m_stolenGoodie == 0 &&
            dynamic_cast<Pickupable*>(getWorld()->getActors()[i]) != nullptr &&
            getWorld()->getActors()[i]->getImageID() != IID_JEWEL &&
            getWorld()->getActors()[i]->getX() == getX() &&
            getWorld()->getActors()[i]->getY() == getY())
        {
            int chance = rand() % 10 + 1;
            if (chance == 1)    // 10% chance
            {
                m_stolenGoodie = getWorld()->getActors()[i]->getImageID();
                getWorld()->getActors()[i]->setDead();              // Destroy Goodie to be recreated later
                getWorld()->playSound(SOUND_ROBOT_MUNCH);
                return true;
            }
        }
    }
    
    if (m_distanceMoved < m_distanceBeforeTurning)
    {
        if (move())
            return true;
        
        m_distanceMoved = 0;        // Restart sequence
    }
    
    // Pick new direction
    
    m_distanceBeforeTurning = rand() % 6 + 1;
    int d = rand() % 4;
    
    
    for(int i = 0; i < 4; i++)
    {
        pickDirection(d);
        if (move())
            return true;
        if (d != 3)
            d++;
        else
            d = 0;
    }
    
    pickDirection(d);
    return false;
}


bool KleptoBot::getAttacked()
{
    if(!Bot::getAttacked())
    {
        if (getImageID() == IID_ANGRY_KLEPTOBOT)
            getWorld()->increaseScore(20);
        else
            getWorld()->increaseScore(10);
        if (m_stolenGoodie != 0)
        {
            // Recreate Goodie if KleptoBot is killed
            getWorld()->addPickupable(getX(), getY(), m_stolenGoodie);
        }
        return false;
    }
    return true;
}


bool KleptoBot::move()
{
    int i, dx, dy;
    dx = dy = i = 0;

    switch (getDirection())
    {
        case left: dx = -1; dy = 0; break;
        case right: dx = 1; dy = 0; break;
        case up: dx = 0; dy = 1; break;
        case down: dx = 0; dy = -1; break;
    }
    
    if (!objInWay(dx, dy, i))
    {
        moveTo(getX()+dx, getY()+dy);
        m_distanceMoved++;
        return true;
    }
    
    return false;
}

void KleptoBot::pickDirection(int d)
{
    switch (d)
    {
        case 0: setDirection(left); break;
        case 1: setDirection(right); break;
        case 2: setDirection(up); break;
        case 3: setDirection(down); break;
    }
}


// AngryKleptoBot
AngryKleptoBot::AngryKleptoBot(int x, int y, StudentWorld* sw)
:KleptoBot(x, y, sw, 8, IID_ANGRY_KLEPTOBOT)
{}

bool AngryKleptoBot::doSomething()
{
    if (!Bot::doSomething())
        return false;
    
    if (shoot())
        return true;
    
    resetTicks();               // Adjust for calling Bot::doSomething earlier, which assigns a tick value
                                // Allow KleptoBot::doSomething to assign same value again if didn't shoot
    
    if (!KleptoBot::doSomething())
        return true;
    
    return false;
}

// Other


// Wall
 
Wall::Wall(int x, int y, StudentWorld* sw)
: Actor(IID_WALL, x, y, sw, none, false)
{}


// Hole

Hole::Hole(int x, int y, StudentWorld* sw)
:Actor(IID_HOLE, x, y, sw)
{}

// If boulder in same space, swallow boulder (which also fills hole)
bool Hole::doSomething()
{
    if(!Actor::getAlive())
        return false;

    for (size_t k = 0; k < getWorld()->getActors().size(); k++)
    {
        if(getWorld()->getActors()[k]->getImageID() == IID_BOULDER &&
           getWorld()->getActors()[k]->getX() == getX() &&
           getWorld()->getActors()[k]->getY() == getY())
        {
            getWorld()->getActors()[k]->setDead();      // Set boulder to dead
            setDead();
            return true;
        }
    }
    return true;
}

// Bullet

Bullet::Bullet(int x, int y, StudentWorld* sw, Direction d)
: Actor(IID_BULLET, x, y, sw, d, true)
{}

bool Bullet::doSomething()
{
    if(!Actor::getAlive())
        return false;
    
    if (checkCollide())
        return true;
    
    switch (getDirection())
    {
        case left:
            moveTo(getX()-1, getY());
            break;
        case right:
            moveTo(getX()+1, getY());
            break;
        case down:
            moveTo(getX(), getY()-1);
            break;
        case up:
            moveTo(getX(), getY()+1);
            break;
    }
    
    checkCollide();
    return true;
}

// Return true if collides with object and is set to dead
bool Bullet::checkCollide()
{
    for (size_t k = 0; k < getWorld()->getActors().size(); k++)
    {
        if (getWorld()->getActors()[k]->getX() == getX() && getWorld()->getActors()[k]->getY() == getY())
        {
                                                                        // Bullet hits ActorWithHealth
            if (dynamic_cast<ActorWithHealth*>(getWorld()->getActors()[k]) != nullptr)
            {
                dynamic_cast<ActorWithHealth*>(getWorld()->getActors()[k])->getAttacked();
                setDead();
                return true;
            }
            if(getWorld()->getActors()[k]->getImageID() == IID_ROBOT_FACTORY)   // Bullet hits Factory
            {
                for (size_t i = 0; i < getWorld()->getActors().size(); i++)
                {
                    if (dynamic_cast<KleptoBot*>(getWorld()->getActors()[i]) != nullptr &&
                        getWorld()->getActors()[i]->getX() == getX() &&
                        getWorld()->getActors()[i]->getY() == getY())
                    {
                        dynamic_cast<ActorWithHealth*>(getWorld()->getActors()[i])->getAttacked();
                        setDead();
                        return true;
                    }
                }
                setDead();
                return true;
            }
            if (getWorld()->getActors()[k]->getImageID() == IID_WALL)           // Bullet hits wall
            {
                setDead();
                return true;
            }
        }
    }
    return false;
}

// Exit

Exit::Exit(int x, int y, StudentWorld* sw)
:Actor(IID_EXIT, x, y, sw, none, true), m_revealed(false)
{
    setVisible(false);          // Starts off invisible
}

bool Exit::doSomething()
{
    if (getWorld()->getJewels() == 0 && m_revealed == false)            // Collected all jewels
    {
        m_revealed = true;
        setVisible(true);
        getWorld()->playSound(SOUND_REVEAL_EXIT);
    }
    if (getWorld()->getPlayer()->getX() == getX() &&
        getWorld()->getPlayer()->getY() == getY() && m_revealed)        // Completed Level
    {
        setDead();
        getWorld()->playSound(SOUND_FINISHED_LEVEL);
        getWorld()->increaseScore(2000 + getWorld()->getBonus());
        getWorld()->beatLevel();
    }
    return true;
}


// KleptoBotFactory
KleptoBotFactory::KleptoBotFactory(int x, int y, StudentWorld* sw, bool ProducesAngryKleptoBots)
:Actor(IID_ROBOT_FACTORY, x, y, sw, none, false), m_ProducesAngryKleptoBots(ProducesAngryKleptoBots)
{}

bool KleptoBotFactory::doSomething()
{
    int count = 0;
    bool botOnFactory = false;
    for (int i = 0; i < getWorld()->getActors().size(); i++)
    {
        if (dynamic_cast<KleptoBot*>(getWorld()->getActors()[i]) != nullptr)
        {
            if (getWorld()->getActors()[i]->getX() == getX() &&         // Check for KleptoBot on factory
                getWorld()->getActors()[i]->getY() == getY())
                botOnFactory = true;
            if(abs(getX() - getWorld()->getActors()[i]->getX()) <= 3 && // Count KleptoBots in range
               abs(getY() - getWorld()->getActors()[i]->getY()) <= 3)
                count++;
        }
    }
    
    if (count < 3 && !botOnFactory)
    {
        int chance = rand() % 50 + 1;
        if (chance == 1)        // 2% chance
        {
            if (m_ProducesAngryKleptoBots)
                getWorld()->addKleptoBot(getX(), getY(), true);     // Create AngryKleptoBot
            else
                getWorld()->addKleptoBot(getX(), getY(), false);    // Create KleptoBot
            getWorld()->playSound(SOUND_ROBOT_BORN);
        }
    }
    return false;
}



