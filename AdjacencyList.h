#ifndef ADJACENTLIST_ADJACENCYLIST_H
#define ADJACENTLIST_ADJACENCYLIST_H

#include <iterator>
#include <list>

#include "Node.h"

#define adjacency_list_matrix (*adjacency_list)
#define vectorized 1
#define linked_list 2

using namespace std;

template <typename node_type, short container>
struct AdjacencyList {};

template <typename node_type>
struct AdjacencyList<node_type, vectorized> { /// ADJACENCY LIST WITH VECTOR

    typedef node_type Node;
    unsigned int size = 0;

    vector<vector<Node>>* adjacency_list = new vector < vector<Node> >;

    /// CONSTRUCTURES POR DEFECTO
    AdjacencyList() {
        vector<Node> aux;
        adjacency_list->push_back(aux);
    }

    ~AdjacencyList() = default;

    void insert_node(Node* node) {

        adjacency_list_matrix[size].push_back(*node);
        ++size;
        vector<Node> aux;
        adjacency_list_matrix.push_back(aux);
    }

    void link_node(Node* node_from, Node* node_to) {

        for (int i = 0; i < size; ++i) {
            if ( adjacency_list_matrix[i][0] == *node_from ) {
                adjacency_list_matrix[i].push_back(*node_to);
                goto Exit;
            }
        }

        Exit:

        cout << "Node linked" << endl;
    }

    vector<vector<Node>> get_matrix() {
        return adjacency_list_matrix;
    }

    void print_adjacency_list() {
        for (int i = 0; i < size; ++i) {
            for (int j = 0; j < adjacency_list_matrix[i].size(); ++j) {
                adjacency_list_matrix[i][j].print_value();
            }
            cout << endl;
        }
    }

   // void sort_adjacency_list(){}

   void delete_node(Node* node) {
        //TODO ELIMINAR NODO DE LA LISTA DE CONEXIONES EN LOS DEMAS NODOS



        for (auto it = adjacency_list_matrix.begin(); it != adjacency_list_matrix.end()-1; ++it) {
            if(it[0][0] == *node) {
                adjacency_list_matrix.erase(it);
                goto End;
            }
        }

        End:
        --size;
        cout << "Node deleted" << endl;

    }

};




template <typename data_type>
struct AdjacencyList<data_type, linked_list> { /// ADJACENCY LIST WITH LINKED LIST
    list< list<data_type> > adjacency_list;

};


#endif //ADJACENTLIST_ADJACENCYLIST_H
