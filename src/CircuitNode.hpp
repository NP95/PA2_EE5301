#ifndef CIRCUITNODE_HPP
#define CIRCUITNODE_HPP

#include <string>
#include <list>

typedef std::string NodeID;
typedef int GateArea;
class CircuitNode {
    private:
        NodeID node_id_;
	int x_coordinate;
	int y_coordinate;
	GateArea area;
	int construction_id;
        std::string gate_type_;
        std::list<NodeID> fanin_list_;
	std::list<NodeID> fanout_list_;

    public:
        CircuitNode() :
            node_id_(""), x_coordinate(0),y_coordinate(0),area(0),construction_id(0),gate_type_("") { }
       //This type of initialization is called the initializer list
       //https://stackoverflow.com/questions/7665021/c-member-initialization-list
       //the order of initialization is decided in the order in which the data members are declared, not the the order in which you have initialized the data members using initialization list
       //https://www.youtube.com/watch?v=1nfuYMXjZsA
        void set_node_id(const NodeID& node_id);
	void set_x_coordinate(const int& x_coordinate);
	void set_y_coordinate(const int& y_coordinate);
	void set_gate_area(const GateArea& area);
	void set_construction_id(const int& construction_id);
        void set_gate_type(const std::string& gate_type);
        void add_to_fanin_list(const NodeID& node_id);
	void add_to_fanout_list(const NodeID& node_id);
	
       //These are basically function declarations. The definitions are in a separate file 
        const NodeID& get_node_id() const;
        const int& get_x_coordinate() const;
        const int& get_y_coordinate() const;
        const GateArea& get_area() const;
        const int&  get_construction_id() const;
        const std::string& get_gate_type() const;
        const std::list<NodeID>& get_fanin_list() const;
	const std::list<NodeID>& get_fanout_list() const;
	/*Explanation copied from here
	 * https://www.geeksforgeeks.org/const-member-functions-c/
	 A free function cannot be marked with “const”, only a method can.

A method (a function that belongs to a class) marked with “const”, has the type of its “this” pointer turned into const.

For example:

struct A 
{ 
	int n; 
 
    void show() const 
    { 
        std::cout << this->n << "\n"; 
    } 
}; 
In that example, the type of “this” in the method “show” is “const A*”. The type was modified to “const” because the method is marked with const.

Consequences of that?

The method explicits that it cannot modify the value of any attribute of the object, except if it is marked as “mutable”.
Inside this method, no “non-const” method of same class can be invoked, only const-ones.
If the method wants to return a reference to an attribute of the class, the reference has to be const as well.
 */
};

#endif //CIRCUITNODE_HPP
