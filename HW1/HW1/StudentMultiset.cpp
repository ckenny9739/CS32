//
//  StudentMultiset.cpp
//  HW1
//
//  Created by Connor Kenny on 1/17/15.
//  Copyright (c) 2015 Connor Kenny. All rights reserved.
//

#include "StudentMultiset.h"
using namespace std;

StudentMultiset::StudentMultiset()
{}

bool StudentMultiset::add(unsigned long id)
{
    return m_students.insert(id);
}

int StudentMultiset::size() const
{
    return m_students.size();
}

void StudentMultiset::print() const
{
    ItemType t;
    for (int i = 0; i < m_students.uniqueSize(); i++)
        for (int k = 0; k < m_students.get(i, t); k++)
            cout << t << endl;
}
