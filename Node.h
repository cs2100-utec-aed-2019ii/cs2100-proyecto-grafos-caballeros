//
// Created by rodri on 10/12/2019.
//

#ifndef ADJACENTLIST_NODE_H
#define ADJACENTLIST_NODE_H

#include <ostream>

using namespace std;
template <typename T>
struct Node {
    T value;
public:
    explicit Node(T value): value{value} {}

    T get_value() { return value; }

    void print_value() { cout << value <<" ";}

    T& operator*() { return value; }

    friend ostream &operator<<(ostream &os, const Node &node) {
        os << "value: " << node.value;
        return os;
    }

    bool operator==(const Node &rhs) const {
        return value == rhs.value;
    }

    bool operator!=(const Node &rhs) const {
        return !(rhs == *this);
    }
};

#endif //ADJACENTLIST_NODE_H
