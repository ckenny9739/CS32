//
//  Multiset.cpp
//  HW1
//
//  Created by Connor Kenny on 1/14/15.
//  Copyright (c) 2015 Connor Kenny. All rights reserved.
//

#include "Multiset.h"

Multiset::Multiset()
{
    m_count = 0;
}

bool Multiset::empty() const
{
    return !m_count;
}

int Multiset::size() const
{
    int total = 0;
    for (int k = 0; k < m_count; k++)
        total += m_array[k].count;
    return total;
}

int Multiset::uniqueSize() const
{
    return m_count;
}

bool Multiset::insert(const ItemType& value)
{
    if(m_count >= DEFAULT_MAX_ITEMS)
        return false;
    for (int i = 0; i < m_count; i++)                   // Count number of repeated terms
        if (m_array[i].obj == value)
        {
            m_array[i].count++;
            return true;
        }
    m_array[m_count].obj = value;                       // If not already in array
    m_array[m_count].count = 1;
    m_count++;
    return true;
}

int Multiset::erase(const ItemType& value)
{
    for (int i = 0; i < m_count; i++)
        if (m_array[i].obj == value)                            // Erase one object
        {
            m_array[i].count--;
            if (m_array[i].count == 0)
            {
                m_array[i] = m_array[m_count-1];
                m_count--;
            }
            return 1;
        }
    return 0;
}

int Multiset::eraseAll(const ItemType& value)
{
    int numRemoved = 0;
    for (int k = 0; k < m_count; k++)
        if(m_array[k].obj == value)
        {
            numRemoved = m_array[k].count;
            m_array[k].count = 0;
            m_array[k] = m_array[m_count-1];                    // Replace with last object
            m_count--;
            return numRemoved;
        }
    return numRemoved;
}

bool Multiset::contains(const ItemType& value) const
{
    for (int k = 0; k < m_count; k++)                           // Checks for one instance of value
        if (m_array[k].obj == value)
            return true;
    return false;
}

int Multiset::count(const ItemType& value) const
{
    for (int k = 0; k < m_count; k++)
        if (m_array[k].obj == value)                            // Counts number of instances
            return m_array[k].count;
    return 0;
}

int Multiset::get(int i, ItemType& value) const
{
    if (i < 0 || i >= uniqueSize())
        return 0;
    value = m_array[i].obj;                             // Set value + return number of times repeated
    return m_array[i].count;
}

void Multiset::swap(Multiset& other)
{
    int temp = m_count;                                                     // Swap m_count members
    m_count = other.m_count;
    other.m_count = temp;
    
    int lCount;
    m_count > other.m_count? lCount = m_count : lCount = other.m_count;     // Swap m_array members
    for (int k = 0; k < lCount; k++)
    {
        Arr temp = m_array[k];
        m_array[k] = other.m_array[k];
        other.m_array[k] = temp;
    }
}