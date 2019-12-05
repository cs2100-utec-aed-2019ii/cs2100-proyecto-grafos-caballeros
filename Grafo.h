//
// Created by rodri on 10/23/2019.
//

#ifndef CS2100_PROYECTO_GRAFOS_CABALLEROS_FRESH_AVOCADO_GRAFO_H
#define CS2100_PROYECTO_GRAFOS_CABALLEROS_FRESH_AVOCADO_GRAFO_H

#define dirigido true
#define no_dirigido false

#include <map>
#include "AdjacencyList.h"

template <typename node_type, bool graph_type>
class Grafo{};

template <typename node_type>
struct Grafo<node_type, dirigido> {
    AdjacencyList<node_type, 1> * graph =  nullptr;
    vector< pair<node_type,node_type>> * edges = new vector< pair<node_type,node_type>  >;
    explicit Grafo(AdjacencyList<node_type,1> * mygraph) {
        graph = mygraph;
        update_edges();
    }

    float calculate_distance(pair<node_type,node_type>  edge){
        float x1, y1, x2, y2;
        x1 = (edge.first.coordinate).x;
        x2 = (edge.second.coordinate).x;
        y1 = (edge.first.coordinate).y;
        y2 = (edge.second.coordinate).y;
        auto dist = sqrt( ((x2-x1)*(x2-x1)) + ((y2-y1)*(y2-y1)) );
//        cout << endl << "Distancia: " << dist << endl;
//        cout << "Entre nodos: " << edge.first.value << " x: " << x1 << " y: " << y1 << endl;
//        cout << "Entre nodos: " << edge.second.value << " x: " << x2 << " y: " << y2 << endl;
        return dist;

    }

    pair< pair<node_type,float>, pair<node_type,float> > min_max(node_type nodo) {
        auto nodoaux = graph->search_node_by_value_returns_address(nodo.value);
        auto nodos = graph->nodes_linked_to_node(nodoaux);
        float min = 0, max = 0;
        float aux = 0;
        float xmin, ymin, xmax, ymax;
        int imax = 0, imin = 0;
        bool switcho = false;
        for(int i = 0; i<nodos.size(); ++i){
            aux = calculate_distance(make_pair(nodo,nodos[i]));
            if(!switcho){
                min = max = aux;
                switcho = true;
            }
            if(aux > max){
                max = aux;
                xmax = nodos[i].coordinate.x;
                ymax = nodos[i].coordinate.y;
                imax = i;
            }
            else if( aux < min){
                min = aux;
                xmin = nodos[i].coordinate.x;
                ymin = nodos[i].coordinate.y;
                imin = i;
            }
        }

        cout << "Nodo" << nodo.value <<  " x: " << nodo.coordinate.x << " y: " << nodo.coordinate.y << endl;
        cout << "Min node value: " << nodos[imin].value << " x: " << xmin << " y: " << ymin << " distance: " << min <<endl;
        cout << "Max node value: " << nodos[imax].value << " x: " << xmax << " y: " << ymax << " distance: " << max << endl;
        return  make_pair( make_pair(nodos[imin], min), make_pair(nodos[imax], max) );

    }

    void update_edges() {
        edges->clear();
        for(int i =0; i< graph->size ; ++i){
            if( (graph->get_matrix()[i]).size() > 1 ){
                for(int j = 1; j < ((*graph->adjacency_list)[i].size()) ; ++j){
                    edges->push_back( make_pair((*graph->adjacency_list)[i][0] , (*graph->adjacency_list)[i][j]) );
                }
            }
        }
    }

    void insert_node(node_type* node) {
        graph->insert_node_by_address(node);
    }

    void link_node_by_address(node_type* node_from, node_type* node_to) {
        graph->link_node_by_address(node_from, node_to);
        update_edges();
    }

    void print_edges() {
        for(int i=0; i < edges->size(); i++) {
            cout << "Firsto " << (*edges)[i].first.get_value() << " " ;
            cout << "Secundo " << (*edges)[i].second.get_value() << endl;
        }
        cout << endl;
    }

    void print_graph() {
        graph->print_adjacency_list();
    }

    void delete_node(node_type* node) {
        graph->delete_node_by_address(node);
        update_edges();
    }

    float calculate_density() {
        float aux = edges->size();
        return (aux/(graph->size*(graph->size-1)));
    }

    void delete_connection_with_addresses(node_type* node_from, node_type* node_to) {
        graph->delete_connection_with_adresses(node_from, node_to);
        update_edges();
    }

    vector< node_type > a_star (node_type node_start, node_type node_end) {
        // FIXME: puede ser que el key del map se repita, hallar otra forma de hacer esto
        map < double, node_type > m; // map keys should be unique
        vector < node_type > visited_nodes;
        node_type curr_node = node_start;
        node_type prev_node = node_start;
        bool first_time = true;

        if (graph->nodes_linked_to_node(&node_start).size() == 0 || graph->node_grade_in_by_value(node_end.value) == 0) {
            cout << "\033[1;32mEl nodo origen y/o el nodo destino no tienen conexiones!\033[0m\n";
            return {};
        }

        while (curr_node != node_end) {
            vector < node_type > nodes_linked_to_curr_node = graph->nodes_linked_to_node(&curr_node);
            unsigned nodes_size = nodes_linked_to_curr_node.size();

            if (!first_time) {
                for (unsigned j = 0; j < nodes_size; ++j)
                    if (nodes_linked_to_curr_node[j] == prev_node)
                        nodes_linked_to_curr_node.erase(nodes_linked_to_curr_node.begin()+j);
            }

            nodes_size = nodes_linked_to_curr_node.size();

            first_time = false;

            for (unsigned i = 0; i < nodes_size; ++i) {
                node_type neighbor_node = nodes_linked_to_curr_node[i];
                double heuristic = graph->euclidean_distance(curr_node, neighbor_node) + graph->euclidean_distance(node_end, neighbor_node); // distancia euclideana al end node
                m.insert(pair<double, node_type>(heuristic, neighbor_node));
            }

            // get pair with the least heuristic value
            pair<double, node_type> shortest_path = *min_element(m.begin(), m.end());
            cout << "Min heuristic: " << shortest_path.first << " Node: " << shortest_path.second << endl;

            // clear the map for further use
            m.clear();

            prev_node = curr_node;
            visited_nodes.push_back(curr_node);
            curr_node = shortest_path.second;
            if (curr_node == node_end)
                visited_nodes.push_back(curr_node);
        }
        return visited_nodes;
    }

};

template <typename node_type>
struct Grafo<node_type, no_dirigido> {
    AdjacencyList<node_type, 1>* graph =  nullptr;
    vector< pair<node_type,node_type>>* edges = new vector< pair<node_type,node_type>  >;
    explicit Grafo(AdjacencyList<node_type, 1> * mygraph) {
        graph = mygraph;
        update_edges();
    }

    float calculate_distance(pair<node_type,node_type>  edge){
        float x1, y1, x2, y2;
        x1 = (edge.first.coordinate).x;
        x2 = (edge.second.coordinate).x;
        y1 = (edge.first.coordinate).y;
        y2 = (edge.second.coordinate).y;
        auto dist = sqrt( ((x2-x1)*(x2-x1)) + ((y2-y1)*(y2-y1)) );
//        cout << endl << "Distancia: " << dist << endl;
//        cout << "Entre nodos: " << edge.first.value << " x: " << x1 << " y: " << y1 << endl;
//        cout << "Entre nodos: " << edge.second.value << " x: " << x2 << " y: " << y2 << endl;
        return dist;

    }

    pair< pair<node_type,float>, pair<node_type,float> > min_max(node_type nodo) {
        auto nodoaux = graph->search_node_by_value_returns_address(nodo.value);
        auto nodos = graph->nodes_linked_to_node(nodoaux);
        float min = 0, max = 0;
        float aux = 0;
        float xmin, ymin, xmax, ymax;
        int imax = 0, imin = 0;
        bool switcho = false;
        for(int i = 0; i<nodos.size(); ++i){
            aux = calculate_distance(make_pair(nodo,nodos[i]));
            if(!switcho){
                min = max = aux;
                switcho = true;
            }
            if(aux > max){
                max = aux;
                xmax = nodos[i].coordinate.x;
                ymax = nodos[i].coordinate.y;
                imax = i;
            }
            else if( aux < min){
                min = aux;
                xmin = nodos[i].coordinate.x;
                ymin = nodos[i].coordinate.y;
                imin = i;
            }
        }

        cout << "Nodo" << nodo.value <<  " x: " << nodo.coordinate.x << " y: " << nodo.coordinate.y << endl;
        cout << "Min node value: " << nodos[imin].value << " x: " << xmin << " y: " << ymin << " distance: " << min <<endl;
        cout << "Max node value: " << nodos[imax].value << " x: " << xmax << " y: " << ymax << " distance: " << max << endl;
        return  make_pair( make_pair(nodos[imin], min), make_pair(nodos[imax], max) );

    }

    void update_edges() {
        edges->clear();
        for(int i =0; i< graph->size ; ++i){
            if( (graph->get_matrix()[i]).size() > 1 ){
                for(int j = 1; j < ((*graph->adjacency_list)[i].size()) ; ++j){
                    edges->push_back( make_pair((*graph->adjacency_list)[i][0] , (*graph->adjacency_list)[i][j]) );
                }
            }
        }
    }

    void insert_node(node_type* node) {
        graph->insert_node_by_address(node);
    }

    void link_node_by_address(node_type* node_from, node_type* node_to) {
        graph->link_node_by_address(node_from, node_to);
        update_edges();
    }

    void print_edges() {
        for(int i=0; i < edges->size(); i++) {
            cout << "Firsto " << (*edges)[i].first.get_value() << " " ;
            cout << "Secundo " << (*edges)[i].second.get_value() << endl;
        }
        cout << endl;
    }

    void print_graph() {
        graph->print_adjacency_list();
    }

    void delete_node(node_type* node) {
        graph->delete_node_by_address(node);
        update_edges();
    }

    float calculate_density() {
        float aux = edges->size();
        return (aux/(graph->size*(graph->size-1)));
    }

    void delete_connection_with_addresses(node_type* node_from, node_type* node_to) {
        graph->delete_connection_with_adresses(node_from, node_to);
        update_edges();
    }

    vector< node_type > a_star (node_type node_start, node_type node_end) {
        // FIXME: puede ser que el key del map se repita, hallar otra forma de hacer esto
        map < double, node_type > m; // map keys should be unique
        vector < node_type > visited_nodes;
        node_type curr_node = node_start;
        node_type prev_node = node_start;
        bool first_time = true;

        if (graph->nodes_adjacent_to_node(&node_start).size() == 0 || graph->nodes_adjacent_to_node(&node_end).size() == 0) {
            cout << "\033[1;32mEl nodo origen y/o el nodo destino no tienen conexiones!\033[0m\n";
            return {};
        }

        while (curr_node != node_end) {
            vector < node_type > nodes_linked_to_curr_node = graph->nodes_adjacent_to_node(&curr_node);
            unsigned nodes_size = nodes_linked_to_curr_node.size();

            if (!first_time) {
                for (unsigned j = 0; j < nodes_size; ++j)
                    if (nodes_linked_to_curr_node[j] == prev_node)
                        nodes_linked_to_curr_node.erase(nodes_linked_to_curr_node.begin()+j);
            }

            nodes_size = nodes_linked_to_curr_node.size();

            first_time = false;

            for (unsigned i = 0; i < nodes_size; ++i) {
                node_type neighbor_node = nodes_linked_to_curr_node[i];
                double heuristic = graph->euclidean_distance(curr_node, neighbor_node) + graph->euclidean_distance(node_end, neighbor_node); // distancia euclideana al end node
                m.insert(pair<double, node_type>(heuristic, neighbor_node));
            }

            // get pair with the least heuristic value
            pair<double, node_type> shortest_path = *min_element(m.begin(), m.end());
            cout << "Min heuristic: " << shortest_path.first << " Node: " << shortest_path.second << endl;

            // clear the map for further use
            m.clear();

            prev_node = curr_node;
            visited_nodes.push_back(curr_node);
            curr_node = shortest_path.second;
            if (curr_node == node_end)
                visited_nodes.push_back(curr_node);
        }
        return visited_nodes;
    }
};


#endif //CS2100_PROYECTO_GRAFOS_CABALLEROS_FRESH_AVOCADO_GRAFO_H