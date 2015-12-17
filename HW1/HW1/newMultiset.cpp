//
//  newMultiset.cpp
//  HW1
//
//  Created by Connor Kenny on 1/18/15.
//  Copyright (c) 2015 Connor Kenny. All rights reserved.
//

#include "newMultiset.h"
using namespace std;

Multiset::Multiset(int n)
{
    if (n < 0)
    {
        cout << "Error - n must be nonnegative" << endl;
        exit(1);
    }
    m_array = new Arr[n];
    m_len = n;
    m_count = 0;
}

Multiset::~Multiset()
{
    delete [] m_array;
}

Multiset::Multiset(const Multiset& other)
{
    m_count = other.m_count;
    m_len = other.m_len;
    m_array = new Arr[m_len];
    for (int i = 0; i < m_len; i++)
        m_array[i] = other.m_array[i];
}

Multiset& Multiset::operator=(const Multiset& rhs)
{
    Multiset temp(rhs);
    swap(temp);
    return *this;
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
    for (int i = 0; i < m_count; i++)                   // Count number of repeated terms
        if (m_array[i].obj == value)
        {
            m_array[i].count++;
            return true;
        }
        if (m_count >= m_len)                           // Check to see if space left
            return false;
        m_array[m_count].obj = value;                   // If not already in array
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
    return 0;                                                   // Not found
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
        return false;
    value = m_array[i].obj;                             // Set value + return number of times repeated
    return m_array[i].count;
}

void Multiset::swap(Multiset& other)
{
    int temp = m_count;                                         // Swap m_count members
    m_count = other.m_count;
    other.m_count = temp;
    
    int temp2 = m_len;                                          // Swap m_len members
    m_len = other.m_len;
    other.m_len = temp2;
    
    Arr* temp3 = other.m_array;                                 // Swap m_array members
    other.m_array = m_array;
    m_array = temp3;
    temp3 = nullptr;                                            // Safety check
}

