#include "provided.h"
#include "http.h"
#include <string>
using namespace std;

bool WebSteg::hideMessageInPage(const string& url, const string& msg, string& host)
{
    string page;
    if (HTTP().get(url, page) && Steg::hide(page, msg, host))           // get URL and hide message
        return true;
	return false;
}

bool WebSteg::revealMessageInPage(const string& url, string& msg)
{
    string page;
    if (HTTP().get(url, page) && Steg::reveal(page, msg))               // get URL and reveal message
        return true;
    return false;
}
