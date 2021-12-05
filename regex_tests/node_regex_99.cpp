// C++ program to demonstrate working of regex_search()
#include <iostream>
#include <regex>
#include<string.h>
#include <list>
using namespace std;

int main()
{
	// Target sequence
	string s = "R182_U98=NAND(U316,R182_U5)";

	// An object of regex for pattern to be searched
	regex r("([a-zA-Z0-9_]+)=([A-Z]+)\\((([a-zA-Z0-9_]+,?)+)\\)");
    

	// flag type for determining the matching behavior
	// here it is for matches on 'string' objects
	smatch m;
    
	// regex_search() for searching the regex pattern
	// 'r' in the string 's'. 'm' is flag for determining
	// matching behavior.
	regex_search(s, m, r);
	list<string> temp_fanin;
	string s1 = m[3];
	string s2 = m[2];

	stringstream s_stream(s1); //create string stream from the string
   while(s_stream.good()) {
      string substr;
      getline(s_stream, substr, ','); //get first string delimited by comma
      temp_fanin.push_back(substr);
   }
  for (auto p: temp_fanin)
         cout<< p <<" "<<endl;
    cout << "Gate Type :" << s2 << endl;
	// for each loop
	//for (auto x : m)
	//  cout << x << " "<<endl;

	return 0;
}

