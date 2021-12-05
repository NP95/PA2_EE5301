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
	string s2 = m[3];
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
// C++ program to demonstrate working of regex_search()
#include <iostream>
#include <regex>
#include<string.h>
#include <list>
#include <unordered_map>
#include <cmath>
using namespace std;

int main()
{
    int gate_area = 0;
    int extracted_gate_area = 0;
    double faninsize = 0.0;
    int scaling_factor;
    unordered_map<string, int> GateMap;
    GateMap.insert(make_pair("NOT", 1));
    GateMap.insert(make_pair("BUFF", 2));
    GateMap.insert(make_pair("NAND", 2));
    GateMap.insert(make_pair("NOR", 3));
    GateMap.insert(make_pair("XOR", 5));
    GateMap.insert(make_pair("AND", 3));
    GateMap.insert(make_pair("OR", 4));
    GateMap.insert(make_pair("XNOR",6));
    GateMap.insert(make_pair("INPUT",1));
    GateMap.insert(make_pair("OUTPUT",1));
    GateMap.insert(make_pair("DFF",9));

	// Target sequence
	string s = "R182_U98=DFF(U316,R182_U5,G5)";

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
// Calculate area
   faninsize = temp_fanin.size();
   scaling_factor = ceil(faninsize/2);
   cout << scaling_factor << " "<<endl;
   extracted_gate_area = GateMap.at(s2);
   cout << "2 Input gate area " << extracted_gate_area << endl;
   
   gate_area = scaling_factor*(GateMap.at(s2));
   cout << "Area of gate = " << gate_area << endl;
	return 0;
}

