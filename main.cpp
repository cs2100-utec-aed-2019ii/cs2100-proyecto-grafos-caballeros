#include <iostream>
#include <fstream>
#include "Lectura.h"
#include "GraphDrawer.h"

using namespace std;

int main(int argc, char * argv[]) {

    Lectura< Node<int, Coordinate2D>, Coordinate2D > l1;

    auto nodos = l1.cargar_datos("nodos.vtk");

    for (auto & nodo : nodos){
        nodo->print_coordinates();
    }

    return 0;
}
