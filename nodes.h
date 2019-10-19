#ifndef nodes_h
#define nodes_h

#include <iostream>
#include <string>

template <class T>
struct Node {
    T x, y, z;
    std::string values;
    Node(std::string values): values{values} {
        int cantidad_de_coordenadas = (values.size()==5 ? 3 : 2);
        std::string valor;
        char separador = ' ';

        for(size_t p = 0, q = 0, i = 0; i < cantidad_de_coordenadas; p = q, i++) {
            valor = values.substr(p+(p!=0),(q=values.find(separador, p+1))-p-(p!=0));
            std::cout << valor << " ";
        }
    }

};

#endif
