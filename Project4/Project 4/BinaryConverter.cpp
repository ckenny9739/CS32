#include "provided.h"
#include "HashTable.h"
#include <string>
#include <vector>

using namespace std;

const size_t BITS_PER_UNSIGNED_SHORT = 16;

string convertNumberToBitString(unsigned short number);
bool convertBitStringToNumber(string bitString, unsigned short& number);


// Compute Hash Functions


unsigned int computeHash(std::string s)                     // computeHash for strings
{
    unsigned int total = 11;
    for (int i = 0; i < s.size(); i++)
        total += total*7 + s[i];
    return total;
}

unsigned int computeHash(unsigned short i)                  // computeHash for shorts
{
    return static_cast<unsigned int>(i);
}

// BinaryConverter Funcitons

string BinaryConverter::encode(const vector<unsigned short>& numbers)
{
    string bitString = "";
    for (int k = 0; k < numbers.size(); k++)
    {
        bitString += convertNumberToBitString(numbers[k]);          // convert each short in numbers to binary
    }
    
    for (int k = 0; k < bitString.size(); k++)                      // convert to tabs and spaces
    {
        switch (bitString[k])
        {
            case '1':
                bitString[k] = '\t';
                break;
            case '0':
                bitString[k] = ' ';
                break;
        }
    }
	return bitString;
}

bool BinaryConverter::decode(const string& bitString, vector<unsigned short>& numbers)
{
    if (bitString.size() % BITS_PER_UNSIGNED_SHORT != 0 || bitString.size() <= 0)       // if size not multiple of 16
        return false;
    
    numbers.clear();        // gets rid of old values
    
    string tempString = bitString;
    for (int k = 0; k < bitString.size(); k++)          // converts back into binary
    {
        switch (bitString[k])
        {
            case '\t':
                tempString[k] = '1';
                break;
            case ' ':
                tempString[k] = '0';
                break;
        }
    }
    
    string convert;
    for (int k = 0; k < tempString.size()+1; k++)         // +1 To print out the last number
    {
        if ((k == 0 && tempString.size() >=1) || tempString.substr(k).size() % BITS_PER_UNSIGNED_SHORT != 0)       // if tempString is correct size
            convert += tempString[k];
        else
        {
            unsigned short temp = 0;
            if (convertBitStringToNumber(convert, temp))        // convert binary to number
            {
                numbers.push_back(temp);
                convert = tempString[k];
            }
            else
                return false;
        }
    }
	return true;
}

string convertNumberToBitString(unsigned short number)
{
	string result(BITS_PER_UNSIGNED_SHORT, '0');
	for (size_t k = BITS_PER_UNSIGNED_SHORT; number != 0; number /= 2)
	{
		k--;
		if (number % 2 == 1)
			result[k] = '1';
	}
	return result;
}

bool convertBitStringToNumber(string bitString, unsigned short& number)
{
	if (bitString.size() != BITS_PER_UNSIGNED_SHORT)
		return false;
	number = 0;
	for (size_t k = 0; k < bitString.size(); k++)
	{
		number *= 2;
		if (bitString[k] == '1')
			number++;
		else if (bitString[k] != '0')
			return false;
	}
	return true;
}
