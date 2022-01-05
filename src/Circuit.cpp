#include <iostream>
#include <fstream>
#include <sstream>
#include <regex>
#include <algorithm>
#include <cstdlib>
#include <unordered_map>
#include "Circuit.hpp"
#include <cmath>
#define NODE_BUF_SIZE 1000

using namespace std;
unordered_map<string, int> GateMap;
unordered_map<string,vector<string>> FanOutMap;
unordered_map<string,int> ConstructionIDMap;

//int initChipWidth;
//int initChipHeight;
//int SumGatesArea = 0;



Circuit::Circuit(const std::string& ckt_file)
{
    bool file_type_is_ITC99 = false;
    bool file_type_is_ISC85 = false;
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
     cout << "Parsing circuit file: " << ckt_file << endl;
    ifstream ifs(ckt_file.c_str());
    if (!ifs.is_open()) 
    {
        cout << "Error opening file " << ckt_file << endl;
        return;
    }
    int Node_Construction_ID = 1; //Use this for ITC99
    int construction_id; // Use this for ISC85
    nodes_.reserve(NODE_BUF_SIZE);
    cout << " Constructing regexes" << endl;
    const regex input_pad_regex_isc85("INPUT\\((\\d+)\\)");
    cout << "Failed at first" << endl;
    const regex output_pad_regex_isc85("OUTPUT\\((\\d+)\\)");
    const regex node_regex_isc85("(\\d+)=([a-zA-Z0-9_]+)\\(([0-9,\\s]+)\\)");
    const regex input_pad_regex_itc99("INPUT\\(([a-zA-Z0-9_]+)\\)");
    cout << "Failed at 5th" << endl;
    const regex output_pad_regex_itc99("OUTPUT\\(([a-zA-Z0-9_]+)\\)");
    cout << "Failed at 7th" << endl;
    const regex node_regex_itc99("([a-zA-Z0-9_]+)=([A-Z]+)\\((([a-zA-Z0-9_]+,?)+)\\)");
    cout << "Failed at 8th" << endl;  
    const regex isc85_detect("# c[0-9]*");

    cout << "Failed at 8th" << endl;  
    const regex itc99_detect("# edf2bench v0.8");
    cout << "Constructed regexes " << endl;
    smatch filetype;
    smatch output_pad_regex_match;
    smatch input_pad_regex_match;
    smatch node_regexMatch;
    //For node_regexMatch smatch
    // smatch[3] is the gate type
    // smatch[2] is the fan_in list from the file, further processing is needed for extracting the 
    //A flag to process first line after the first comment all comments are ignored
    int file_type_check_status = 0;
    while (ifs.good()) 
    {
        string line;
        getline(ifs, line);
        cout << "Entered the file" << endl;
	if(!file_type_check_status)
	{ 
               if(regex_match(line,filetype,itc99_detect))
	       {
	       file_type_is_ITC99 = true;
		  cout << "Yo this is a ITC99" << endl;
	       file_type_check_status++;
	       }

               if(regex_match(line,filetype,isc85_detect))
	       {
	       file_type_is_ISC85 = true;
		  cout << "Yo this is a ISC85" << endl;
	       file_type_check_status++;
	       }
	}
        cout << "Figured out the file type" <<endl; 
        // We only need to parse before the comment
        string code_line = line.substr(0, line.find("#"));
        NodeID node_id; // What is the point of this? 
	//If the file type is ISC85 do this
	if(file_type_is_ISC85)
          {
        // Remove all whitespace to make regex simpler
        code_line.erase(remove_if(code_line.begin(), code_line.end(), ::isspace), code_line.end());
        
        if (code_line.length() <= 0)
            continue;

        // cout << code_line << endl;

        // Find INPUT(<nodeNumber>)
        regex_match(code_line, input_pad_regex_match, input_pad_regex_isc85);
        if (input_pad_regex_match.size() > 0) {
		//DO stuff with unordered_maps_here
		//Validation linke https://onlinegdb.com/VIwl8c6Pg
	     node_id = input_pad_regex_match[1];
             construction_id = stoi(node_id); //Stoi is only for a ISC85
	    ConstructionIDMap.insert(make_pair(node_id,construction_id));
            allocate_for_node_id(construction_id);
            nodes_[construction_id]->set_node_id(node_id);
            nodes_[construction_id]->set_construction_id(construction_id);
	    nodes_[construction_id]->set_gate_area(1);
	    nodes_[construction_id]->set_gate_type("INPUT");
	    //Input nodes have a fanin list
	    SumGatesArea = SumGatesArea + 1;
            continue;
        }

        // Find OUTPUT(<nodeNumber>)
        regex_match(code_line, output_pad_regex_match, output_pad_regex_isc85);
        if (output_pad_regex_match.size() > 0) {
	     //  https://onlinegdb.com/SF37Qaukm
	     node_id = input_pad_regex_match[1];
              construction_id = stoi(node_id);
            ConstructionIDMap.insert(make_pair(node_id,construction_id));	    
            allocate_for_node_id(construction_id);
            nodes_[construction_id]->set_node_id(node_id);
	    nodes_[construction_id]->set_construction_id(construction_id);
	    nodes_[construction_id]->set_gate_area(1);
	    nodes_[construction_id]->set_gate_type("OUTPUT");
	    SumGatesArea = SumGatesArea + 1;
	    //Output nodes have a fanout, not a fanin
            continue;
        }
         list<string> temp_fanin;    
	 int gate_area = 0;
         int extracted_gate_area = 0;
         double faninsize = 0.0;
         int scaling_factor;

        // Find <nodeNumber>=(<nodeNumber...>)
        regex_match(code_line, node_regexMatch, node_regex_isc85);
        if (node_regexMatch.size() > 0) 
	{
		string gate_type;
	if(!(ConstructionIDMap.count(node_regexMatch[1])))
	     {
	        node_id = node_regexMatch[1];
              construction_id = stoi(node_id);
	      //Check for existence and then insert
	      //If exists then process as it is,as the only field modified would be the fanout field

            allocate_for_node_id(construction_id);
            nodes_[construction_id]->set_node_id(node_regexMatch[1]);
	    nodes_[construction_id]->set_construction_id(construction_id);
             ConstructionIDMap.insert(make_pair(node_id,construction_id));
	     }
            gate_type = node_regexMatch[2];
            transform(gate_type.begin(), gate_type.end(), gate_type.begin(), ::toupper);
            nodes_[construction_id]->set_gate_type(gate_type);

            stringstream node_ids_str(node_regexMatch[3]);
    
	    while(node_ids_str.good()) 
	    {
                  string fanin_elem;
                  getline(node_ids_str, fanin_elem, ','); //get first string delimited by comma
                  temp_fanin.push_back(fanin_elem); // We probably want to allocate these nodes as a part of the graph here itself using the ConstructionMap
                  nodes_[construction_id]->add_to_fanin_list(fanin_elem);
		  if(!ConstructionIDMap.count(fanin_elem))
		  {
			int fan_out_construction_id = stoi(fanin_elem);
		        allocate_for_node_id(fan_out_construction_id);
			nodes_[fan_out_construction_id]->set_node_id(fanin_elem);
		nodes_[fan_out_construction_id]->set_construction_id(fan_out_construction_id);
		ConstructionIDMap.insert(make_pair(fanin_elem,fan_out_construction_id));	
		  }
             }

            vector<string> current;
	    
	    for (auto const& i : temp_fanin) 
	    {
                   
		  //Correct usage of umap.insert
		  current.push_back(node_id);
		  FanOutMap.insert(make_pair(i,current));
             }


          faninsize = temp_fanin.size();
          scaling_factor = ceil(faninsize/2);
         // cout << scaling_factor << " "<<endl;
          extracted_gate_area = GateMap.at(gate_type);
         // cout << "2 Input gate area " << extracted_gate_area << endl;

          gate_area = scaling_factor*(GateMap.at(gate_type));
	  nodes_[construction_id]->set_gate_area(gate_area);
    //      cout << "Area of gate = " << gate_area << endl;
          SumGatesArea = SumGatesArea + gate_area;
	    // Rewrite this part to get 
          //  NodeID input_node_id;
           // char delim;
          // while (node_ids_str >> input_node_id) {
            //    nodes_[node_id]->add_to_fanin_list(input_node_id);
              //  node_ids_str >> delim;


          //1. First get the fanin list_size
	  //2. Calculate the gate_area by mapping using the map and k/2*(map_area)
	  //3. Add to SumOfGatesArea

            }
                 
            continue;
        }

	      

	else if(file_type_is_ITC99)
	{
              cout<< " Yo you are a ITC99 file" << endl;
        // Remove all whitespace to make regex simpler
        code_line.erase(remove_if(code_line.begin(), code_line.end(), ::isspace), code_line.end());

        if (code_line.length() <= 0)
            continue;

         cout << code_line << endl;

        // Find INPUT(<nodeNumber>)
        regex_match(code_line, input_pad_regex_match, input_pad_regex_itc99);
        if (input_pad_regex_match.size() > 0) {
            NodeID node_id = (input_pad_regex_match[1]);
            Node_Construction_ID++;
            allocate_for_node_id(Node_Construction_ID);
            nodes_[Node_Construction_ID]->set_node_id(node_id);
            nodes_[Node_Construction_ID]->set_construction_id(Node_Construction_ID);
            ConstructionIDMap.insert(make_pair(node_id,Node_Construction_ID));
	    nodes_[Node_Construction_ID]->set_gate_area(1);
	    nodes_[Node_Construction_ID]->set_gate_type("INPUT");
	    SumGatesArea = SumGatesArea + 1;
            continue;
        }

        // Find OUTPUT(<nodeNumber>)
        regex_match(code_line, output_pad_regex_match, output_pad_regex_itc99);
        if (output_pad_regex_match.size() > 0) {
           NodeID node_id = output_pad_regex_match[1];
           Node_Construction_ID++;
           allocate_for_node_id(Node_Construction_ID);
           nodes_[Node_Construction_ID]->set_node_id(node_id);
	   nodes_[Node_Construction_ID]->set_construction_id(Node_Construction_ID);
           ConstructionIDMap.insert(make_pair(node_id,Node_Construction_ID));
	   nodes_[Node_Construction_ID]->set_gate_area(1);
	   nodes_[Node_Construction_ID]->set_gate_type("OUTPUT");
	   SumGatesArea = SumGatesArea + 1;
           // nodes_[node_id]->set_node_id(node_id);
           // nodes_[node_id]->set_output_pad(true);
            continue;
        }


	  list<string> temp_fanin;
          int gate_area = 0;
          int extracted_gate_area = 0;
          double faninsize = 0.0;
          int scaling_factor;

        // Find <nodeNumber>=(<nodeNumber...>)
        regex_match(code_line, node_regexMatch, node_regex_itc99);
        if (node_regexMatch.size() > 0) {
                         string gate_type;
			 NodeID node_id = node_regexMatch[1];
        if(!(ConstructionIDMap.count(node_id)))
             {
	     Node_Construction_ID++;
            //  construction_id = stoi(node_regexMatch[1]);
              //Check for existence and then insert
              //If exists then process as it is,as the only field modified would be the fanout field
            
            allocate_for_node_id(Node_Construction_ID);
            nodes_[Node_Construction_ID]->set_node_id(node_id);
            nodes_[Node_Construction_ID]->set_construction_id(Node_Construction_ID);
             ConstructionIDMap.insert(make_pair(node_id,Node_Construction_ID));
             }
            gate_type = node_regexMatch[2];
            transform(gate_type.begin(), gate_type.end(), gate_type.begin(), ::toupper);
            nodes_[Node_Construction_ID]->set_gate_type(gate_type);

            stringstream node_ids_str(node_regexMatch[3]);
    
            while(node_ids_str.good()) 
            {
                  string fanin_elem;
                  getline(node_ids_str, fanin_elem, ','); //get first string delimited by comma
                  temp_fanin.push_back(fanin_elem); // We probably want to allocate these nodes as a part of the graph here itself using the ConstructionMap
                  nodes_[construction_id]->add_to_fanin_list(fanin_elem);
                  if(!ConstructionIDMap.count(fanin_elem))
                  {
			  Node_Construction_ID++;
                     //   int fan_out_construction_id = stoi(fanin_elem);
                        allocate_for_node_id(Node_Construction_ID);
                        nodes_[Node_Construction_ID]->set_node_id(fanin_elem);
			nodes_[Node_Construction_ID]->set_construction_id(Node_Construction_ID);
                        ConstructionIDMap.insert(make_pair(fanin_elem,Node_Construction_ID)); 
                  }
             }

            vector<string> current;
            for (auto const& i : temp_fanin) 
            {
    
                  //Correct usage of umap.insert
		  current.push_back(node_id);
		  FanOutMap.insert(make_pair(i,current));
             }


          faninsize = temp_fanin.size();
          scaling_factor = ceil(faninsize/2);
          extracted_gate_area = GateMap.at(gate_type);

          gate_area = scaling_factor*(GateMap.at(gate_type));
          nodes_[construction_id]->set_gate_area(gate_area);
          SumGatesArea = SumGatesArea + gate_area;
            }
            continue;
    cout << "Gate Area Sum:" << SumGatesArea<< endl;  
        }


              else
	      {
	      cout << "Input a valid bench file" << endl;
	      }
initChipWidth = ceil(sqrt(SumGatesArea));
initChipHeight = ceil(SumGatesArea/initChipWidth);

//Initialize the chip here
//vector<vector<gate_location>> Chip;
//Chip.resize(initChipHeight);
//for (int i = 0; i < initChipHeight; ++i)
// {
//    Chip[i].resize(initChipWidth);
 //}


    } 

    } 

Circuit::~Circuit() {
    for(const auto& node_ptr: nodes_) {
        if (node_ptr != nullptr)
            delete node_ptr;
    }
}

void Circuit::allocate_for_node_id(const int& node_id) {
    // cout << "Checking Node ID: " << node_id << endl;
    static int size_incr = 1;

    // -1 here due the nodeID being used directly as index so we need one extra in size
    // For example node ID 2000, would be stored in index 2000
    // which means the vector will be of size 2001
    if ((int) nodes_.capacity() - 1 < node_id) {
        // cout << "Allocating for Node ID: " << node_id << endl;
        // Increase the capacity increments of NODE_BUF_SIZE * size_incr
        nodes_.reserve(((node_id / NODE_BUF_SIZE) + size_incr) * NODE_BUF_SIZE);
        // cout << "Done. New capacity = " << nodes_.capacity() << endl;
    }

    if ((int) nodes_.size() - 1 < node_id) {
        nodes_.resize(node_id + 1, nullptr);
    }

    if (nodes_[node_id] == nullptr) {
        nodes_[node_id] = new CircuitNode();
    }
}

void Circuit::print_node_info(const int& node_id) {
    if (node_id >= (int) nodes_.size()) {
        cout << "Invalid Node ID: " << node_id << endl;
        cout << "We only have allocated for " << nodes_.size() << " nodes\n";
        return;
    }

    // Check the stored node ID. Just a sanity check for valid node or not
    // It will be -1 if we had not parsed it from file
    if (nodes_[node_id] == nullptr) {
        cout << "Invalid Node ID: " << node_id << endl;
        cout << "Did not parse this Node ID from file\n";
        return;
    }
      // This has to be modified to print all the fields
        cout <<"Node ID: "<<nodes_[node_id]->get_node_id() << " " << endl;
        cout <<"X Coordinate: "<<nodes_[node_id]->get_x_coordinate() << " " << endl;
        cout <<"Y Coordinate: "<<nodes_[node_id]->get_y_coordinate() << " " << endl;
        cout <<"Area: "<<nodes_[node_id]->get_area() << " " << endl;
        cout <<"Construction ID: "<<nodes_[node_id]->get_construction_id() << " " << endl;
        cout <<"Gate Type: "<<nodes_[node_id]->get_node_id() << " " << endl;
       // cout <<"Fanout List: "<<nodes_[node_id]->get_fanout_list() << " " << endl;
       // cout <<"Fanin List: "<<nodes_[node_id]->get_fanin_list() << " " << endl;
            
            }


int Circuit::get_ckt_size()
{
	return nodes_.size(); 
}
    
//    cout << endl;

/*
void Circuit::test() {
    
    gate_db_.test();
    for(const auto& node: nodes_) {
        if (node.get_node_id() != -1) {
            if (node.is_input_pad()) {
                cout << "INPUT <" << node.get_node_id() << ">";
            }
            else if (node.is_output_pad()) {
                cout << "OUTPUT <" << node.get_node_id() << ">";
            } else {
                cout << node.get_gate_type() << " <" << node.get_node_id() << ">";
                if (node.get_gate_info() != nullptr) {
                    cout << "(" << node.get_gate_info()->output_slew[2][1] << ")";
                }
            }

            if (node.get_fanin_list().size() > 0) {
                cout << " -- ";
                for(const auto& adj_node: node.get_fanin_list()) {
                    cout << adj_node << " ";
                }
            }
            cout << endl;
        }
    }
    

    srand(0);
    for (int i = 0; i < 10;)
    {
        size_t idx = rand() % nodes_.size();
        NodeID node_id = nodes_[idx]->get_node_id();
        if (node_id != -1) {
            cout << node_id << " ";
            i++;
        }
    }
    
    // for(const auto& node: nodes_) {
    //     NodeID node_id = node.get_node_id();
    //     if (node_id != -1) {
    //         cout << node_id << " ";
    //     }
    // }
    cout << endl;
}


//void RandomPlacement()
//{};

//int initial_HPWL()

//Initial function_list
//random_placement( pointers to list, chip datastructure)
//Initial HPWL
//
//Random Placement
//Swap 2 nodes
//Legal overlap
//Insert(empty space)
//Calculate k
// Wirite the annealing engine*/
