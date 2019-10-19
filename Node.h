//
// Created by rodri on 10/12/2019.
//

#ifndef ADJACENTLIST_NODE_H
#define ADJACENTLIST_NODE_H

#include <iostream>
#include <vector>

#include "Coordinates.h"

using namespace std;

template <typename T, typename coordinate_type>
struct Node {};

template <typename T>
struct Node<T, Coordinate2D> {
    T value;
    typedef Coordinate2D coordinate_type;
    coordinate_type coordinate;

    explicit Node(const string &values){
        int cantidad_de_coordenadas = (values.size()==5 ? 3 : 2);
        string valor;
        double valor_;
        vector<double> coordinates;
        char separador = ' ';

        for(size_t p = 0, q = 0, i = 0; i < cantidad_de_coordenadas; p = q, i++) {
            valor = values.substr(p+(p!=0),(q=values.find(separador, p+1))-p-(p!=0));
            valor_ = stod(valor);
            coordinates.push_back(valor_);
        }
        coordinate = Coordinate2D(coordinates[0], coordinates[1]);
    }

    void print_coordinates () {
        cout << "x: " << coordinate.x << ", y: " << coordinate.y << endl;
    }

    explicit Node(T value, float x, float y): value{value} {
        coordinate = Coordinate2D(x, y);
    }

    void print_value() {
        cout << value << " ";
    }

    T& operator*() {
        return value;
    }

    friend ostream &operator<<(ostream &os, const Node& self) {
        os << "node value: " << self.value << " at x: " << self.coordinate.x << " y: " << self.coordinate.y;
        return os;
    }

    bool operator==(const Node &rhs) const {
        return value == rhs.value;
    }

    bool operator!=(const Node &rhs) const {
        return !(rhs == *this);
    }
};

template <typename T>
struct Node<T, Coordinate3D> {

};

#endif //ADJACENTLIST_NODE_H
