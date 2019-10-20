//
// Created by rodri on 10/12/2019.
//

#ifndef ADJACENTLIST_NODE_H
#define ADJACENTLIST_NODE_H

#include <iostream>
#include <vector>
#include <random>

#include "Coordinates.h"

using namespace std;

template <typename T, typename coordinate_type>
struct Node {};

template <typename T>
struct Node<T, Coordinate2D> {
    T value;
    typedef Coordinate2D coordinate_type;
    coordinate_type coordinate;

    void print_coordinates () {
        cout << "x: " << coordinate.x << ", y: " << coordinate.y << endl;
    }

    Node(float x, float y){
        srand(time(nullptr));
        value = (rand()%1000)+1;
        coordinate = Coordinate2D(x, y);
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