#include <iostream>
#include <vector>

#include "Circuit.hpp"

using namespace std;

int main(int argc, char* argv[])
{
	if(argc < 2)
	{
		cout << "Need min 2 arguments" << endl;
	}
	return -1;

}

         string ckt_file(argv[1]); 

         Circuit ckt(ckt_file);

         for(const auto& node_id: node_ids)
	 {
	 ckt.print_node_info(node_id);
	 }	 



	 return 0;
}


//int random_placement();

//int initial_HPWL_calculation();
