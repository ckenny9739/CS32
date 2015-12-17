//
//  character.cpp
//  HW3
//
//  Created by Connor Kenny on 2/4/15.
//  Copyright (c) 2015 Connor Kenny. All rights reserved.
//

//Your declarations and implementations would go here

class Character
{
public:
    Character(string name)
    : m_name(name) {}
    virtual ~Character() {}
    string name() const {return m_name;}
    virtual void printWeapon() const = 0;
    virtual string attackAction() const
    {
        return "rushes toward the enemy";
    }
    
private:
    string m_name;
};

class Elf : public Character
{
public:
    Elf(string name, int numArrows)
    : Character(name), m_arrows(numArrows)
    {}
    
    ~Elf()
    {
        cout << "Destroying " << name() << " the elf" << endl;
    }
    
    virtual void printWeapon() const
    {
        cout << "a bow and quiver of " << m_arrows << " arrows";
    }
    
private:
    int m_arrows;
};

class Dwarf : public Character
{
public:
    Dwarf(string name)
    : Character(name)
    {}

    ~Dwarf()
    {
        cout << "Destroying " << name() << " the dwarf" << endl;
    }
    
    virtual void printWeapon() const
    {
        cout << "an axe";
    } 
};

class Boggie : public Character
{
public:
    Boggie(string name)
    : Character(name)
    {}
    
    ~Boggie()
    {
        cout << "Destroying " << name() << " the boggie" << endl;
    }
    
    virtual void printWeapon() const
    {
        cout << "a short sword";
    }
    
    virtual string attackAction() const
    {
        return "whimpers";
    }
};