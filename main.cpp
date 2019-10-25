#define node_int_2D Node<int, Coordinate2D>

//#include "GraphDrawer.h"

#include "AdjacencyList.h"
#include "Grafo.h"
#define adjacency_list_with_node_int_2D_and_vectorized AdjacencyList<node_int_2D, vectorized>

using namespace std;

int main(int argc, char** argv) {

    AdjacencyList<node_int_2D, vectorized>* al = new AdjacencyList<node_int_2D, vectorized>;

    auto* n1 = new node_int_2D(100, 100, 100);
    auto* n2 = new node_int_2D(200, 200, 200);
    auto* n3 = new node_int_2D(300, 300, 300);
    auto* n4 = new node_int_2D(400, 150, 150);

    al->insert_node_by_address(n1);
    al->insert_node_by_address(n2);
    al->insert_node_by_address(n3);
    al->insert_node_by_address(n4);

    al->print_adjacency_list();

    al->link_node_by_address(n1, n2);
    al->link_node_by_address(n2, n3);
    al->link_node_by_address(n1, n4);
    al->link_node_by_address(n4, n3);

    al->print_adjacency_list();

    Grafo<node_int_2D, dirigido> mygraph(al);

    mygraph.print_edges();

    auto x = new node_int_2D(150,156,150);

    mygraph.insert_node(x);

    mygraph.link_node_by_address(x,n1);

    mygraph.print_graph();

    mygraph.print_edges();

    mygraph.delete_node(n4);

    mygraph.print_edges();

    mygraph.print_graph();

    mygraph.delete_connection_with_addresses(x,n1);

    mygraph.print_graph();
    mygraph.print_edges();

    return 0;
}