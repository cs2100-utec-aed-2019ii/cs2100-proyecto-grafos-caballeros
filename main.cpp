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
    al.insert_node(&d);
    al.insert_node(&a);
    al.insert_node(&e);
    al.insert_node(&b);
    al.insert_node(&c);
    al.insert_node(&f);

    al.link_node(&a,&b);

    al.link_node(&a,&c);

    al.link_node(&a,&e);

    al.link_node(&a,&f);

    al.link_node(&a,&d);


    al.print_adjacency_list();



    al.delete_node(&d);

    al.print_adjacency_list();

    al.delete_node(&f);

    al.print_adjacency_list();

    al.delete_node(&e);

    al.print_adjacency_list();

    auto aux = al.nodes_linked_to_node(&a);

    for (int i=0;i<aux.size();i++){
        aux[i].print_value();
    }

    return 0;
}