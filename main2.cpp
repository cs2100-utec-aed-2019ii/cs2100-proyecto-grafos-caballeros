#include <iostream>
#include "AdjacencyList.h"
#define node_type Node<int, Coordinate2D>

using namespace std;

int main(){
    AdjacencyList< node_type, 1> al;

    auto n0 = new node_type(0);
    auto n1 = new node_type(1);
    auto n2 = new node_type(2);
    auto n3 = new node_type(3);

    al.insert_node_by_address(n0);
    al.insert_node_by_address(n1);
    al.insert_node_by_address(n2);
    al.insert_node_by_address(n3);

    al.link_node_by_address(n0, n1);
    al.link_node_by_address(n0, n2);
    al.link_node_by_address(n1, n2);
    al.link_node_by_address(n2, n0);
    al.link_node_by_address(n2, n3);

    al.print_adjacency_list();

    al.isReachable(0, 3);

    return 0;
}