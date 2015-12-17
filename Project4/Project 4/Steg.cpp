#include "provided.h"
#include "HashTable.h"
#include <algorithm>
#include <iostream>
#include <string>
using namespace std;

bool Steg::hide(const string& hostIn, const string& msg, string& hostOut) 
{
    if (hostIn == "")
        return false;
    
    // Splits message
    
    string line = "";
    vector<string> lines;
    for (int k = 0; k < hostIn.size(); k++)
    {
        if (hostIn[k] != '\n' && k != hostIn.size()-1)      // not last character
            line += hostIn[k];
        else
        {
            if (hostIn[k] == '\r')      // skip newline that follows
                k++;
            lines.push_back(line);
            line = "";
        }
    }
    
    // Strips the message of whitespace
    
    vector<string> altered;
    
    for (int i = 0; i < lines.size(); i++)
    {
        bool foundSymbol = false;
        string newLine = "";
        for (int k = lines[i].size()-1; k >= 0; k--)
        {
            if ((lines[i][k] == ' ' || lines[i][k] == '\t') && !foundSymbol)        // strip the whitespace
                continue;
            foundSymbol = true;
            newLine += lines[i][k];
        }
        reverse(newLine.begin(), newLine.end());                                    // reverse because started from the end
        altered.push_back(newLine);
    }
    
    vector<unsigned short> shorts;
    Compressor::compress(msg, shorts);                                      // compress and encode
    string secret = BinaryConverter::encode(shorts);

    int N = lines.size();
    if (N <= 0)
        return false;
    int L = secret.size();
    int numStrings = L % N;
    int firstLength = L / N + 1;
    int secondLength = L / N;
    
    int length = 0;
    int count = 0;
    
    for (int k = 0; k < lines.size(); k++)
    {
        if (k < numStrings)                         // assign correct length
            length = firstLength;
        else
            length = secondLength;
        
        
        for (int i = 0; i < length; i++)
        {
            altered[k] += secret[count];            // add each letter from secret
            count++;
        }
        altered[k] += "\n";
    }
    
    for (int k = 0; k < altered.size(); k++)        // add each line to altered
        hostOut += altered[k];
    
	return true;
}



bool Steg::reveal(const string& hostIn, string& msg) 
{
    string line = "";
    vector<string> lines;
    for (int k = 0; k < hostIn.size(); k++)
    {
        if (hostIn[k] != '\n' && hostIn[k] != '\r')             // if not the end of the line
            line += hostIn[k];
        else
        {
            if (hostIn[k] == '\r')      // skip newline that follows
                k++;
            lines.push_back(line);
            line = "";
        }
    }
    
    string msgToBeDecoded = "";
    
    for (int k = 0; k < lines.size(); k++)
    {
        bool foundSymbol = false;
        string tempMsg = "";
        for (int i = lines[k].size()-1; i >= 0; i--)            // start from end, save the spaces and tabs
        {
            if (lines[k][i] == ' ' || lines[k][i] == '\t')
            {
                if (!foundSymbol)
                {
                    tempMsg += lines[k][i];
                    continue;
                }
            }
            break;
        }
        reverse(tempMsg.begin(), tempMsg.end());            // reverse because went backwards
        msgToBeDecoded += tempMsg;
    }
    
    vector<unsigned short> nums;
    if (BinaryConverter::decode(msgToBeDecoded, nums) && Compressor::decompress(nums, msg))     // decode and decompress
            return true;
	return false;
}
