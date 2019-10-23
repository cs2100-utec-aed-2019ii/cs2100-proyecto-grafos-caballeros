//
// Created by Gabriel Spranger Rojas on 10/19/19.
//

#ifndef GRAPHOS_LECTURA_H
#define GRAPHOS_LECTURA_H

#include <fstream>
#include <GLUT/glut.h>
#include <OpenGL/gl.h>
#include <OpenGL/glu.h>

#include "AdjacencyList.h"

template <class node_type, class coordinate_type>
struct Lectura {};

template <class node_type>
struct Lectura<node_type, Coordinate2D> {

    vector < node_type* > vtk_nodes;

    AdjacencyList<node_type, vectorized>* cargar_datos(const string& vtk_file) {
        
        ///Revisar si existe un archivo previo
        ifstream check("nodos_antiguos.vtk");
        bool prev;
        if ( check.fail() ){
            cout << "No existe previo archivo." << endl;
            //system("cp nodos.vtk nodos_antiguos.vtk");
            prev = false;
        } else {
            cout << "Existe previo archivo." << endl;
            prev = true;
        }

        string path = prev ? "nodos_antiguos.vtk" : "nodos.vtk";
        ifstream myfile (path);
        vector < Node<int, Coordinate2D>* > nodos;
        long cantidad_de_nodos = 0;
        string cantidad;
        string line;
        char separador = ' ';
        auto* list = new AdjacencyList<node_type, vectorized>();

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

            cantidad_de_nodos = stol(cantidad);

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
                auto node = new Node<int, Coordinate2D>(coordinates[0], coordinates[1], cantidad_de_nodos*3, i);
                nodos.push_back(node);
            }

        }

        vtk_nodes = nodos;

        for (auto & nodo : nodos) {
            while (list->search_node_by_value_returns_position(nodo->value) != -1){
                nodo->new_value();
            }
            list->insert_node_by_address(nodo);
        }

        ///Eliminar linea blanca
        getline(myfile, line);

        ///Extraer la cantidad de conexiones
        int conexiones_totales = 0;
        string cline;
        getline(myfile, line);
        cline = line;
        for (size_t p = 0, q = 0, i = 0; i < 2; p = q, i++) {
            cantidad = cline.substr(p + (p != 0), (q = cline.find(separador, p + 1)) - p - (p != 0));
        }
        conexiones_totales = stoi(cantidad);
        cout << "Conexiones totales: " << conexiones_totales << endl;

        ///Extraer las conexiones que hara cada nodo
        for (int i = 0; i < conexiones_totales; ++i) {
            getline(myfile, line);
            size_t q_ = 0;
            string nodos_a_conectar;
            string nodo;
            for (size_t p = 0, q = 0, i = 0; i < 1; p = q, i++) {
                nodos_a_conectar = line.substr(p + (p != 0), (q = line.find(separador, p + 1)) - p - (p != 0));
                q_ += q;
            }

            vector<int> indices;

            ///Extraer las conexiones
            for (size_t p = q_, q, i = 0; i < stoi(nodos_a_conectar) ; p = q, i++) {
                nodo = line.substr(p + (p != 0), (q = line.find(separador, p + 1)) - p - (p != 0));
                indices.push_back(stoi(nodo));
            }

            for (int i = 0; i < indices.size(); ++i) {
                    auto nodo_1 = list->search_node_by_value_returns_address(vtk_nodes[indices[0]]->value);
                    auto nodo_2 = list->search_node_by_value_returns_address(vtk_nodes[indices[i]]->value);
                    list->link_node_by_address(nodo_1, nodo_2);
            }
        }

        return list;

    }

    void print() {
        for (auto & node : vtk_nodes) { node->print_all(); }
    }

    void save_adjacency_list (AdjacencyList<node_type, vectorized>* al) {
        ofstream myfile("nodos_antiguos.vtk");
        ///Lineas por defecto
        myfile << "# vtk DataFile Version 2.0\n2D scalar data\nASCII\nDATASET UNSTRUCTURED_GRID\n\n";
        myfile << "POINTS " << al->size << " float" << endl;

        auto adj_mat = al->get_matrix();

        ///Ordenar por id
        

        ///Nodos
        for (unsigned i = 0; i < al->size; ++i) {
            myfile << adj_mat[i][0].coordinate.x << " " << adj_mat[i][0].coordinate.y << " 0";
            myfile << endl;
        }

        ///Conexiones de nodos

        myfile << "\nCELLS " << al->size << endl;
        for (unsigned i = 0; i < al->size; ++i) {
            myfile << adj_mat[i].size() << " ";
            for (unsigned j = 0; j < adj_mat[i].size(); ++j) {
                myfile << adj_mat[i][j].id << " ";
            }
            myfile << endl;
        }
        myfile<<endl;
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
            //system("cp nodos.vtk nodos_antiguos.vtk");
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