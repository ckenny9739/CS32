// We have not yet produced the test driver main routine for you.

/*

#include "provided.h"
#include "http.h"
#include <iostream>
using namespace std;

int main()
{
    string originalPageText =
    "<html>\n"
    "<head>\n"
    " <title>My Pretend Web Page</title>\n"
    "</head>\n"
    "<body>\n"
    "<h2>My Pretend Web Page<h1>\n"
    "<p>\n"
    "I wish I were creative enough to have something interesting \n"
    "to say here.  I'm not, though, so this is a boring page.\n"
    "</p>\n"
    "<p>\n"
    "Oh, well.\n"
    "</p>\n"
    "</body>\n"
    "</html>\n"
    ;
    HTTP().set("http://boring.com", originalPageText);
    string plan =
    "Lefty and Mugsy enter the bank by the back door.\n"
    "Shorty waits out front in the getaway car.\n"
    "Don't let the teller trip the silent alarm.\n"
    ;
    string newPageText;
    if ( ! WebSteg::hideMessageInPage("http://boring.com", plan, newPageText))
    {
        cout << "Error hiding!" << endl;
        return 1;
    }
    HTTP().set("http://boring.com", newPageText);
    string msg;
    if ( ! WebSteg::revealMessageInPage("http://boring.com", msg))
    {
        cout << "Error revealing!" << endl;
        return 1;
    }
    if (msg != plan)
    {
        cout << "Message not recovered properly:\n" << msg;
        return 1;
    }
    cout << "Recovered text is what was hidden:\n" << msg;
}

*/




#include "provided.h"
//#include "substituteHashTable.h"                        // change this!!!!!
#include "HashTable.h"
#include "http.h"
#include <iostream>
#include <cassert>
using namespace std;


// Test HashTable

void argh()
{
    HashTable<string, int> hash(100, 200);
    hash.set("Carey", 43);
    hash.set("David", 97);
    hash.set("Timothy", 43, true);
    hash.set("Ivan", 28);
    hash.set("Sally", 22);
    hash.set("David", 55);
    hash.touch("Carey");
    // let’s discard the two least recently written items
    for (int k = 0; k < 5; k++)
    {
        string discardedName;
        int discardedAge;
        if (hash.discard(discardedName,discardedAge))
            cout << "Discarded " << discardedName
            << " who was " << discardedAge
            << " years old.\n";
        else
            cout << "There are no items to discard!\n";
    }
}

int main()
{
    cout << "Test HashTable" << endl;
    
    int value = -999;
    string key = "FAILED";
    
    HashTable<string, int> nameToAge(5, 10);
    // add a bunch of associations in this order
    nameToAge.set("Carey", 43);
    nameToAge.set("David", 97);
    nameToAge.set("Lily", 18);
    nameToAge.set("Sally", 22);
    nameToAge.set("David", 55);
    nameToAge.set("aDvid", 10);
    nameToAge.set("avDid", 15);

    assert(!nameToAge.isFull());
    
    assert(nameToAge.get("Lily", value) && value == 18);

    assert(nameToAge.get("David", value) && value == 55);
    
    assert(nameToAge.get("aDvid", value) && value == 10);
    
    assert(nameToAge.get("avDid", value) && value == 15);           // test collisions
    
    assert(nameToAge.touch("Lily"));
    
    assert(nameToAge.discard(key, value) && key == "Carey");
    
    HashTable<string, int> test(5, 10);
    test.set("TEST", 99);
    assert(test.discard(key, value) && key == "TEST");      // test one item
    
    assert(!test.discard(key, value));          // test empty
    
    argh();
    
    
    // Test BinaryConverter
    cout << endl << "Test BinaryConverter" << endl;
    
    
    vector<unsigned short> v;
    v.push_back(104);
    v.push_back(105);
    // We use the :: operator below because encode() is a static
    // member function. You don’t create a BinaryConverter object
    // to use encode(); instead, you call it using the class name.
    string hiddenMessage = BinaryConverter::encode(v);
    cout << hiddenMessage << endl; // prints tabs and spaces
    
    
    
    vector<unsigned short> v2;
    // Using the -/_ representation in this comment, the string
    // below contains _______________-_____________-_-
    if (BinaryConverter::decode(hiddenMessage, v2))
    {
        cout << "The vector has " << v2.size() << " numbers:";
        for (int k = 0; k != v2.size(); k++)
            cout << ' ' << v2[k];
        cout << endl;
    }
    else
        cout << "The string has a bad character or a bad length.";
    
    // The vector has 2 numbers: 1 5
    
    
    
    
    // Test Compressor
    
    
    cout << endl << "Compressor Test" << endl;
    
    string compressTest = "cat in the hat sat on the bat";
    vector<unsigned short> compressed;
    Compressor::compress(compressTest, compressed);
    
    for (int i = 0; i < compressed.size(); i++)
        cout << compressed[i] << " ";
    
    //{65, 65, 256, 65, 257, 65, 66, 517}
    
    string output;
    Compressor::decompress(compressed, output);
    
    cout << endl << output << endl << endl;
    
    // "AAAAAAAAAB"
    
    
    // Test Convert + Compress
    
    cout << "Test Convert + Compress" << endl;
    
    string hiddenMessage2 = "AABBAABBAABBAABB";
    cout << hiddenMessage2 << endl;
    
    vector<unsigned short> compressed2;
    Compressor::compress(hiddenMessage2, compressed2);
    
    for (int i = 0; i < compressed2.size(); i++)
        cout << compressed2[i] << " ";
    cout << endl;
    
    
    
    string hiddenMessage3 = BinaryConverter::encode(compressed2);
    cout << hiddenMessage3 << endl; // prints tabs and spaces
    
    
    vector<unsigned short> v4;
    if (BinaryConverter::decode(hiddenMessage3, v4))
    {
        cout << "The vector has " << v4.size() << " numbers:";
        for (int k = 0; k != v4.size(); k++)
            cout << ' ' << v4[k];
        cout << endl;
    }
    else
        cout << "The string has a bad character or a bad length.";

    
    string output2;
    Compressor::decompress(v4, output2);
    
    cout << output2 << endl << endl;



    

    // Test Steg
    
    cout << "Test Steg" << endl;
    
    string hostIn = "<html>   \nQ \tQQ \t \nBBB\t\t\t   \n\nGG \nBBB \n\t\nDDD\nEEE </html>   ";
    string message = "hello connor";
    cout << message << endl;
    string hostOut = "";
    assert(Steg::hide(hostIn, message, hostOut));
    
    cout << hostOut << endl;
    
    string finalMsg;
    assert(Steg::reveal(hostOut, finalMsg));
    cout << finalMsg << endl;
    
    
    
    
    // Test WebSteg
    
    
	cout << endl << "Test WebSteg" << endl;

	string text;
	if (! WebSteg::hideMessageInPage("https://www.google.com/", "Hello there!", text))
        cout << "Error hiding!" << endl;
    
    HTTP().set("pretendWebPage", text);
    
	string msg;
	if (! WebSteg::revealMessageInPage("pretendWebPage", msg))
		cout << "Error revealing!" << endl;
  
    //cout << text << endl;

    cout << "Here is message: " << msg << endl;
    
    
    
    
    cout << "PASSED ALL TESTS!!!!!" << endl;

}


