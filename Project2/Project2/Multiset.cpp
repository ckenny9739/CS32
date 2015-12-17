//
//  Multiset.cpp
//  Project2
//
//  Created by Connor Kenny on 1/21/15.
//  Copyright (c) 2015 Connor Kenny. All rights reserved.
//

#include "Multiset.h"

Multiset::Multiset()
: m_count(0)
{
    m_head = new Node;                                                 // Create dummy node
    m_head->m_next = m_head;
    m_head->m_prev = m_head;
}

Multiset::~Multiset()
{
    Node* p = m_head->m_next;
    while (p != m_head)
    {
        p = p->m_next;
        delete p->m_prev;                                               // Delete Nodes
    }
    delete m_head;                                                      // Delete dummy Node
}

Multiset::Multiset(const Multiset& other)
: m_count(0)                                                        // Incrememnted via insert()
{
    m_head = new Node;                                              // Create a new m_head AKA new list
    m_head->m_next = m_head;
    m_head->m_prev = m_head;
    
    for (Node* p = other.m_head->m_next; p != other.m_head; p = p->m_next)  // Insert value
        for (int i = 0; i < p->m_size; i++)
            insert(p->m_data);
}

Multiset& Multiset::operator=(const Multiset& rhs)
{
    if(this != &rhs)                                                // lhs is equal to copy of rhs
    {
        Multiset temp(rhs);
        swap(temp);
    }
    return *this;
}

bool Multiset::empty() const
{
    return !m_count;                               // No elements in list
}

int Multiset::size() const
{
    int size = 0;
    for (Node* p = m_head->m_next; p != m_head; p = p->m_next)    // Total number of elements in list
        size += p->m_size;
    return size;
}

int Multiset::uniqueSize() const
{
    return m_count;                                                 // Number of unique elements
}

bool Multiset::insert(const ItemType& value)
{
    for (Node* p = m_head->m_next; p != m_head; p = p->m_next)        // If already exists
    {
        if(p->m_data == value)
        {
            p->m_size++;
            return true;
        }
    }
    Node* insert = new Node;                                                    // New node
    insert->m_data = value;                                                     // Assign values
    insert->m_size = 1;
    m_count++;
    
    insert->m_next = m_head->m_next;
    m_head->m_next = insert;
    
    insert->m_prev = m_head;
    insert->m_next->m_prev = insert;
    
    return true;
}

int Multiset::erase(const ItemType& value)
{
    for (Node* p = m_head->m_next; p != m_head; p = p->m_next)                // Erase 1 or 0 elements
        if(p->m_data == value)
        {
            p->m_size--;
            if(p->m_size == 0)
            {
                p->m_prev->m_next = p->m_next;
                p->m_next->m_prev = p->m_prev;
                delete p;
                m_count--;
            }
            return 1;
        }
    return 0;
}

int Multiset::eraseAll(const ItemType& value)
{
    int numRemoved = 0;
    for (Node* p = m_head->m_next; p != m_head; p = p->m_next)    // Erase all instances of element
        if(p->m_data == value)
        {
            numRemoved = p->m_size;
            p->m_prev->m_next = p->m_next;
            p->m_next->m_prev = p->m_prev;
            delete p;
            m_count--;
            return numRemoved;
        }
    return numRemoved;                                              // Return # removed
}

bool Multiset::contains(const ItemType& value) const
{
    for (Node* p = m_head->m_next; p != m_head; p = p->m_next)        // If value in list, return true
        if(p->m_data == value)
            return true;
    return false;
}

int Multiset::count(const ItemType& value) const
{
    for (Node* p = m_head->m_next; p != m_head; p = p->m_next)        // Count number of instances
        if(p->m_data == value)
            return p->m_size;
    return 0;
}

int Multiset::get(int i, ItemType& value) const
{
    if (i < 0 || i >= m_count)
        return 0;
    Node* n = m_head->m_next;
    for(int j = 0; j < i; j++)
        n = n->m_next;
    value = n->m_data;                            // Set value + return number of times repeated
    return n->m_size;
}

void Multiset::swap(Multiset& other)
{
    int temp = m_count;                                         // Swap m_count members
    m_count = other.m_count;
    other.m_count = temp;
    
    Node* temp2 = m_head;                               // Swap m_head members also effectively swaps lists
    m_head = other.m_head;
    other.m_head = temp2;
}

void combine(const Multiset& ms1, const Multiset& ms2, Multiset& result)
{
    result = ms1;                                               // Start with ms1
    for (int i = 0; i < ms2.uniqueSize(); i++)
    {
        ItemType element;
        ms2.get(i, element);                                    // Add number of each element of ms2
        for (int j = 0; j < ms2.count(element); j++)
            result.insert(element);
    }
}

void subtract(const Multiset& ms1, const Multiset& ms2, Multiset& result)
{
    result = ms1;                                               // Start with ms1
    ItemType element;
    for (int i = 0; i < ms2.uniqueSize(); i++)
    {
        ms2.get(i, element);
        int count1 = result.count(element);
        int count2 = ms2.count(element);
        if(count1 > count2)                                     // result gets count1-count2 of element
            for (int j = 0; j < count2; j++)
                result.erase(element);
        else
            result.eraseAll(element);                           // Does not appear in result
    }
}