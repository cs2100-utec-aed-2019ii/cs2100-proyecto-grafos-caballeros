#include <iostream>
#include <fstream>
#include "GraphDrawer.h"

using namespace std;

int main(int argc, char * argv[]) {
    vector < Node<int, Coordinate2D>* > nodos;
    int cantidad_de_nodos = 0;
    string cantidad;
    string line;
    ifstream myfile ("test.vtk");
    char separador = ' ';

    if (myfile.is_open()) {
        for (int i = 0; i < 5; ++i)
            getline(myfile, line);

        string sline = line;

        ///Extraer la cantidad de nodos
        getline(myfile, line);
        sline = line;
        for(size_t p = 0, q = 0, i = 0; i < 2; p = q, i++)
            cantidad = sline.substr(p+(p!=0),(q=sline.find(separador, p+1))-p-(p!=0));

        cantidad_de_nodos = stoi(cantidad);

        cout << "Cantidad de nodos: " << cantidad_de_nodos << endl;

        for (int i = 0; i < cantidad_de_nodos; ++i) {
            getline(myfile, line);
            auto node = new Node<int, Coordinate2D>(line);
            nodos.push_back(node);
        }

        for (int i = 0; i < nodos.size(); ++i) {
            nodos[i]->print_coordinates();
        }

        //Linea vacia
        getline(myfile, line);

        //TODO: Realizar las conexiones de los nodos
        while ( getline (myfile,line) ) {
            cout << "line: " << line << '\n';
        }


        myfile.close();

    } else {
        cout << "Unable to open file" << endl;
    }

    return 0;
}