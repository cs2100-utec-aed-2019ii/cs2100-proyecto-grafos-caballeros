#include <iostream>
#include "AdjacencyList.h"
#include "Node.h"
#include <vector>
using namespace std;
int main() {

    AdjacencyList< Node<int> , vector<Node<int>> > al;
    typedef Node<int> Node;

    auto a = Node(3);
    auto b = Node(4);
    auto c = Node(1);

    al.insert_node(&a);
    al.insert_node(&b);
    al.insert_node(&c);

    al.link_node(&a,&c);


    al.link_node(&b, &a);


    al.print_adjacency_list();



    al.delete_node(&b);

    al.print_adjacency_list();

    return 0;
}