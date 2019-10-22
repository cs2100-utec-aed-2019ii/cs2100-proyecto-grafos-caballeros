#include <iostream>
#include "Lectura.h"
#include "AdjacencyList.h"

#define node_int_2D Node<int, Coordinate2D>

using namespace std;

int main(int argc, char * argv[]) {

    srand(time(nullptr));

    Lectura< Node<int, Coordinate2D>, Coordinate2D > l1;

    ///Adjacency list
    auto list = l1.cargar_datos("nodos.vtk");

    l1.print();

    cout << "\nLista de adyacencia: " << endl;
    list.print_adjacency_list();


    return 0;
}