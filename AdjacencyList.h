#ifndef ADJACENTLIST_ADJACENCYLIST_H
#define ADJACENTLIST_ADJACENCYLIST_H

#include <vector>
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

    vector < vector<Node> > * adjacency_list = new vector < vector<Node> >;

    /// CONSTRUCTURES POR DEFECTO
    AdjacencyList() = default;
    ~AdjacencyList() = default;


//--------------------------------------------------------------------------INSERT NODE

    void insert_node_by_address(Node* node) {
        if (size == 0) {
            vector<Node> aux;
            aux.push_back(*node);
            adjacency_list_matrix.push_back(aux);
            ++size;

        } else {
            int pos = 0;
            int l = 0;
            int r = size;
            if (*node > adjacency_list_matrix[r - 1][0]) {
                pos = r - 1;
            } else if (*node < adjacency_list_matrix[0][0]) {
                pos = -1;
            } else {
                while (l <= r) {
                    int m = l + (r - l) / 2;

                    if (adjacency_list_matrix[m][0] < *node && adjacency_list_matrix[m + 1][0] > *node) {
                        pos = m;
                        goto Exit;
                    }

                    if (adjacency_list_matrix[m][0] < *node) {
                        l = m + 1;
                    } else {
                        r = m - 1;
                    }
                }

                cout << "Node with same value already inserted" << endl;
                pos = -2;
                goto Exit;
            }
            Exit:
            if (pos == -1) {
                vector<Node> aux;
                aux.push_back(*node);
                adjacency_list_matrix.insert(adjacency_list_matrix.begin(), aux);
                ++size;

            } else if (pos == -2) {
                cout << "Node not inserted" << endl;
            } else {
                vector<Node> aux;
                aux.push_back(*node);
                auto it = adjacency_list_matrix.begin() + (pos + 1);
                adjacency_list_matrix.insert(it, aux);
                ++size;
            }
        }
    }

    void link_node_by_position(int position_from, int position_to) {
        int p1 = position_from;
        int p2 = position_to;

        auto node_from = &adjacency_list_matrix[p1][0];
        auto node_to = &adjacency_list_matrix[p2][0];

        int l = 0;
        int pos = 0;
        int r = size;
        while (l <= r){
            int m = l + (r-l)/2;

            if (adjacency_list_matrix[m][0] == *node_from)
                pos = m;

            if (adjacency_list_matrix[m][0] < *node_from)
                l = m + 1;

            else
                r = m - 1;
        }

        if(adjacency_list_matrix[pos].size() == 1){
            adjacency_list_matrix[pos].push_back(*node_to);

        }

        else{
            int pos1 = 0;
            int l1 = 0;
            int r1 = adjacency_list_matrix[pos].size();
            if(*node_to > adjacency_list_matrix[pos][r1-1]){
                pos1 = r1-1;
            }
            else if(*node_to < adjacency_list_matrix[pos][1]){
                pos1 = -1;
            }
            else{
                while (l1 <= r1) {
                    int m1 = l1 + (r1 - l1) / 2;
                    if (adjacency_list_matrix[pos][m1] < *node_to && adjacency_list_matrix[pos][m1+1] > *node_to) {
                        pos1 = m1;
                        goto Exit;
                    }

                    if (adjacency_list_matrix[pos][m1] < *node_to) {
                        l1 = m1 + 1;
                    }
                    else {
                        r1 = m1 - 1;
                    }
                }
                cout<<"Node with same value already inserted"<<endl;
                pos1 = -2;
                goto Exit;
            }
            Exit:
            if(pos1 == -1){
                adjacency_list_matrix[pos].insert(adjacency_list_matrix[pos].begin()+1,*node_to);

            }
            else if (pos1 == -2){
                cout << "Node not inserted" << endl;
            }
            else{
                auto it = adjacency_list_matrix[pos].begin() +  (pos1 +1);
                adjacency_list_matrix[pos].insert(it, *node_to);

            }
        }

        cout << "Node linked" << endl;
    }

    void insert_node_by_value(typename node_type::n_type value) {
        Node * node = new Node(value);
        if (size == 0) {
            vector<Node> aux;
            aux.push_back(*node);
            adjacency_list_matrix.push_back(aux);
            ++size;

        } else {
            int pos = 0;
            int l = 0;
            int r = size;
            if (*node > adjacency_list_matrix[r - 1][0]) {
                pos = r - 1;
            } else if (*node < adjacency_list_matrix[0][0]) {
                pos = -1;
            } else {
                while (l <= r) {
                    int m = l + (r - l) / 2;

                    if (adjacency_list_matrix[m][0] < *node && adjacency_list_matrix[m + 1][0] > *node) {
                        pos = m;
                        goto Exit;
                    }

                    if (adjacency_list_matrix[m][0] < *node) {
                        l = m + 1;
                    } else {
                        r = m - 1;
                    }
                }

                cout << "Node with same value already inserted" << endl;
                pos = -2;
                goto Exit;
            }
            Exit:
            if (pos == -1) {
                vector<Node> aux;
                aux.push_back(*node);
                adjacency_list_matrix.insert(adjacency_list_matrix.begin(), aux);
                ++size;

            } else if (pos == -2) {
                cout << "Node not inserted" << endl;
            } else {
                vector<Node> aux;
                aux.push_back(*node);
                auto it = adjacency_list_matrix.begin() + (pos + 1);
                adjacency_list_matrix.insert(it, aux);
                ++size;
            }
        }
    }



//--------------------------------------------------------------------------


//--------------------------------------------------------------------------LINK NODE

    void link_node_by_value(typename node_type::n_type value_from, typename node_type::n_type value_to) {
        int p1 = search_node_by_value_returns_position(value_from);
        int p2 = search_node_by_value_returns_position(value_to);

        auto node_from = &adjacency_list_matrix[p1][0];
        auto node_to = &adjacency_list_matrix[p2][0];

        int l = 0;
        int pos = 0;
        int r = size;
        while (l <= r){
            int m = l + (r-l)/2;

            if (adjacency_list_matrix[m][0] == *node_from)
                pos = m;

            if (adjacency_list_matrix[m][0] < *node_from)
                l = m + 1;

            else
                r = m - 1;
        }

        if(adjacency_list_matrix[pos].size() == 1){
            adjacency_list_matrix[pos].push_back(*node_to);

        }

        else{
            int pos1 = 0;
            int l1 = 0;
            int r1 = adjacency_list_matrix[pos].size();
            if(*node_to > adjacency_list_matrix[pos][r1-1]){
                pos1 = r1-1;
            }
            else if(*node_to < adjacency_list_matrix[pos][1]){
                pos1 = -1;
            }
            else{
                while (l1 <= r1) {
                    int m1 = l1 + (r1 - l1) / 2;
                    if (adjacency_list_matrix[pos][m1] < *node_to && adjacency_list_matrix[pos][m1+1] > *node_to) {
                        pos1 = m1;
                        goto Exit;
                    }

                    if (adjacency_list_matrix[pos][m1] < *node_to) {
                        l1 = m1 + 1;
                    }
                    else {
                        r1 = m1 - 1;
                    }
                }
                cout<<"Node with same value already inserted"<<endl;
                pos1 = -2;
                goto Exit;
            }
            Exit:
            if(pos1 == -1){
                adjacency_list_matrix[pos].insert(adjacency_list_matrix[pos].begin()+1,*node_to);

            }
            else if (pos1 == -2){
                cout << "Node not inserted" << endl;
            }
            else{
                auto it = adjacency_list_matrix[pos].begin() +  (pos1 +1);
                adjacency_list_matrix[pos].insert(it, *node_to);

            }
        }

        cout << "Node linked" << endl;
    }


    void link_node_by_address(Node* node_from, Node* node_to) {
        int l = 0;
        int pos = 0;
        int r = size;
        while (l <= r){
            int m = l + (r-l)/2;

            if (adjacency_list_matrix[m][0] == *node_from)
                pos = m;

            if (adjacency_list_matrix[m][0] < *node_from)
                l = m + 1;

            else
                r = m - 1;
        }

        if(adjacency_list_matrix[pos].size() == 1){
            adjacency_list_matrix[pos].push_back(*node_to);

        }

        else{
            int pos1 = 0;
            int l1 = 0;
            int r1 = adjacency_list_matrix[pos].size();
            if(*node_to > adjacency_list_matrix[pos][r1-1]){
                pos1 = r1-1;
            }
            else if(*node_to < adjacency_list_matrix[pos][1]){
                pos1 = -1;
            }
            else{
                while (l1 <= r1) {
                    int m1 = l1 + (r1 - l1) / 2;
                    if (adjacency_list_matrix[pos][m1] < *node_to && adjacency_list_matrix[pos][m1+1] > *node_to) {
                        pos1 = m1;
                        goto Exit;
                    }

                    if (adjacency_list_matrix[pos][m1] < *node_to) {
                        l1 = m1 + 1;
                    }
                    else {
                        r1 = m1 - 1;
                    }
                }
                cout<<"Node with same value already inserted"<<endl;
                pos1 = -2;
                goto Exit;
            }
            Exit:
            if(pos1 == -1){
                adjacency_list_matrix[pos].insert(adjacency_list_matrix[pos].begin()+1,*node_to);

            }
            else if (pos1 == -2){
                cout << "Node not inserted" << endl;
            }
            else{
                auto it = adjacency_list_matrix[pos].begin() +  (pos1 +1);
                adjacency_list_matrix[pos].insert(it, *node_to);

            }
        }

        cout << "Node linked" << endl;
    }



//--------------------------------------------------------------------------


//--------------------------------------------------------------------------NODES LINKED TO NODE

    vector<Node> nodes_linked_to_node(Node* node){
        vector<Node> aux;
        int pos = 0;
        if(*node == adjacency_list_matrix[0][0]){
            aux = adjacency_list_matrix[0];
        }
        else if( *node == adjacency_list_matrix[size-1][0]){
            aux = adjacency_list_matrix[size-1];
        }
        else{
            int l = 0;
            int r = size;
            if (*node > adjacency_list_matrix[r - 1][0]) {
                pos = r - 1;
            } else if (*node < adjacency_list_matrix[0][0]) {
                pos = 0;
            } else {
                while (l <= r) {
                    int m = l + (r - l) / 2;
                    if (adjacency_list_matrix[m][0] == *node) {
                        pos = m;
                        goto Exit;
                    }

                    if (adjacency_list_matrix[m][0] < *node) {
                        l = m + 1;
                    } else {
                        r = m - 1;
                    }
                }
                Exit:
                aux = adjacency_list_matrix[pos];
            }
        }



        vector<Node> rtrn;
        for(int i =1; i < aux.size(); i++){
            rtrn.push_back(adjacency_list_matrix[pos][i]);
        }
        return rtrn;
    }

//--------------------------------------------------------------------------

//--------------------------------------------------------------------------SEARCH NODE

    int search_node_by_address_returns_position(Node* nodo){
        int pos = -1;
        int l = 0;
        int r = size;
        while (l <= r) {
            int m = l + (r - l) / 2;

            if (adjacency_list_matrix[m][0] == *nodo)
                pos = m;

            if (adjacency_list_matrix[m][0] < *nodo)
                l = m + 1;

            else
                r = m - 1;
        }

        if(pos > size-1){
            return -1;
        }
        else{
            return pos;
        }

    }
    int search_node_by_value_returns_position(typename node_type::n_type value){
        int pos = -1;
        int l = 0;
        int r = size;
        while (l <= r) {
            int m = l + (r - l) / 2;

            if (*adjacency_list_matrix[m][0] == value)
                pos = m;

            if (*adjacency_list_matrix[m][0] < value)
                l = m + 1;

            else
                r = m - 1;
        }

        if(pos > size-1){
            return -1;
        }
        else{
            return pos;
        }

    }

    Node* search_node_by_value_returns_address(typename node_type::n_type value){
        int p1 = search_node_by_value_returns_position(value);

        int pos = -1;
        int l = 0;
        int r = size;
        while (l <= r) {
            int m = l + (r - l) / 2;

            if (adjacency_list_matrix[m][0] == adjacency_list_matrix[p1][0])
                pos = m;

            if (adjacency_list_matrix[m][0] < adjacency_list_matrix[p1][0])
                l = m + 1;

            else
                r = m - 1;
        }

        if(pos > size-1){
            cout << "Node not found" << endl;
            return nullptr;
        }
        else{
            auto nodo = &adjacency_list_matrix[pos][0];
            return nodo;
        }

    }



//--------------------------------------------------------------------------

//--------------------------------------------------------------------------DELETE CONNECTIONS

    void delete_connection_with_adresses(Node* node_from, Node* node_to){
        int l = 0;
        int pos = 0;
        int r = size;
        while (l <= r){
            int m = l + (r-l)/2;

            if (adjacency_list_matrix[m][0] == *node_from)
                pos = m;

            if (adjacency_list_matrix[m][0] < *node_from)
                l = m + 1;

            else
                r = m - 1;
        }

        if(adjacency_list_matrix[pos].size() == 1){
            cout << "No nodes are elegible for deletion in this node_from" << endl;
            return;
        }

        else{
            int pos1 = 0;
            int l1 = 0;
            int r1 = adjacency_list_matrix[pos].size();
            if(*node_to > adjacency_list_matrix[pos][r1-1]){
                pos1 = r1-1;
            }
            else if(*node_to < adjacency_list_matrix[pos][1]){
                pos1 = -1;
            }
            else{
                while (l1 <= r1) {
                    int m1 = l1 + (r1 - l1) / 2;
                    if (adjacency_list_matrix[pos][m1] == *node_to) {
                        pos1 = m1;
                        goto Exit;
                    }

                    if (adjacency_list_matrix[pos][m1] < *node_to) {
                        l1 = m1 + 1;
                    }
                    else {
                        r1 = m1 - 1;
                    }
                }
                pos1 = -2;
                goto Exit;
            }
            Exit:
            if(pos1 == -2){
                adjacency_list_matrix[pos].erase(adjacency_list_matrix[pos].begin()+1);
                cout << "Node de-linked " << endl;
            }
            else if (pos1 == -1){
                cout << "No conection was deleted" << endl;
            }
            else{
                auto it = adjacency_list_matrix[pos].begin() +  (pos1);
                adjacency_list_matrix[pos].erase(it);
                cout << "Node de-linked " << endl;
            }
        }


    }

    void delete_connection_with_values(typename node_type::n_type node_from, typename node_type::n_type node_to){
        int l = 0;
        int pos = 0;
        int r = size;
        while (l <= r){
            int m = l + (r-l)/2;

            if (*adjacency_list_matrix[m][0] == node_from)
                pos = m;

            if (*adjacency_list_matrix[m][0] < node_from)
                l = m + 1;

            else
                r = m - 1;
        }

        if(adjacency_list_matrix[pos].size() == 1){
            cout << "No nodes are elegible for deletion in this node_from" << endl;
            return;
        }

        else{
            int pos1 = 0;
            int l1 = 0;
            int r1 = adjacency_list_matrix[pos].size();
            if(node_to > *adjacency_list_matrix[pos][r1-1]){
                pos1 = r1-1;
            }
            else if(node_to < *adjacency_list_matrix[pos][1]){
                pos1 = -1;
            }
            else{
                while (l1 <= r1) {
                    int m1 = l1 + (r1 - l1) / 2;
                    if (*adjacency_list_matrix[pos][m1] == node_to) {
                        pos1 = m1;
                        goto Exit;
                    }

                    if (*adjacency_list_matrix[pos][m1] < node_to) {
                        l1 = m1 + 1;
                    }
                    else {
                        r1 = m1 - 1;
                    }
                }
                pos1 = -2;
                goto Exit;
            }
            Exit:
            if(pos1 == -2){
                adjacency_list_matrix[pos].erase(adjacency_list_matrix[pos].begin()+1);
                cout << "Node de-linked " << endl;
            }
            else if (pos1 == -1){
                cout << "No conection was deleted" << endl;
            }
            else{
                auto it = adjacency_list_matrix[pos].begin() +  (pos1);
                adjacency_list_matrix[pos].erase(it);
                cout << "Node de-linked " << endl;
            }
        }


    }


//--------------------------------------------------------------------------


//--------------------------------------------------------------------------DELETE NODE


    void delete_node_by_value(typename node_type::n_type node) {

        if(node == *adjacency_list_matrix[0][0]){
            adjacency_list_matrix.erase(adjacency_list_matrix.begin());
            size--;
        }
        else if( node == *adjacency_list_matrix[size-1][0]){
            adjacency_list_matrix.erase(adjacency_list_matrix.end());
            size--;
        }
        else{
            int pos = 0;
            int l = 0;
            int r = size;
            if (node > *adjacency_list_matrix[r - 1][0]) {
                pos = r - 1;
            } else if (node < *adjacency_list_matrix[0][0]) {
                pos = 0;
            } else {
                while (l <= r) {
                    int m = l + (r - l) / 2;
                    if (*adjacency_list_matrix[m][0] == node) {
                        pos = m;
                        goto Exit;
                    }

                    if (*adjacency_list_matrix[m][0] < node) {
                        l = m + 1;
                    } else {
                        r = m - 1;
                    }
                }
                Exit:
                auto it = adjacency_list_matrix.begin() + pos;
                adjacency_list_matrix.erase(it);
                cout << "Node deleted" << endl;
                --size;
            }
        }

        for(int i =0; i < size; i++){
            if(node == *adjacency_list_matrix[i][1] && adjacency_list_matrix[i].size() > 1){
                auto aux = adjacency_list_matrix[i].begin() + 1;
                adjacency_list_matrix[i].erase(aux);
            }
            else if( node == *adjacency_list_matrix[size-1][adjacency_list_matrix[size-1].size()-1]){

                adjacency_list_matrix[i].erase(adjacency_list_matrix[i].end());
            }

            else{

                int pos = -1;
                int l = 0;
                int r = adjacency_list_matrix[i].size();
                if (node > *adjacency_list_matrix[i][r - 1]) {
                    pos = r - 1;
                } else if (node < *adjacency_list_matrix[i][1]) {
                    pos = 0;
                } else {
                    while (l <= r) {
                        int m = l + (r - l) / 2;
                        if (*adjacency_list_matrix[i][m] == node) {
                            pos = m;
                            goto Exit1;
                        }

                        if (*adjacency_list_matrix[i][m] < node) {
                            l = m + 1;
                        }else{
                            r = m - 1;
                        }
                    }

                    Exit1:
                    if(pos == -1){
                        cout<<"";
                    }
                    else{

                        auto it = adjacency_list_matrix[i].begin() + pos;
                        adjacency_list_matrix[i].erase(it);
                    }

                }

            }
        }

    }


    void delete_node_by_address(Node* node) {

        if(*node == adjacency_list_matrix[0][0]){
            adjacency_list_matrix.erase(adjacency_list_matrix.begin());
            size--;
        }
        else if( *node == adjacency_list_matrix[size-1][0]){
            adjacency_list_matrix.erase(adjacency_list_matrix.end());
            size--;
        }
        else{
            int pos = 0;
            int l = 0;
            int r = size;
            if (*node > adjacency_list_matrix[r - 1][0]) {
                pos = r - 1;
            } else if (*node < adjacency_list_matrix[0][0]) {
                pos = 0;
            } else {
                while (l <= r) {
                    int m = l + (r - l) / 2;
                    if (adjacency_list_matrix[m][0] == *node) {
                        pos = m;
                        goto Exit;
                    }

                    if (adjacency_list_matrix[m][0] < *node) {
                        l = m + 1;
                    } else {
                        r = m - 1;
                    }
                }
                Exit:
                auto it = adjacency_list_matrix.begin() + pos;
                adjacency_list_matrix.erase(it);
                cout << "Node deleted" << endl;
                --size;
            }
        }

        for(int i =0; i < size; i++){
            if(*node == adjacency_list_matrix[i][1] && adjacency_list_matrix[i].size() > 1){
                auto aux = adjacency_list_matrix[i].begin() + 1;
                adjacency_list_matrix[i].erase(aux);
            }
            else if( *node == adjacency_list_matrix[size-1][adjacency_list_matrix[size-1].size()-1]){

                adjacency_list_matrix[i].erase(adjacency_list_matrix[i].end());
            }

            else{

                int pos = -1;
                int l = 0;
                int r = adjacency_list_matrix[i].size();
                if (*node > adjacency_list_matrix[i][r - 1]) {
                    pos = r - 1;
                } else if (*node < adjacency_list_matrix[i][1]) {
                    pos = 0;
                } else {
                    while (l <= r) {
                        int m = l + (r - l) / 2;
                        if (adjacency_list_matrix[i][m] == *node) {
                            pos = m;
                            goto Exit1;
                        }

                        if (adjacency_list_matrix[i][m] < *node) {
                            l = m + 1;
                        }else{
                            r = m - 1;
                        }
                    }

                    Exit1:
                    if(pos == -1){
                        cout<<"";
                    }
                    else{

                        auto it = adjacency_list_matrix[i].begin() + pos;
                        adjacency_list_matrix[i].erase(it);
                    }

                }

            }
        }

    }


//--------------------------------------------------------------------------

//--------------------------------------------------------------------------SMALL FUNCTIONS
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
        cout<<endl;
    }



//--------------------------------------------------------------------------



};




template <typename data_type>
struct AdjacencyList<data_type, linked_list> { /// ADJACENCY LIST WITH LINKED LIST
    list< list<data_type> > adjacency_list;

};


#endif //ADJACENTLIST_ADJACENCYLIST_H
