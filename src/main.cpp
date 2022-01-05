#include <iostream>
#include <vector>

#include "Circuit.hpp"

using namespace std;

int main(int argc, char* argv[])
{
	if(argc < 2)
	{
		cout << "Need min 2 arguments" << endl;
	return -1;
	}


         
         cout << " Parsing " << argv[1] << endl; 
         string ckt_file(argv[1]); 
         Circuit ckt(ckt_file);
         int index_begin = 1;
         int index_end = ckt.get_ckt_size();
         	 
         for(int i = index_begin; i <= index_end; i++)
	 {
	 ckt.print_node_info(i);
	 }	 



	 return 0;
}


//int random_placement();

//int initial_HPWL_calculation();
