//
//  main.cpp
//  Project2
//
//  Created by Connor Kenny on 1/21/15.
//  Copyright (c) 2015 Connor Kenny. All rights reserved.
//




 //STRING



#include "Multiset.h"
#include <iostream>
#include <cassert>
using namespace std;

void test()
{
    Multiset sms;
    assert(sms.insert("cumin"));
    assert(sms.insert("turmeric"));
    assert(sms.insert("cumin"));
    assert(sms.insert("coriander"));
    assert(sms.insert("cumin"));
    assert(sms.insert("turmeric"));
    assert(sms.size() == 6  &&  sms.uniqueSize() == 3);
    assert(sms.count("turmeric") == 2);
    assert(sms.count("cumin") == 3);
    assert(sms.count("coriander") == 1);
    assert(sms.count("cardamom") == 0);
}

void testCases()
{
    // default constructor
    Multiset ms;
    ItemType t;
    
    // For an empty multiset:
    assert(ms.size() == 0);         // test size
    assert(ms.empty());             // test empty
    assert(ms.erase("cumin") == 0); // nothing to erase
    assert(ms.get(0, t) == 0);      // nothing to get
    
    // For one item in multiset
    assert(ms.insert("cumin"));     // test insert
    assert(ms.size() == 1);         // test size
    assert(ms.uniqueSize() == 1);   // test uniqueSize
    assert(ms.get(0, t) == 1);      // test get
    assert(ms.count("cumin") == 1); // test count
    assert(ms.contains("cumin") == 1); // test contains
    
    // For one unique item, but repeated
    assert(ms.insert("cumin"));     // insert multiple
    assert(ms.size() == 2);         // test size with repeats
    assert(ms.uniqueSize() == 1);   // test uniqueSize with repeats
    assert(ms.get(0, t) == 2);      // test get
    assert(ms.count("cumin") == 2); // test count
    assert(ms.insert("cumin"));
    assert(ms.insert("cumin"));
    assert(ms.erase("cumin") == 1); // test erase
    assert(ms.eraseAll("cumin") == 3);  // test eraseAll
    assert(ms.eraseAll("cumin") == 0);  // nothing to erase
    assert(ms.size() == 0);         // check to see eraseAll worked
    
    // For multiple unique items
    assert(ms.insert("salt"));
    assert(ms.insert("pepper"));    // insert multiple unique items
    assert(ms.insert("salt"));
    assert(ms.insert("salt"));
    assert(ms.insert("cumin"));
    assert(ms.size() == 5);         // test size
    assert(ms.uniqueSize() == 3);   // test uniqueSize
    assert(ms.count("salt") == 3);  // test count
    cout << "ms.get() test" << endl;
    for (int i = 0; i < ms.uniqueSize(); i++)   // test get with multiple items
        cout << ms.get(i, t) << " - " << t << endl;
    
    // copy constructor
    Multiset ms1(ms);
    assert(&ms1 != &ms);                // Check to see if new Multiset
    assert(ms1.size() == 5);            // test size
    assert(ms1.uniqueSize() == 3);      // test uniqueSize
    assert(ms1.count("salt") == 3);     // test count
    cout << "ms1.get() test" << endl;
    for (int i = 0; i < ms1.uniqueSize(); i++)   // test get with multiple items
        cout << ms1.get(i, t) << " - " << t << endl;

    // assignment operator
    Multiset ms2 = ms;
    assert(&ms2 != &ms);                // Check to see if new Multiset
    assert(ms2.size() == 5);            // test size
    assert(ms2.uniqueSize() == 3);      // test uniqueSize
    assert(ms2.count("salt") == 3);     // test count
    cout << "ms2.get() test" << endl;
    for (int i = 0; i < ms2.uniqueSize(); i++)   // test get with multiple items
        cout << ms2.get(i, t) << " - " << t << endl;
    
    Multiset ms3;
    assert(ms3.insert("spice"));
    assert(ms3.insert("spice"));    // insert multiple unique items
    assert(ms3.insert("flavor"));
    assert(ms3.insert("yummy"));
    assert(ms3.insert("cumin"));
    assert(ms3.insert("cumin"));
    assert(ms3.insert("cumin"));
    assert(ms3.insert("salt"));
    
    // test swap
    ms3.swap(ms);
    assert(ms.size() == 8);             // test size
    assert(ms.uniqueSize() == 5);       // test uniqueSize
    assert(ms.count("spice") == 2);     // test count
    cout << "swap ms test" << endl;
    for (int i = 0; i < ms.uniqueSize(); i++)   // test get to print Multiset
        cout << ms.get(i, t) << " - " << t << endl;
    
    assert(ms3.size() == 5);            // test size
    assert(ms3.uniqueSize() == 3);      // test uniqueSize
    assert(ms3.count("salt") == 3);     // test count
    cout << "swap ms3 test" << endl;
    for (int i = 0; i < ms3.uniqueSize(); i++)   // test get to print Multiset
        cout << ms3.get(i, t) << " - " << t << endl;
    
    // test combine
    Multiset result1;
    combine(ms, ms3, result1);
    cout << "combine test" << endl;
    for (int i = 0; i < result1.uniqueSize(); i++)   // test get to print Multiset
        cout << result1.get(i, t) << " - " << t << endl;
    
    // test combine with aliasing
    combine(result1, ms, result1);
    cout << "combine test with aliasing" << endl;
    for (int i = 0; i < result1.uniqueSize(); i++)   // test get to print Multiset
        cout << result1.get(i, t) << " - " << t << endl;
    
    // test subtract
    Multiset result2;
    subtract(ms, ms3, result2);
    cout << "subtract test" << endl;
    for (int i = 0; i < result2.uniqueSize(); i++)   // test get to print Multiset
        cout << result2.get(i, t) << " - " << t << endl;
    
    // test subtract with aliasing
    Multiset ms4;
    assert(ms4.insert("spice"));
    subtract(result2, ms4, result2);
    cout << "subtract test with aliasing" << endl;
    for (int i = 0; i < result2.uniqueSize(); i++)   // test get to print Multiset
        cout << result2.get(i, t) << " - " << t << endl;

    cout << "Passed test cases" << endl;
}

int main()
{
    test();
    testCases();
    cout << "Passed all tests" << endl;
}



/*
    // UNSIGNED LONG

#include "Multiset.h"
#include <iostream>
#include <cassert>
using namespace std;

void test()
{
    Multiset ulms;
    assert(ulms.insert(20));
    assert(ulms.insert(10));
    assert(ulms.insert(20));
    assert(ulms.insert(30));
    assert(ulms.insert(20));
    assert(ulms.insert(10));
    assert(ulms.size() == 6  &&  ulms.uniqueSize() == 3);
    assert(ulms.count(10) == 2);
    assert(ulms.count(20) == 3);
    assert(ulms.count(30) == 1);
    assert(ulms.count(40) == 0);
}

int main()
{
    test();
    cout << "Passed all tests" << endl;
}

*/