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
int initChipWidth = 0;
int initChipHeight = 0;
int SumGatesArea = 0;

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

Circuit::Circuit(const std::string& ckt_file):
        // Call the constructor of GateDatabase
    bool file_type_is_ITC99 = false;
    bool file_type_is_ISC85 = false;
    // cout << "Parsing circuit file: " << ckt_file << endl;
    ifstream ifs(ckt_file.c_str());
    if (!ifs.is_open()) {
        cout << "Error opening file " << ckt_file << endl;
        return;
    }
    int Node_Construction_ID = 0;
    nodes_.reserve(NODE_BUF_SIZE);

    const regex input_pad_regex_isc85("INPUT\\((\\d+)\\)");
    const regex output_pad_regex_isc85("OUTPUT\\((\\d+)\\)");
    const regex node_regex_isc85("(\\d+)=([a-zA-Z0-9_]+)\\(([0-9,\\s]+)\\)");
    const regex input_pad_regex_itc99("INPUT\\([a-zA-Z0-9_]+\\)");
    const regex output_pad_regex_itc99("OUTPUT\\([a-zA-Z0-9_]+\\))";
    const regex node_regex_itc99("([a-zA-Z0-9_]+)=[A-Z]+\\(([a-zA-Z0-9_]+,?)+\\)");

    const regex isc85_detect("# c[0-9]*");

    const regex itc99_detect("# edf2bench v0.8");
    smatch filetype;
    smatch output_pad_regex_match;
    smatch input_pad_regex_match;
    smatch node_regexMatch;
    //A flag to process first line after the first comment all comments are ignored
    int file_type_check_status = 0;
    while (ifs.good()) {
        string line;
        getline(ifs, line);

	if(!file_type_check_status)
	{ 
               if(regex_match(line,filetype,itc99_detect))
	       {
	       file_type_is_ITC99 = true;
	       file_type_check_status++;
	       }

               if(regex_match(line,filetype,isc85_detect))
	       {
	       file_type_is_ISC85 = true;
	       file_type_check_status++;
	       }
	}

        // We only need to parse before the comment
        string code_line = line.substr(0, line.find("#"));

	//If the file type is ISC85 do this shizz bang
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
            int  construction_id = stoi(input_pad_regex_match[1]);            
            allocate_for_node_id(node_id);
            nodes_[node_id]->set_node_id(node_id);
            nodes_[node_id]->set_input_pad(true);
            continue;
        }

        // Find OUTPUT(<nodeNumber>)
        regex_match(code_line, output_pad_regex_match, output_pad_regex_isc_85);
        if (output_pad_regex_match.size() > 0) {
	                
            int  construction_id = stoi(input_pad_regex_match[1]);            
            allocate_for_node_id(construction_id);
            nodes_[node_id]->set_node_id(node_id);
            nodes_[node_id]->set_output_pad(true);
            continue;
        }

        // Find <nodeNumber>=(<nodeNumber...>)
        regex_match(code_line, node_regexMatch, node_regex_isc_85);
        if (node_regexMatch.size() > 0) {
             int construction_id = stoi(node_regexMatch[1]);
            string gate_type = node_regexMatch[2];
            transform(gate_type.begin(), gate_type.end(), gate_type.begin(), ::toupper);
            allocate_for_node_id(node_id);

            nodes_[node_id]->set_node_id(node_id);
            nodes_[node_id]->set_gate_type(gate_type);

            stringstream node_ids_str(node_regexMatch[3]);
            NodeID input_node_id;
            char delim;
            while (node_ids_str >> input_node_id) {
                nodes_[node_id]->add_to_fanin_list(input_node_id);
                node_ids_str >> delim;


          //1. First get the fanin list_size
	  //2. Calculate the gate_area by mapping using the map and k/2*(map_area)
	  //3. Add to SumOfGatesArea

            }

            continue;
        }
    }

	if(file_type_is_ITC99)
	{
              
        // Remove all whitespace to make regex simpler
        code_line.erase(remove_if(code_line.begin(), code_line.end(), ::isspace), code_line.end());

        if (code_line.length() <= 0)
            continue;

        // cout << code_line << endl;

        // Find INPUT(<nodeNumber>)
        regex_match(code_line, input_pad_regex_match, input_pad_regex_itc99);
        if (input_pad_regex_match.size() > 0) {
            //NodeID node_id = (input_pad_regex_match[1]);
	    
            Node_Construction_ID++
            allocate_for_node_id(Node_Construction_ID);
            nodes_[Node_Construction_ID]->set_node_id(node_id);
            nodes_[node_id]->set_input_pad(true);
            continue;
        }

        // Find OUTPUT(<nodeNumber>)
        regex_match(code_line, output_pad_regex_match, output_pad_regex_itc99);
        if (output_pad_regex_match.size() > 0) {
         //   NodeID node_id = stoi(output_pad_regex_match[1]);
	                
            Node_Construction_ID++
            allocate_for_node_id(Node_Construction_ID);
            nodes_[Node_Construction_ID]->set_node_id(node_id);
            allocate_for_node_id(node_id);
           // nodes_[node_id]->set_node_id(node_id);
           // nodes_[node_id]->set_output_pad(true);
            continue;
        }

        // Find <nodeNumber>=(<nodeNumber...>)
        regex_match(code_line, node_regexMatch, node_regex_itc99);
        if (node_regexMatch.size() > 0) {
            NodeID node_id = stoi(node_regexMatch[1]);
            string gate_type = node_regexMatch[2];
            transform(gate_type.begin(), gate_type.end(), gate_type.begin(), ::toupper);
            allocate_for_node_id(node_id);

            nodes_[node_id]->set_node_id(node_id);
            nodes_[node_id]->set_gate_type(gate_type);
            nodes_[node_id]->set_gate_info(gate_db_.get_gate_info(gate_type));

            stringstream node_ids_str(node_regexMatch[3]);
            NodeID input_node_id;
            char delim;
            while (node_ids_str >> input_node_id) {
                nodes_[node_id]->add_to_fanin_list(input_node_id);
                node_ids_str >> delim;
            }
            continue;
        }

        }
}
initChipWidth = ceil(sqrt(sumGateArea));
initCHipHeight = ceil(sumGateArea/initChipWidth);

Circuit::~Circuit() {
    for(const auto& node_ptr: nodes_) {
        if (node_ptr != nullptr)
            delete node_ptr;
    }
}

void Circuit::allocate_for_node_id(const NodeID& node_id) {
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

void Circuit::print_node_info(const NodeID& node_id) {
    if (node_id >= (NodeID) nodes_.size()) {
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

    cout << node_id << " ";
    if (nodes_[node_id]->is_input_pad()) {
        cout << "INPUT";
    } else {
        cout << nodes_[node_id]->get_gate_type() << " " <<
            nodes_[node_id]->get_fanin_list().front() << " " <<
            nodes_[node_id]->get_fanin_list().back() << " ";
            
            const GateInfo* gate_info = nodes_[node_id]->get_gate_info();
            if (gate_info == nullptr) {
                cout << 0.;
            } else {
                cout << gate_info->output_slew[2][1];
            }
    }
    cout << endl;
}

void Circuit::test() {
    /*
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
    */

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
