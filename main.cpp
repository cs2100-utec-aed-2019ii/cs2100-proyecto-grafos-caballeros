#define node_int_2D Node<int, Coordinate2D>

//#include "GraphDrawer.h"

#include "AdjacencyList.h"
#define adjacency_list_with_node_int_2D_and_vectorized AdjacencyList<node_int_2D, vectorized>

using namespace std;

int main(int argc, char** argv) {

    AdjacencyList<node_int_2D, vectorized> al;

    auto a = node_int_2D(3, 10, 10);
    auto b = node_int_2D(4, 10, 20);
    auto c = node_int_2D(1, 20, 10);
    auto d = node_int_2D(2, 2, 1);
    auto f = node_int_2D(0,1,1);
    auto e = node_int_2D(5, 2, 11);
    al.insert_node_by_address(&d);

    al.insert_node_by_address(&a);

    al.insert_node_by_address(&e);

    al.insert_node_by_address(&b);

    al.insert_node_by_address(&c);

    al.insert_node_by_address(&f);

    al.link_node_by_value(3,1);

    al.link_node_by_value(3,0);

    al.print_adjacency_list();

    al.delete_node_by_value(1);

    al.print_adjacency_list();

    return 0;
}