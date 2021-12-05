// C++ program to demonstrate working of regex_search()
#include <iostream>
#include <regex>
#include<string.h>
using namespace std;

int main()
{
	// Target sequence
	string s = "INPUT(START)";

	// An object of regex for pattern to be searched
	regex r("INPUT\\(([a-zA-Z0-9_]+)\\)");

	// flag type for determining the matching behavior
	// here it is for matches on 'string' objects
	smatch m;

	// regex_search() for searching the regex pattern
	// 'r' in the string 's'. 'm' is flag for determining
	// matching behavior.
	regex_search(s, m, r);

	// for each loop
	for (auto x : m)
		cout << x << " "<<endl;

	return 0;
}

