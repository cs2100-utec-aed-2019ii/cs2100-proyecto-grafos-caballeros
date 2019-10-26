//
// Created by Gabriel Spranger Rojas on 10/26/19.
//

#ifndef CS2100_PROYECTO_GRAFOS_CABALLEROS_BENJAMINDIAZGARCIA_GRAFO_H
#define CS2100_PROYECTO_GRAFOS_CABALLEROS_BENJAMINDIAZGARCIA_GRAFO_H

#define dirigido true
#define no_dirigido false

#include "AdjacencyList.h"

template <typename node_type, bool graph_type>
class Grafo{};

template <typename node_type>
struct Grafo<node_type, dirigido> {

    AdjacencyList<node_type,dirigido> * graph =  nullptr;


    vector< pair<node_type,node_type>  > * edges = new vector< pair<node_type,node_type>  >;


    explicit Grafo(AdjacencyList<node_type,dirigido>* mygraph) {

        graph = mygraph;
        update_edges();
    }

    void update_edges(){
        edges->clear();
        for(int i =0; i< graph->size ; ++i){
            if( (graph->get_matrix()[i]).size() > 1 ){
                for(int j = 1; j < ((*graph->adjacency_list)[i].size()) ; ++j){
                    edges->push_back( make_pair((*graph->adjacency_list)[i][0] , (*graph->adjacency_list)[i][j]) );
                }
            }
        }
    }

    void insert_node(node_type* node){
        graph->insert_node_by_address(node);
    }

    void link_node_by_address(node_type* node_from, node_type* node_to ){
        graph->link_node_by_address(node_from, node_to);
        update_edges();
    }

    void print_edges(){
        for(int i=0; i < edges->size(); i++){
            cout << "Firsto " << (*edges)[i].first.get_value() << " " ;
            cout << "Secundo " << (*edges)[i].second.get_value() << endl;
        }
        cout << endl;
    }

    void print_graph(){
        graph->print_adjacency_list();
    }

    void delete_node(node_type* node){
        graph->delete_node_by_address(node);
        update_edges();
    }

    float calculate_density(){
        float aux = edges->size();
        return (aux/(graph->size*(graph->size-1)));
    }

    void delete_connection_with_addresses(node_type* node_from, node_type* node_to){
        graph->delete_connection_with_adresses(node_from, node_to);
        update_edges();
    }


};

template <typename node_type>
class Grafo<node_type, no_dirigido> {

};


#endif //CS2100_PROYECTO_GRAFOS_CABALLEROS_BENJAMINDIAZGARCIA_GRAFO_H
