#include <iostream>
#include "Lectura.h"
#include "AdjacencyList.h"
#include "GraphDrawer.h"

#define node_int_2D Node<int, Coordinate2D>
#define adjacency_list_with_node_int_2D_and_vectorized AdjacencyList<node_int_2D, vectorized>

using namespace std;

int main(int argc, char * argv[]) {

    srand(time(nullptr));

    Lectura< Node<int, Coordinate2D>, Coordinate2D > l1;

    ///Adjacency list
    auto list = l1.cargar_datos("nodos.vtk");

    l1.print();
    //list.print_adjacency_list();


    return 0;
}