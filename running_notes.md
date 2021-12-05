Parser,Initial Placement and HPWL Architecture

A circuit is made of nodes so the node definition is in the CircuitNode.hpp file
 Hierarchy of the files are o CircuitNodei\_PA2.hpp-> CircuitNode.cpp-\>Circuit.hpp-\>Circuit.cpp

1. Take in the file
2. Check whether the file is a ISCAS85 or ITC99i (Done)

 How do you do that?
Regex for #c and alphanumeric is iSC85
Regex for # edf2bench v0.8
Set the boolean flag respectively

How do you do this?
As you are reading in the loop, initialize a flag\_reader and after the first line is read, initialize the flag and ignore rest of the comments

3. What should be regex pattern logic for each?
OUTPUT\(([a-zA-Z0-9_]+)\)  smatch[1] is the NodeIdentifier->Regex for an output pad
INPUT\(([(a-zA-Z0-9_)]+)\) smatch[1] is the NodeIdentifier-> Regex for an input pad

([a-zA-Z0-9_]+)=([a-zA-Z0-9_]+)\\(([0-9,\\s]+)\\)
([a-zA-Z0-9_]+)=([A-Z]+)\((([a-zA-Z0-9_]+,?)+)\)-> Regex for node
smatch[1] -> 

If file is ISC85, use Gaurav's logic in PA0b
If file is ITC99 then?
"Unordered maps"
"Strings"

4. What do you need in the graph? Justify each decision you make. You alsh have to think how these will relate to simulated annealing
Create the graph and initialize chip_area = 0
a. Each node should have the fan in and fan out. Strictly define what is the context of these
b. The data structure 
->1. bool assigned or not
->2. x,y coordinates
->3. fan in
->4. Calculate area of gate,store it and add it to initial chip area

Once the graph is created then, you have a rectangle x units by y units

What is x and y?
Psuedocode for  this step
a/ For ISC 85






b/ For ITC99

5. How should you reason behind the initial random placement
6. Function for calculating the HPWL and doesn't matter in this case inplace implementation would be faster
Not a 1-1 map, first determine the gate type and then the number of inputs


