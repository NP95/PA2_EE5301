#ifndef CIRCUIT_HPP
#define CIRCUIT_HPP

#include <string>
#include <vector>
#include <list>

#include "CircuitNode.hpp"
//A circuit is made of nodes so the node definition is in the CircuitNode.hpp file
// Hierarchy of the files are  base 1. CircuitNode.hpp 2. CircuitNode.cpp 3.Circuit.hpp 4. Circuit.cpp
// Compared to PA1 since the data structure for the node is a bit different, for referencing a particular node instead of a NodeID we are using a construction ID, since our graph is a vector of nodes, and we are giving an index to each node using a construction id, for ISC85 the string nodeID is converted to integer
class Circuit {
    private:
        // Store a pointer to CircuitNode, so each resize only moves pointers around
        // rather than the entire CircuitNode object. Additionally, any unused elements
        // will contain a nullptr rather than a empty CircuitNode object, saving memory
        std::vector<CircuitNode*> nodes_;
	
       // GateDatabase gate_db_;

        // Resizes the nodes_ vector to fit the node_id
        void allocate_for_node_id(const NodeID& node_id);
    public:
        Circuit(const std::string& ckt_file);
        ~Circuit();

        void print_node_info(const int& construction_id);
        void test();
};

#endif //CIRCUIT_HPP
