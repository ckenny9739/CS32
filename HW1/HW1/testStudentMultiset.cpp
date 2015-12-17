//
//  testStudentMultiset.cpp.
//  HW1
//
//  Created by Connor Kenny on 1/17/15.
//  Copyright (c) 2015 Connor Kenny. All rights reserved.
//

#include "StudentMultiset.h"
#include <iostream>
#include <cassert>
using namespace std;

int main()
{
    StudentMultiset s;
    s.print();
    cerr << "Test 1 complete \n";                               // Empty
    assert(s.add(42));
    assert(s.add(50));
    assert(s.size() == 2);
    assert(s.add(60));
    s.print();
    cerr << "Test 2 complete\n";                                // 1 of each element
    assert(s.add(42));
    assert(s.add(42));
    s.print();
    cerr << "Test 3 complete\n";                                // Duplicates
}
 
