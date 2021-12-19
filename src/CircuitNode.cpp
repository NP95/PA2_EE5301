#include "CircuitNode.hpp"

//https://stackoverflow.com/questions/983310/calling-a-method-from-another-method-in-the-same-class-in-c
//https://stackoverflow.com/questions/9745722/bad-programming-practice-to-have-a-method-dependent-on-another-method
//Consensus seems it is tricky to do so, since private variables, use a setter method
//These are setter methods

void CircuitNode::set_node_id(const NodeID& node_id) 
{
    node_id_ = node_id;
}

void CircuitNode::set_x_coordinate(const int& x_coordinate_)
{
   x_coordinate = x_coordinate_;
}

void CircuitNode::set_y_coordinate(const int& y_coordinate_)
{
   y_coordinate = y_coordinate_;
}

void CircuitNode::set_gate_area(const int& area_)
{
  area = area_; 
}

void CircuitNode::set_construction_id(const int& construction_id_) 
{
    construction_id = construction_id_;
}

void CircuitNode::set_gate_type(const std::string& gate_type) 
{
    gate_type_ = gate_type;
}

void CircuitNode::add_to_fanin_list(const NodeID& node_id) 
{
    fanin_list_.push_back(node_id);
}

void CircuitNode::add_to_fanout_list(const NodeID& node_id)
{
   fanout_list_.push_back(node_id);
}

//These are getter methods, the const declaration ensures that calling them does not modify the object
const NodeID& CircuitNode::get_node_id() const 
{
    return node_id_;
}


const int& CircuitNode::get_x_coordinate() const 
{
    return x_coordinate;
}

const int& CircuitNode::get_y_coordinate() const
{
	return y_coordinate;
}

const GateArea& CircuitNode::get_area() const
{
	return area;
}

const int& CircuitNode::get_construction_id() const
{
	return construction_id;
}

const std::string& CircuitNode::get_gate_type() const 
{
    return gate_type_;
}


const std::list<NodeID>& CircuitNode::get_fanin_list() const 
{
    return fanin_list_;
}


const std::list<NodeID>& CircuitNode::get_fanout_list() const 
{
    return fanout_list_;
}
