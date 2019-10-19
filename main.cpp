#define node_int_2D Node<int, Coordinate2D>

#include "GraphDrawer.h"

#define adjacency_list_with_node_int_2D_and_vectorized AdjacencyList<node_int_2D, vectorized>

using namespace std;

int main(int argc, char** argv) {

    AdjacencyList<node_int_2D, vectorized> al;

    auto a = node_int_2D(3, 10, 10);
    auto b = node_int_2D(4, 10, 20);
    auto c = node_int_2D(1, 20, 10);

    al.insert_node(&a);
    al.insert_node(&b);
    al.insert_node(&c);

    al.link_node(&a,&c);

    al.link_node(&b, &a);

    al.print_adjacency_list();

    al.delete_node(&b);

    al.print_adjacency_list();

    // Dibujando el grafo

    GraphDrawer<adjacency_list_with_node_int_2D_and_vectorized, node_int_2D> graphDrawer(al);

    graphDrawer.initialize(argc, argv);

    return 0;
}