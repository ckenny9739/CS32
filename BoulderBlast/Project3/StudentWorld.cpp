  #include <sstream>
#include <iomanip>
#include "StudentWorld.h"
#include "Actor.h"
#include "Level.h"
using namespace std;

GameWorld* createStudentWorld(string assetDir)
{
    return new StudentWorld(assetDir);
}

// Students:  Add code to this file (if you wish), StudentWorld.h, Actor.h and Actor.cpp

StudentWorld::StudentWorld(std::string assetDir)
: GameWorld(assetDir), m_player(nullptr), m_numJewels(0), m_completedLevel(false), m_levelBonus(0)
{}

StudentWorld::~StudentWorld()
{
    delete m_player;
    
    vector<Actor*>::iterator p = m_actors.begin();              // Destroy each actor still alive
    for (size_t k = 0; k < m_actors.size(); k++)
        destroy(p, k);
}

void StudentWorld::updateDisplayText()
{
    int score = getScore();
    int level = getLevel();
    unsigned int bonus = m_levelBonus;
    int livesLeft = getLives();
    int health = getPlayer()->getHealth();
    int ammo = getPlayer()->getAmmo();
    
    string s = format(score, level, livesLeft, health, ammo, bonus);
    setGameStatText(s);
}

string StudentWorld::format(int score, int level, int lives, int health, int ammo, unsigned int bonus)
{
    ostringstream oss;
    oss << "Score: ";
    oss.fill('0');
    oss << setw(7) << score << "  Level: " << setw(2) << level;
    oss.fill(' ');
    oss <<  "  Lives: " << setw(2) << lives << "  Health: " << setw(3) << health * 100 / 20 << "%  Ammo: " << setw(3) << ammo << "  Bonus: " << setw(4) << bonus;
    return oss.str();
}

void StudentWorld::destroy(std::vector<Actor*>::iterator p, size_t& k)      // Destroy individual actor
{
    delete *(p+k);
    m_actors.erase(p+k);
    k--;
}

std::vector<Actor*> StudentWorld::getActors() const         // Returns vector of pointers to Actors
{
    return m_actors;
}

Player* StudentWorld::getPlayer() const
{
    return m_player;
}

int StudentWorld::getJewels() const
{
    return m_numJewels;
}

int StudentWorld::getBonus() const
{
    return m_levelBonus;
}
 
void StudentWorld::addBullet(int x, int y, GraphObject::Direction d)
{
    m_actors.push_back(new Bullet(x, y, this, d));
}

void StudentWorld::addPickupable(int x, int y, int ImageID)
{
    m_actors.push_back(new Pickupable(ImageID, x, y, this));
}

void StudentWorld::addKleptoBot(int x, int y, bool isAngryKleptoBot)
{
    if (isAngryKleptoBot)
        m_actors.push_back(new AngryKleptoBot(x, y, this));
    else
        m_actors.push_back(new KleptoBot(x, y, this));
}

void StudentWorld::beatLevel()
{
    m_completedLevel = true;
}

int StudentWorld::init()
{
    string curLevel;
    string s = to_string(getLevel());
    if (getLevel() < 10)
        curLevel = "level0" + s + ".dat";
    else
        curLevel = "level" + s + ".dat";
    
    Level lev(assetDirectory());
    Level::LoadResult result = lev.loadLevel(curLevel);
    
    if (getLevel() > 99 || result == Level::load_fail_file_not_found )      // No more levels left -> WIN
        return GWSTATUS_PLAYER_WON;
    
    if (result	==	Level:: load_fail_bad_format)
        return GWSTATUS_LEVEL_ERROR; //	something	bad	happened!
    //	otherwise	the	load	was	successful	and	you	can	access	the
    //	contents	of	the	level – here’s	an	example
    
    m_numJewels = 0;
    m_levelBonus = 1000;
    
    for(int x = 0; x < VIEW_WIDTH; x++)
        for(int y = 0; y < VIEW_HEIGHT; y++)
        {
            Level::MazeEntry item =	lev.getContentsOf(x,y);
            switch (item)
            {
                case Level::player:
                    m_player = new Player(x, y, this);
                    m_actors.push_back(m_player);
                    break;
                case Level::wall:
                    m_actors.push_back(new Wall(x, y, this));
                    break;
                case Level::hole:
                    m_actors.push_back(new Hole(x, y, this));
                    break;
                case Level::boulder:
                    m_actors.push_back(new Boulder(x, y, this));
                    break;
                case Level::jewel:
                    m_actors.push_back(new Pickupable(IID_JEWEL, x, y, this));
                    m_numJewels++;
                    break;
                case Level::extra_life:
                    m_actors.push_back(new Pickupable(IID_EXTRA_LIFE, x, y, this));
                    break;
                case Level::restore_health:
                    m_actors.push_back(new Pickupable(IID_RESTORE_HEALTH, x, y, this));
                    break;
                case Level::ammo:
                    m_actors.push_back(new Pickupable(IID_AMMO, x, y, this));
                    break;
                case Level::exit:
                    m_actors.push_back(new Exit(x, y, this));
                    break;
                case Level::horiz_snarlbot:
                    m_actors.push_back(new SnarlBot(x, y, this, GraphObject::right));
                    break;
                case Level::vert_snarlbot:
                    m_actors.push_back(new SnarlBot(x, y, this, GraphObject::down));
                    break;
                case Level::kleptobot_factory:
                    m_actors.push_back(new KleptoBotFactory(x, y, this, false));
                    break;
                case Level::angry_kleptobot_factory:
                    m_actors.push_back(new KleptoBotFactory(x, y, this, true));
                    break;
            }
        }
    return GWSTATUS_CONTINUE_GAME;
}

int StudentWorld::move()
{
    updateDisplayText();
    
    vector<Actor*>::iterator p = m_actors.begin();
    for (size_t k = 0; k < m_actors.size(); k++)
    {
        if(m_actors[k]->Actor::getAlive())
        {
            m_actors[k]->doSomething();
            if (!m_player->Actor::getAlive())           // If player has died
            {
                decLives();
                return GWSTATUS_PLAYER_DIED;
            }
            if (m_completedLevel)                       // If completed the level
                return GWSTATUS_FINISHED_LEVEL;
        }
    }
    for (size_t k = 0; k < m_actors.size(); k++)
    {
        if(!m_actors[k]->Actor::getAlive())
        {
            if (m_actors[k]->getImageID() == IID_JEWEL)     // If jewel, decrease number of jewels
                m_numJewels--;
            destroy(p, k);
        }
    }
    if (m_levelBonus > 0)               // Decrease level bonus
        m_levelBonus--;
    
    if (!m_player->Actor::getAlive())
        return GWSTATUS_PLAYER_DIED;
    
    if (m_completedLevel)
        return GWSTATUS_FINISHED_LEVEL;
    
    return GWSTATUS_CONTINUE_GAME;
}


void StudentWorld::cleanUp()
{
    m_completedLevel = false;                       // Level must not be completed before next level starts
    vector<Actor*>::iterator p = m_actors.begin();
    for (size_t k = 0; k < m_actors.size(); k++)
        destroy(p,k);
}
