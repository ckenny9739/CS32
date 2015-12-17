#ifndef STUDENTWORLD_H_
#define STUDENTWORLD_H_

#include "GraphObject.h"
#include "GameWorld.h"
#include "GameConstants.h"
#include <vector>
#include <string>

// Students:  Add code to this file, StudentWorld.cpp, Actor.h, and Actor.cpp

class Actor;
class Player;

class StudentWorld : public GameWorld
{
public:
    // Constructor + Destructor
    StudentWorld(std::string assetDir);
    virtual ~StudentWorld();
    
    // Display Text
    void updateDisplayText();
    std::string format(int score, int level, int lives, int health, int ammo, unsigned int bonus);
    
    // Destroy actors
    void destroy(std::vector<Actor*>::iterator p, size_t& k);
    
    // Accessors
    std::vector<Actor*> getActors() const;
    Player* getPlayer() const;
    int getJewels() const;
    int getBonus() const;
    
    // Add Actors
    void addBullet(int x, int y, GraphObject::Direction d);
    void addPickupable(int x, int y, int ImageID);
    void addKleptoBot(int x, int y, bool isAngryKleptoBot);
    
    // Complete Level
    void beatLevel();
    
    // GameWorld Functions
    virtual int init();
    virtual int move();
    virtual void cleanUp();
private:
    Player* m_player;
    std::vector<Actor*> m_actors;
    int m_levelBonus;
    int m_numJewels;
    bool m_completedLevel;
};

#endif // STUDENTWORLD_H_
