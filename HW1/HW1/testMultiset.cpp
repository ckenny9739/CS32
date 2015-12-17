//
//  main.cpp
//  HW1
//
//  Created by Connor Kenny on 1/14/15.
//  Copyright (c) 2015 Connor Kenny. All rights reserved.
//

#include "Multiset.h"
#include <iostream>
#include <cassert>
using namespace std;

int main()
{
    // unsigned long test cases
     
    Multiset ms;
    assert(ms.empty());                                                         // empty
    unsigned long x = 999;
    assert(ms.get(0, x) == 0  &&  x == 999);  // x unchanged by get failure     // get - empty
    assert( ! ms.contains(42));                                                 // contains
    ms.insert(42);                                                              // insert - 2x
    ms.insert(42);
    assert(ms.size() == 2  &&  ms.uniqueSize() == 1);                           // size + usize
    assert(ms.get(1, x) == 0  &&  x == 999);  // x unchanged by get failure     // get
    assert(ms.get(-5, x) == 0  &&  x == 999);  // x unchanged by get failure
    assert(ms.get(0, x) == 2  &&  x == 42);
    
    ms.insert(50);
    ms.insert(50);
    ms.erase(50);                                                               // erase
    assert(ms.size() == 3 && ms.uniqueSize() == 2);
    assert(ms.eraseAll(42) == 2);                                               // eraseAll
    assert(ms.size() == 1 && ms.uniqueSize() == 1);
    assert(ms.eraseAll(3) == 0);
    
    ms.insert(2);
    ms.insert(3);
    ms.insert(3);
    ms.insert(3);
    ms.insert(4);
    assert(ms.contains(4));                                                     // contains
    assert(ms.count(3) == 3);                                                   // count
 
    // std::string test cases
/*
    Multiset ms0;
    ms0.insert("fennel");
    ms0.insert("fennel");
    ms0.insert("fenugreek");
    ms0.insert("fennel");
    for (int k = 0; k < ms0.uniqueSize(); k++)
    {
        string x;
        int n = ms0.get(k, x);
        cout << x << " occurs " << n << " times." << endl;
    }
  
    Multiset ms;
    ms.insert("cinnamon");
    ms.insert("galangal");
    ms.insert("cinnamon");
    string s1;
    int n1 = ms.get(1, s1);
    assert((s1 == "cinnamon"  &&  n1 == 2)  ||  (s1 == "galangal"  &&  n1 == 1));
    string s2;
    int n2 = ms.get(1, s2);
    assert(s2 == s1  &&  n2 == n1);
    
    Multiset ms1;
    ms1.insert("cumin");
    ms1.insert("cumin");
    ms1.insert("cumin");
    ms1.insert("turmeric");
    Multiset ms2;
    ms2.insert("coriander");
    ms2.insert("cumin");
    ms2.insert("cardamom");
    ms1.swap(ms2);  // exchange contents of ms1 and ms2                             // swap
    assert(ms1.size() == 3  &&  ms1.count("coriander") == 1  &&
           ms1.count("cumin") == 1  &&  ms1.count("cardamom") == 1);
    assert(ms2.size() == 4  &&  ms2.count("cumin") == 3  &&
           ms2.count("turmeric") == 1);
    
    
    Multiset ms3;
    ms3.insert("cumin");
    assert(!ms3.contains(""));
    ms3.insert("nutmeg");
    ms3.insert("");
    ms3.insert("saffron");
    assert(ms3.contains(""));
    ms3.erase("cumin");
    assert(ms3.size() == 3  &&  ms3.contains("saffron")  &&  ms3.contains("nutmeg")  &&
           ms3.contains(""));
 
    #include "Multiset.h"
    #include <string>
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
    
    int main()
    {
        test();
        cout << "Passed all tests" << endl;
    }
 
    */
    
    cout << "Passed all tests" << endl;
}