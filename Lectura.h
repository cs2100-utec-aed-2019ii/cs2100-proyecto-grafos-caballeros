#ifndef GRAPHOS_LECTURA_H
#define GRAPHOS_LECTURA_H

#include <vector>
#include "Node.h"

template <class node_type, class coordinate_type>
struct Lectura {};

template <class node_type>
struct Lectura<node_type, Coordinate2D> {

    vector < node_type* > cargar_datos (const string& vtk_file) {
        ///Revisar si existe un archivo previo
        ifstream check("nodos_antiguos.vtk");
        bool prev;
        if ( check.fail() ){
            cout << "No existe previo archivo." << endl;
            system("cp nodos.vtk nodos_antiguos.vtk");
            prev = false;
        } else {
            cout << "Existe previo archivo." << endl;
            prev = true;
        }

        string path = prev ? "nodos_antiguos.vtk" : "nodos.vtk";
        ifstream myfile (path);
        vector < Node<int, Coordinate2D>* > nodos;
        int cantidad_de_nodos = 0;
        string cantidad;
        string line;
        char separador = ' ';

        ///Lectura del .vtk
        if ( myfile.is_open() ) {
            for (int i = 0; i < 5; ++i)
                getline(myfile, line);

            string sline = line;

            ///Extraer la cantidad de nodos
            getline(myfile, line);
            sline = line;
            for (size_t p = 0, q = 0, i = 0; i < 2; p = q, i++)
                cantidad = sline.substr(p + (p != 0), (q = sline.find(separador, p + 1)) - p - (p != 0));

            cantidad_de_nodos = stoi(cantidad);

            cout << "Cantidad de nodos: " << cantidad_de_nodos << endl;

            ///Crear los nodos y guardarlos en un vector
            for (int i = 0; i < cantidad_de_nodos; ++i) {
                getline(myfile, line);

                int cantidad_de_coordenadas = 2;
                string valor;
                double valor_;
                vector<double> coordinates;

                for(size_t p = 0, q = 0, i = 0; i < cantidad_de_coordenadas; p = q, i++) {
                    valor = line.substr(p+(p!=0),(q=line.find(separador, p+1))-p-(p!=0));
                    valor_ = stod(valor);
                    coordinates.push_back(valor_);
                }
                auto node = new Node<int, Coordinate2D>(coordinates[0], coordinates[1]);
                nodos.push_back(node);
            }

        }

        return nodos;

    }

};

template <class node_type>
struct Lectura<node_type, Coordinate3D> {
    vector < node_type* > cargar_datos (const string& vtk_file) {
        ///Revisar si existe un archivo previo
        ifstream check("nodos_antiguos.vtk");
        bool prev;
        if ( check.fail() ){
            cout << "No existe previo archivo." << endl;
            system("cp nodos.vtk nodos_antiguos.vtk");
            prev = false;
        } else {
            cout << "Existe previo archivo." << endl;
            prev = true;
        }

        string path = prev ? "nodos_antiguos.vtk" : "nodos.vtk";
        ifstream myfile (path);
        vector < Node<int, Coordinate2D>* > nodos;
        int cantidad_de_nodos = 0;
        string cantidad;
        string line;
        char separador = ' ';

        ///Lectura del .vtk
        if ( myfile.is_open() ) {
            for (int i = 0; i < 5; ++i)
                getline(myfile, line);

            string sline = line;

            ///Extraer la cantidad de nodos
            getline(myfile, line);
            sline = line;
            for (size_t p = 0, q = 0, i = 0; i < 2; p = q, i++)
                cantidad = sline.substr(p + (p != 0), (q = sline.find(separador, p + 1)) - p - (p != 0));

            cantidad_de_nodos = stoi(cantidad);

            cout << "Cantidad de nodos: " << cantidad_de_nodos << endl;

            ///Crear los nodos y guardarlos en un vector
            for (int i = 0; i < cantidad_de_nodos; ++i) {
                getline(myfile, line);

                int cantidad_de_coordenadas = 2;
                string valor;
                double valor_;
                vector<double> coordinates;

                for(size_t p = 0, q = 0, i = 0; i < cantidad_de_coordenadas; p = q, i++) {
                    valor = line.substr(p+(p!=0),(q=line.find(separador, p+1))-p-(p!=0));
                    valor_ = stod(valor);
                    coordinates.push_back(valor_);
                }
                auto node = new Node<int, Coordinate2D>(coordinates[0], coordinates[1]);
                nodos.push_back(node);
            }

        }

        return nodos;

    }
};

#endif
