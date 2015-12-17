//
//  StudentMultiset.h
//  HW1
//
//  Created by Connor Kenny on 1/17/15.
//  Copyright (c) 2015 Connor Kenny. All rights reserved.
//

#ifndef __HW1__StudentMultiset__
#define __HW1__StudentMultiset__

#include "Multiset.h"
#include <iostream>

class StudentMultiset
{
public:
    StudentMultiset();       // Create an empty student multiset.
    
    bool add(unsigned long id);
    // Add a student id to the StudentMultiset.  Return true if and only
    // if the id was actually added.
    
    int size() const;
    // Return the number of items in the StudentMultiset.  If an id was
    // added n times, it contributes n to the size.
    
    void print() const;
    // Print to cout every student id in the StudentMultiset one per line;
    // print as many lines for each id as it occurs in the StudentMultiset.
    
private:
    Multiset m_students;
};

#endif /* defined(__HW1__StudentMultiset__) */
