#include "provided.h"
#include "HashTable.h"
#include <string>
#include <vector>
using namespace std;

const double MAX_LOAD_FACTOR = .5;

void Compressor::compress(const string& s, vector<unsigned short>& numbers)
{
    numbers.clear();
    
    int N = s.size();
    int C = (N/2 + 512 < 16384) ? N/2 + 512 : 16384;
    
    HashTable<string, unsigned short> H(C / MAX_LOAD_FACTOR, C);      // C*(1/MAX_LOAD_FACTOR) gives correct number of buckets
    
    for (int j = 0; j < 256; j++)               // assign each character a spot in the Hash Table
    {
        string temp = "";
        temp += static_cast<char>(j);
        H.set(temp, j, true);
    }
    
    int nextFreeID = 256;
    string runSoFar = "";
    vector<unsigned short> V;
    
    unsigned short x;
    
    for (int k = 0; k < s.size(); k++)          // for each character in s
    {
        string c = "";
        c += s[k];
        string expandedRun = runSoFar + c;
        if (H.get(expandedRun, x))              // if found expandedRun, make runSoFar = expandedRun
        {
            runSoFar = expandedRun;
            continue;
        }
        H.get(runSoFar, x);
        V.push_back(x);
        H.touch(runSoFar);
        runSoFar = "";
        unsigned short cv;
        H.get(c, cv);
        V.push_back(cv);          // push x and cv onto V
        
        if (!H.isFull())                        // add an association
        {
            H.set(expandedRun, nextFreeID);
            nextFreeID++;
        }
        else
        {
            unsigned short ID;                  // discard last touched item
            string key;
            H.discard(key, ID);
            H.set(expandedRun, ID);
        }
    }
    if (runSoFar != "")                         // push any extra numbers
    {
        H.get(runSoFar, x);
        V.push_back(x);
    }
    V.push_back(C);                       // push_back the capacity
    numbers = V;                            // assign numbers to V
}

bool Compressor::decompress(const vector<unsigned short>& numbers, string& s)
{
    if (numbers.empty())
        return false;
    
    if (numbers.size() == 1)            // just capacity
    {
        s = "";
        return true;
    }
    
    unsigned short C = numbers[numbers.size()-1];
    HashTable<unsigned short, string> H(C / MAX_LOAD_FACTOR, C);
    
    for (int j = 0; j < 256; j++)               // assign each character a spot in the Hash Table
    {
        string temp = "";
        temp += static_cast<char>(j);
        H.set(j, temp, true);
    }
    
    int nextFreeID = 256;
    string runSoFar = "";
    string output = "";
    
    for (unsigned short us = 0; us < numbers.size()-1; us++)
    {
        string append = "";
        if (numbers[us] < 256)              // if is a single character
        {
            string expandedRun;                 
            H.get(numbers[us], append);
            output += append;
            if (runSoFar == "")
            {
                runSoFar += append;
                continue;
            }
            else
            {
                expandedRun = runSoFar + append;
            }
            
            if (!H.isFull())                    // assign new association
            {
                H.set(nextFreeID, expandedRun);
                nextFreeID++;
            }
            else
            {
                unsigned short ID;
                string key;
                H.discard(ID, key);
                H.set(ID, expandedRun);
            }
            runSoFar = "";
        }
        else
        {
            string S;
            if (!H.get(numbers[us], S))         // if can't find us, return false
                return false;
            else
            {
                H.touch(numbers[us]);           // touch so it goes to top of recentlyTouched list
                output += S;
                runSoFar = S;
            }
        }
    }
    s = output;
	return true;
}
