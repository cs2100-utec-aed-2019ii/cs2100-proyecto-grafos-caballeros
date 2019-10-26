//
// Created by rodri on 10/12/2019.
//

#ifndef ADJACENTLIST_NODE_H
#define ADJACENTLIST_NODE_H

#include <iostream>
#include <random>
#include <ctime>

#include "Macros.h"
#include "Coordinates.h"

using namespace std;

template <typename T, typename coordinate_type>
struct Node {};

template <typename T>
struct Node<T, Coordinate2D> {
    T value;
    int max_value =  1000;
    long id;
    typedef T n_type;
    typedef Coordinate2D coordinate_type;
    coordinate_type coordinate;

    Node() = default;

    void print_all () {
        cout << "x: " << coordinate.x << ", y: " << coordinate.y << ", z: " << 0 << ", value: " << value << endl;
    }

    void new_value() {
        value = (rand()%max_value)+1;
    }

    Node(float x, float y, long max_value_, long id_) {
        id = id_;
        max_value = max_value_;
        value = (rand()%max_value)+1;
        coordinate = Coordinate2D(x, y);
    }

    /*Node(float x, float y, long max_value_) {
        max_value = max_value_;
        value = (rand()%max_value)+1;
        coordinate = Coordinate2D(x, y);
    }*/

    Node(float x, float y) {
        value = (rand()%max_value)+1;
        cout << "VALUAO: " << value << endl;
        coordinate = Coordinate2D(x, y);
    }

    Node(T value_) {
        //TODO: Verificar que no haya superposición
        value = value_;
        coordinate = Coordinate2D((rand()%WINDOW_WIDTH)+1, (rand()%WINDOW_HEIGHT)+1);
    }

    Node(T value, float x, float y): value{value} {
        coordinate = Coordinate2D(x, y);
    }

    void print_id() {
        cout << id << " ";
    }

    void print_value() {
        cout << value << " ";
    }

    T& operator*() {
        return value;
    }

    T get_value() { return value; }

    bool operator<(const Node<T, Coordinate2D>& rhs) const {
        return value < rhs.value;
    }

    bool operator>(const Node<T, Coordinate2D> &rhs) const {
        return value > rhs.value;
    }

    bool operator<=(const Node<T, Coordinate2D> &rhs) const {
        return value <= rhs.value;
    }

    bool operator>=(const Node<T, Coordinate2D> &rhs) const {
        return value >= rhs.value;
    }

    friend ostream &operator<<(ostream &os, const Node& self) {
        os << "node value: " << self.value << " at x: " << self.coordinate.x << " y: " << self.coordinate.y;
        return os;
    }

    bool operator==(const Node &rhs) const {
        return value == rhs.value;
    }

    bool operator!=(const Node &rhs) const {
        return !(*this == rhs);
    }

    ~Node() {
//        cout << "HE MUERTO TENGO VALOR: " << value << endl;
    }

};

template <typename T>
struct Node<T, Coordinate3D> {

};

#endif //ADJACENTLIST_NODE_H