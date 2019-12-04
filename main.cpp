#define _node_type int
#define generic_node Node<_node_type, Coordinate2D>
#define _dirigido true
#define GL_SILENCE_DEPRECATION

#include "Lectura.h"

#define adjacency_list_with_node_int_2D_and_vectorized AdjacencyList<generic_node, vectorized>

using namespace std;
// Prototyping OpenGL functions
GLvoid initGL();
GLvoid key_pressed(unsigned char key, int x, int y);
GLvoid mouse_click(int button, int state, int x, int y);
GLvoid window_display();
GLvoid window_reshape(GLsizei width, GLsizei height);
GLvoid draw_graph(int argc, char** argv);
void show_menu();
void print_with_color(const string& color, const string& str);

AdjacencyList<generic_node, vectorized>* al; // adjacency list object
Lectura<generic_node, Coordinate2D, _dirigido>* lector; // read and save (.vtk format)
vector<vector<generic_node>> adj_mat; // adjacency list
Grafo<generic_node, _dirigido>* graph;

/// Booleans that help with the different modes in the menu

// Minimum Spanning Trees
bool draw_MST_Kruskal = false;
bool draw_MST_Prim = false;

// Breadth First Search and Depth First Search
bool draw_BFS_path = false;
bool draw_DFS_path = false;
bool insert_mode = false;
pair<generic_node, generic_node> nodes_for_BFS_or_DFS;
short nodes_selected_for_DFS = 0;
short nodes_selected_for_BFS = 0;

// Degree of node
bool degree_of_node_mode = false; bool degree_of_incidence = false; bool degree_of_outcidence = false;

// Neighborhood of node
bool neighborhood_mode = false; bool permission_to_draw_neighborhood = false;

// Validation of neighborhood
bool validate_neighborhood = false;

// Dijkstra and A*
bool a_star_mode = false;
bool dijkstra_mode = false;
bool origin_node_selected = false;
bool destination_node_selected = false;
bool draw_a_star_path = false;
bool draw_dijkstra_path = false;
vector<generic_node> nodes_for_a_star;
vector<generic_node> a_star_path;
vector<generic_node> nodes_for_dijkstra;

// Iterator
bool iterator_mode = false;
vector<generic_node> linked_ones;
vector<generic_node> iterator_path;

// Insertion
vector<generic_node> nodes_to_link;

// ?
vector<generic_node> possible_neighbors;

short possible_neighbors_selected = 0;
generic_node iterating_node;

int main(int argc, char** argv) {

    srand(time(nullptr));

    lector = new Lectura<generic_node, Coordinate2D, _dirigido>();

    /// Read adjacency list from .vtk
    al = lector->cargar_datos("100points.vtk");
    al->print_adjacency_list();

    /// Creando el grafo
    graph = new Grafo<generic_node, _dirigido>(al);

    adj_mat = *(al->adjacency_list);

    /// Dibujar grafo
    draw_graph(argc, argv);

    cout << "Programa terminando..." << endl;

    delete al;
    delete graph;
    delete lector;

    return 0;
}

void show_menu() {
    print_with_color("34", "--------------------Grapho--------------------");
    print_with_color("37", "Hacer click en un nodo para eliminarlo.");
    print_with_color("37", "1. Insertar nodo.");
    print_with_color("37", "3. Grado de un nodo.");
    print_with_color("37", "4. Comprobar si el grafo es conexo.");
    print_with_color("37", "5. Comprobar si el grafo es bipartito.");
    print_with_color("37", "6. Comprobar si el grafo es denso.");
    print_with_color("37", "7. MST de Kruskal.");
    print_with_color("37", "8. MST de Prim.");
    print_with_color("37", "a. A*");
    print_with_color("37", "s. Depth First Search.");
    print_with_color("37", "b. Mostrar vecindad de un nodo.");
    print_with_color("37", "c. Validar la vecindad de un nodo.");
    print_with_color("37", "d. Dijkstra.");
    print_with_color("37", "i. Iterar.");
    print_with_color("37", "q. Terminar el programa.");
}

GLvoid initGL() {
    glClearColor(RED, GREEN, BLUE, ALPHA);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
}

generic_node new_node = generic_node();
generic_node selected_node_for_neighborhood = generic_node();
vector<generic_node> nodes_linked_to_selected_node_for_neighborhood;

GLvoid key_pressed(unsigned char key, int x, int y) {
    switch (key) {
        case '1': {
            if (!insert_mode) {
                insert_mode = true;
                print_with_color("32", "Modo insertar habilitado.");
                _node_type new_value;
                do {
                    cout << "Valor del nodo: ";
                    cin >> new_value;
                } while (al->search_node_by_value_returns_position(new_value) != -1);

                new_node.value = new_value;
                new_node.coordinate.x = (float)x;
                new_node.coordinate.y = float(WINDOW_HEIGHT-y);
                cout << "New node created: \n";
                cout << new_node << endl;
                al->print_adjacency_list();
                al->insert_node_by_address(&new_node);
                adj_mat = *(al->adjacency_list); // vital
                al->print_adjacency_list();
                cout << "Haga click en los nodos que usted desea conectar con el nodo insertado.\n";
                cout << "Presione 2 cuando haya terminado.\n";
                glutPostRedisplay();
                break;
            }
            break;
        }
        case '2': {
            if (insert_mode) {
                for (auto& n : nodes_to_link) {
                    al->link_node_by_address(&new_node, &n);
                }
                adj_mat = *(al->adjacency_list); // update reference to adjacency list
                al->print_adjacency_list();
                nodes_to_link.clear(); // reset auxiliar vector
                insert_mode = false; // turn off insert mode
                print_with_color("32", "Modo insertar deshabilitado.");
                glutPostRedisplay(); // re-draw window
                break;
            }
            break;
        }
        case '3': {
            if (!degree_of_node_mode) {
                degree_of_node_mode = true;
                print_with_color("32", "Modo grado de un nodo activado. Presione 3 para desactivar.");

                if (_dirigido) {
                    short degree_type;
                    do {
                        cout << "¿Grado de incidencia o outcidencia? [1/2] : ";
                        cin >> degree_type;
                    } while (degree_type != 1 && degree_type != 2);

                    if (degree_type == 1) {
                        degree_of_incidence = true;
                    } else {
                        degree_of_outcidence = true;
                    }
                }

                cout << "Seleccione un nodo para descubrir su grado.\n";
                al->print_adjacency_list();
            } else {
                degree_of_node_mode = false;
                degree_of_incidence = false;
                degree_of_outcidence = false;
                print_with_color("32", "Modo grado de un nodo desactivado.");
            }
            break;
        }
        case '4': {
            // TODO: conexo
            cout << "Es conexo? Nunca lo sabremos...\n";
            break;
        }
        case '5': {
            // TODO: bipartito
            cout << "Es bipartito? Nunca lo sabremos...\n";
            break;
        }
        case '6': {
            if (graph->calculate_density() >= 0.9f) { // grafo es denso si #nodos es mayor al 90%
                cout << "El grafo es denso!\n";
            } else {
                cout << "El grafo no es denso!\n";
            }
            break;
        }
        case '7': { // MST Kruskal: pintar el minimum spanning tree de rojo
            if (!draw_MST_Kruskal) {
                print_with_color("32", "Modo MST Kruskal habilitado. Presione 7 para desactivar.");
                draw_MST_Kruskal = true;
                // refresh window so that path can be drawn
                glutPostRedisplay();
            } else {
                draw_MST_Kruskal = false;
                print_with_color("32", "Modo MST Kruskal deshabilitado.");
                // refresh window so that path can be erased from the window
                glutPostRedisplay();
            }
            break;
        }
        case '8': { // MST Prim: pintar el minimum spanning tree de rojo
            if (!draw_MST_Prim) {
                print_with_color("32", "Modo MST Prim habilitado. Presione 8 para desactivar.");
                draw_MST_Prim = true;
                // refresh window so that path can be drawn
                glutPostRedisplay();
            } else {
                draw_MST_Prim = false;
                print_with_color("32", "Modo MST Prim deshabilitado.");
                // refresh window so that path can be erased from the window
                glutPostRedisplay();
            }
            break;
        }
        case '9': { // BFS: mostrar la secuencia de nodos
            if (!draw_BFS_path) {
                print_with_color("32", "Modo BFS habilitado. Presione 9 para desactivar.");
                draw_BFS_path = true;
                // refresh window so that path can be drawn
                glutPostRedisplay();
            } else {
                draw_BFS_path = false;
                nodes_selected_for_BFS = 0;
                print_with_color("32", "Modo BFS deshabilitado.");
                // refresh window so that path can be erased from the window
                glutPostRedisplay();
            }
            break;
        }
        case 's': { // DFS: mostrar la secuencia de nodos
            if (!draw_DFS_path) {
                print_with_color("32", "Modo DFS habilitado. Presione 'a' para desactivar.");
                draw_DFS_path = true;
                // refresh window so that path can be drawn
                glutPostRedisplay();
            } else {
                draw_DFS_path = false;
                nodes_selected_for_DFS = 0;
                print_with_color("32", "Modo DFS deshabilitado.");
                // refresh window so that path can be erased from the window
                glutPostRedisplay();
            }
            break;
        }
        case 'b': {
            if (!neighborhood_mode) {
                // TODO: colocar restriccion de vecindad en cuanto al promedio del weight de las aristas vecinas
                neighborhood_mode = true;
                print_with_color("32", "Modo vecindad de un nodo habilitado. Presione 'b' para deshabilitarlo.");
                print_with_color("32", "Seleccione un nodo para descubrir su vecindario.");
            } else {
                print_with_color("32", "Modo vecindad deshabilitado.");
                permission_to_draw_neighborhood = false;
                neighborhood_mode = false;
                nodes_linked_to_selected_node_for_neighborhood.clear();
                glutPostRedisplay();
            }
            break;
        }
        case 'c': {
            if (!validate_neighborhood) {
                validate_neighborhood = true;
                print_with_color("32", "Modo validar vecindad habilitado. Presione 'c' para deshabiliarlo.");
            } else {
                print_with_color("32", "Modo validar vecindad deshabilitado.");
                possible_neighbors_selected = 0;
                validate_neighborhood = false;
                possible_neighbors.clear(); // delete all items in vector
            }
            break;
        }

        case 'a': {
            // TODO: no se puede con un nodo que no tiene nodos adyacentes
            if (!a_star_mode) {
                a_star_mode = true;
                print_with_color("32", "Modo A* activado.");
                print_with_color("32", "Seleccione el nodo origen con el click izquierdo.");
            } else {
                a_star_mode = false;
                origin_node_selected = false;
                destination_node_selected = false;
                draw_a_star_path = false;
                nodes_for_a_star.clear();
                a_star_path.clear();
                glutPostRedisplay(); // refresh the window so that the path drawn dissapears
                print_with_color("32", "Modo A* desactivado.");
            }
            break;
        }

        case 'd': {
            // TODO: no se puede con un nodo que no tiene nodos adyacentes
            if (!dijkstra_mode) {
                dijkstra_mode = true;
                print_with_color("32", "Modo Dijkstra activado!");
                print_with_color("32", "Seleccione el nodo origen con el click izquierdo.");
            } else {
                dijkstra_mode = false;
                origin_node_selected = false;
                destination_node_selected = false;
                draw_dijkstra_path = false;
                nodes_for_dijkstra.clear();
                glutPostRedisplay(); // refresh the window so that the path drawn dissapears
                print_with_color("32", "Modo Dijkstra desactivado!");
            }
            break;
        }

        case 'i': {
            if (!iterator_mode) {
                print_with_color("32", "Modo iteración activado.");
                iterator_mode = true;
                iterating_node = adj_mat[int(rand()%al->size)][0]; // iterator begins at random node
                linked_ones = al->nodes_adjacent_to_node(&iterating_node);
                iterator_path.push_back(iterating_node);
                glutPostRedisplay();
                cout << "Node selected for iterating: " << iterating_node << endl;
            } else {
                print_with_color("32", "Modo iteración desactivado.");
                iterator_mode = false;
                iterator_path.clear();
                glutPostRedisplay();
            }
            break;
        }

        case 'q': {
            char respuesta;
            print_with_color("31", "¿Quieres guardar el grafo en disco? [y/n]");
            cin >> respuesta;

            if (respuesta == 'y') {
                //al->delete_node_by_value(4);
                lector->save_adjacency_list(al);
                print_with_color("31", "Grafo guardado como nodos_antiguos.vtk.");
            }

            delete lector;
            delete al;
            exit(1);
        }
        default:
            print_with_color("36", "Esa opción no existe!");
            break;
    }
}

GLvoid key_press(int key, int x, int y) {
    if (key == GLUT_KEY_LEFT) {
        cout << "Left arrow key pressed!\n";
        if (iterator_mode) { // move to max edge
            iterating_node = graph->min_max(iterating_node).second.first;
            iterator_path.push_back(iterating_node);
            linked_ones = al->nodes_adjacent_to_node(&iterating_node);
            glutPostRedisplay(); // new starting node, redraw screen
        }
    }

    if (key == GLUT_KEY_RIGHT) {
        cout << "Right arrow key pressed!\n";
        if (iterator_mode) { // move to min edge
            iterating_node = graph->min_max(iterating_node).first.first;
            iterator_path.push_back(iterating_node);
            linked_ones = al->nodes_adjacent_to_node(&iterating_node);
            glutPostRedisplay(); // new starting node, redraw screen
        }
    }
}

GLvoid mouse_click(int button, int state, int x, int y) {

    if (state == GLUT_DOWN && button == GLUT_RIGHT_BUTTON) {
        cout << "Right click at x: " << x << " y: " << WINDOW_HEIGHT-y << endl;
        if (a_star_mode) {
            if (!destination_node_selected) {
                for (unsigned i = 0; i < al->size; ++i) {
                    generic_node _node = adj_mat[i][0];
                    if (_node.coordinate.x == float(x) && _node.coordinate.y == float(WINDOW_HEIGHT-y)) {
                        cout << "Nodo destino seleccionado!\n";
                        cout << _node << endl;
                        nodes_for_a_star.push_back(_node);
                        destination_node_selected = true;
                        draw_a_star_path = true;
                        a_star_path = graph->a_star(nodes_for_a_star[0], nodes_for_a_star[1]);
                        print_with_color("31", "Dibujando el camino de A*...");
                        glutPostRedisplay(); // refresh window so that the path is drawn
                        cout << "Presione 'a' cuando haya terminado de visualizar el camino.\n";
                    }
                }
            }
        } else if (dijkstra_mode) {
            // TODO: draw dijkstra
            if (!destination_node_selected) {
                for (unsigned i = 0; i < al->size; ++i) {
                    generic_node _node = adj_mat[i][0];
                    if (_node.coordinate.x == float(x) && _node.coordinate.y == float(WINDOW_HEIGHT-y)) {
                        cout << "Nodo destino seleccionado!\n";
                        cout << _node << endl;
                        nodes_for_dijkstra.push_back(_node);
                        destination_node_selected = true;
                        draw_dijkstra_path = true;
                        glutPostRedisplay(); // refresh window so that the path is drawn
                        print_with_color("31", "Dibujando el camino de Dijkstra...");
                        cout << "Presione 'd' cuando haya terminado de visualizar el camino.\n";
                    }
                }
            }
        }
    }

    if (state == GLUT_DOWN && button == GLUT_LEFT_BUTTON) {
        cout << "Left click at x: " << x << " y: " << WINDOW_HEIGHT-y << endl;
        // se van a seleccionar dos nodos para buscar su camino
        // no interpretar esa selección como un delete
        if (insert_mode) {
            for (unsigned i = 0; i < al->size; ++i) {
                generic_node _node = adj_mat[i][0];
                if (_node.coordinate.x == float(x) && _node.coordinate.y == float(WINDOW_HEIGHT-y)) {
                    cout << "Nodo seleccionado!\n";
                    cout << _node << endl;
                    nodes_to_link.push_back(_node);
                }
            }
        } else if (a_star_mode) {
            if (!origin_node_selected) {
                for (unsigned i = 0; i < al->size; ++i) {
                    generic_node _node = adj_mat[i][0];
                    if (_node.coordinate.x == float(x) && _node.coordinate.y == float(WINDOW_HEIGHT-y)) {
                        cout << "Nodo origen seleccionado!\n";
                        cout << _node << endl;
                        cout << "Seleccione el nodo destino con el click derecho.\n";
                        nodes_for_a_star.push_back(_node);
                        origin_node_selected = true;
                    }
                }
            }
        } else if (dijkstra_mode) {
            if (!origin_node_selected) {
                for (unsigned i = 0; i < al->size; ++i) {
                    generic_node _node = adj_mat[i][0];
                    if (_node.coordinate.x == float(x) && _node.coordinate.y == float(WINDOW_HEIGHT-y)) {
                        cout << "Nodo origen seleccionado!\n";
                        cout << _node << endl;
                        cout << "Seleccione el nodo destino con el click derecho.\n";
                        nodes_for_dijkstra.push_back(_node);
                        origin_node_selected = true;
                    }
                }
            }
        } else if (validate_neighborhood) {
            for (unsigned i = 0; i < al->size; ++i) {
                generic_node _node = adj_mat[i][0];
                if (_node.coordinate.x == float(x) && _node.coordinate.y == float(WINDOW_HEIGHT-y)) {
                    cout << "Nodo seleccionado!\n";
                    cout << _node << endl;
                    ++possible_neighbors_selected;
                    possible_neighbors.push_back(_node);
                    if (possible_neighbors_selected == 2) { // two nodes were selected
                        generic_node starting_node = possible_neighbors[0];
                        generic_node neighbor_to_evaluate = possible_neighbors[1];
                        if (al->is_neighbor(&starting_node, &neighbor_to_evaluate)) {
                            cout << "Son adyacentes!\n";
                            return;
                        } else {
                            cout << "No son adyacentes!\n";
                            return;
                        }
                    }
                }
            }
        } else if (neighborhood_mode) {
            for (unsigned i = 0; i < al->size; ++i) {
                generic_node _node = adj_mat[i][0];
                if (_node.coordinate.x == float(x) && _node.coordinate.y == float(WINDOW_HEIGHT-y)) {
                    cout << "Nodo seleccionado!\n";
                    cout << _node << endl;
                    selected_node_for_neighborhood = _node;
                    nodes_linked_to_selected_node_for_neighborhood = al->nodes_adjacent_to_node(&_node);
                    permission_to_draw_neighborhood = true;
                    glutPostRedisplay();
                }
            }
        } else if (degree_of_node_mode) {
            for (unsigned i = 0; i < al->size; ++i) {
                generic_node _node = adj_mat[i][0];
                if (_node.coordinate.x == float(x) && _node.coordinate.y == float(WINDOW_HEIGHT-y)) {
                    if (_dirigido) {
                        if (degree_of_incidence) {
                            cout << _node << endl;
                            cout << "Degree of incidence: " << al->node_grade_in_by_value(_node.value) << endl;
                        } else if (degree_of_outcidence) {
                            cout << _node << endl;
                            cout << "Degree of outcidence: " << al->node_grade_out_by_value(_node.value) << endl;
                        }
                    } else {
                        cout << _node << endl;
                        cout << "Degree of node " << al->node_grade_out_by_value(_node.value) << endl;
                    }
                }
            }
        } else if (draw_DFS_path) {
            // user needs to select two nodes
            if (nodes_selected_for_DFS == 0) {
                for (unsigned i = 0; i < al->size; ++i) {
                    generic_node _node = adj_mat[i][0];
                    if (_node.coordinate.x == float(x) && _node.coordinate.y == float(WINDOW_HEIGHT-y)) {
                        cout << "Node selected DFS!" << endl;
                        nodes_for_BFS_or_DFS.first = _node;
                        ++nodes_selected_for_DFS;
                        glutPostRedisplay(); // refresh window to see the change
                        return;
                    }
                }
            } else if (nodes_selected_for_DFS == 1) {
                for (unsigned i = 0; i < al->size; ++i) {
                    generic_node _node = adj_mat[i][0];
                    if (_node.coordinate.x == float(x) && _node.coordinate.y == float(WINDOW_HEIGHT-y)) {
                        cout << "Node selected DFS!" << endl;
                        nodes_for_BFS_or_DFS.second = _node;
                        ++nodes_selected_for_DFS;
                        glutPostRedisplay(); // refresh window to see the change
                        return;
                    }
                }
            }
        } else if (draw_BFS_path) {
            if (nodes_selected_for_BFS == 0) {
                for (unsigned i = 0; i < al->size; ++i) {
                    generic_node _node = adj_mat[i][0];
                    if (_node.coordinate.x == float(x) && _node.coordinate.y == float(WINDOW_HEIGHT-y)) {
                        cout << "Node selected for BFS!" << endl;
                        nodes_for_BFS_or_DFS.first = _node;
                        ++nodes_selected_for_BFS;
                        glutPostRedisplay(); // refresh window to see the change
                        return;
                    }
                }
            } else if (nodes_selected_for_BFS == 1) {
                for (unsigned i = 0; i < al->size; ++i) {
                    generic_node _node = adj_mat[i][0];
                    if (_node.coordinate.x == float(x) && _node.coordinate.y == float(WINDOW_HEIGHT-y)) {
                        cout << "Node selected for BFS!" << endl;
                        nodes_for_BFS_or_DFS.second = _node;
                        ++nodes_selected_for_BFS;
                        glutPostRedisplay(); // refresh window to see the change
                        return;
                    }
                }
            }
        } else { // interpretar click en nodo como un delete request
            unsigned s = al->size;
            for (unsigned i = 0; i < s; ++i) {
                generic_node _node = adj_mat[i][0];
                if (_node.coordinate.x == (float)x && _node.coordinate.y == float(WINDOW_HEIGHT-y)) {
                    cout << "NODE ABOUT TO BE DELETED HAS VALUE: " << _node.value << endl;
                    al->delete_node_by_value(_node.value);
                    cout << "NODE DELETED" << endl;
                    adj_mat = *(al->adjacency_list);
                    al->print_adjacency_list();
                    glutPostRedisplay(); // refresh window to see the change
                    return;
                }
            }
        }

    }
}

GLvoid window_display() {
    glClear(GL_COLOR_BUFFER_BIT);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0.0f, WINDOW_WIDTH, 0.0f, WINDOW_HEIGHT, 0.0f, Z_DIST);

    /// Esto dibuja los nodos O(V)

    for (unsigned i = 0; i < al->size; ++i) {
        glPushMatrix();
            glTranslatef(adj_mat[i][0].coordinate.x, adj_mat[i][0].coordinate.y, 0);
            glColor3f(SPHERE_RED, SPHERE_GREEN, SPHERE_BLUE);
            glutWireSphere(NODE_RADIUS, SPHERE_SLICES, SPHERE_STACKS);
        glPopMatrix();
    }

    /// Esto dibuja las aristas O(V^2)

    for (unsigned i = 0; i < al->size; ++i) {
        generic_node starting_node = adj_mat[i][0]; // get NODE
        vector<generic_node> nodes = al->nodes_linked_to_node(&starting_node); // get nodes linked to NODE
        for (auto& n : nodes) { // iterate through nodes linked to NODE
            glBegin(GL_LINES);
                glVertex2f(starting_node.coordinate.x, starting_node.coordinate.y);
                glVertex2f(n.coordinate.x, n.coordinate.y);
            glEnd();
        }
    }

    if (draw_BFS_path) {
        if (nodes_selected_for_BFS == 2) { // pair is complete, we can access both nodes
            // TODO:  call al->bfs(nodes_selected.first, nodes_selected.second)
            glBegin(GL_LINES);
                glColor3f(1.0f, 0.0f, 0.0f);
                glVertex2f(nodes_for_BFS_or_DFS.first.coordinate.x, nodes_for_BFS_or_DFS.first.coordinate.y);
                glVertex2f(nodes_for_BFS_or_DFS.second.coordinate.x, nodes_for_BFS_or_DFS.second.coordinate.y);
            glEnd();
        }
    } else if (iterator_mode) {
        // iterating node's path
        if (iterator_path.size() >= 2) {
            for (unsigned i = 0; i < iterator_path.size()-1; ++i) {
                glBegin(GL_LINES);
                    glColor3f(1.0f, 0.0f, 0.0f);
                    glVertex2f(iterator_path[i].coordinate.x, iterator_path[i].coordinate.y);
                    glVertex2f(iterator_path[i+1].coordinate.x, iterator_path[i+1].coordinate.y);
                glEnd();
            }
        }
        // draw neighbors
        for (const auto& n : linked_ones) {
            cout << "LINKED ONE: " << n << endl;
            glPushMatrix();
                glTranslatef(n.coordinate.x, n.coordinate.y, 0);
                glColor3f(1.0f, 0.0f, 1.0f); // magenta
                glutWireSphere(4, SPHERE_SLICES, SPHERE_STACKS); // changed node radius
            glPopMatrix();
        }
        // draw iterating node
        glTranslatef(iterating_node.coordinate.x, iterating_node.coordinate.y, 0);
        glColor3f(1.0f, 0.0f, 0.0f); // magenta
        glutWireSphere(4, SPHERE_SLICES, SPHERE_STACKS); // changed node radius
    } else if (draw_a_star_path) {
        // TODO: llamar la function que hace A* y dibujar el camino
        unsigned a_star_path_size = a_star_path.size();
        if (a_star_path_size >= 2) {
            for (unsigned i = 0; i < a_star_path_size-1; ++i) {
                glBegin(GL_LINES);
                glColor3f(1.0f, 0.0f, 0.0f);
                glVertex2f(a_star_path[i].coordinate.x, a_star_path[i].coordinate.y);
                glVertex2f(a_star_path[i+1].coordinate.x, a_star_path[i+1].coordinate.y);
                glEnd();
            }
        }
    } else if (draw_dijkstra_path) {
        // TODO: llamar la function que hace Dijkstra y dibujar el camino
        glBegin(GL_LINES);
            glColor3f(1.0f, 1.0f, 0.0f);
            glVertex2f(nodes_for_dijkstra[0].coordinate.x, nodes_for_dijkstra[0].coordinate.y);
            glVertex2f(nodes_for_dijkstra[1].coordinate.x, nodes_for_dijkstra[1].coordinate.y);
        glEnd();
    } else if (permission_to_draw_neighborhood) {
        for (auto& n : nodes_linked_to_selected_node_for_neighborhood) {
            glPushMatrix();
                // z = 1 para que se dibuje encima
                glTranslatef(n.coordinate.x, n.coordinate.y, 0);
                glColor3f(1.0f, 0.0f, 0.0f); // red
                glutWireSphere(NODE_RADIUS, SPHERE_SLICES, SPHERE_STACKS);
            glPopMatrix();
        }
    } else if (draw_DFS_path) {
        if (nodes_selected_for_DFS == 2) { // pair is complete, we can access both nodes
            // TODO:  call al->dfs(nodes_selected.first, nodes_selected.second)
            glBegin(GL_LINES);
                glColor3f(1.0f, 1.0f, 0.0f);
                glVertex2f(nodes_for_BFS_or_DFS.first.coordinate.x, nodes_for_BFS_or_DFS.first.coordinate.y);
                glVertex2f(nodes_for_BFS_or_DFS.second.coordinate.x, nodes_for_BFS_or_DFS.second.coordinate.y);
            glEnd();
        }
    } else if (draw_MST_Prim) {
        // TODO: call al->mst_prim()
        glPushMatrix();
            glTranslatef(WINDOW_WIDTH/2, WINDOW_HEIGHT/2, 0);
            glColor3f(1.0f, 0.0f, 1.0f);
            glutWireSphere(50, SPHERE_SLICES, SPHERE_STACKS);
        glPopMatrix();
    } else if (draw_MST_Kruskal) {
        // TODO: call al->mst_prim()
        glPushMatrix();
            glTranslatef(WINDOW_WIDTH/2, WINDOW_HEIGHT/2, 0);
            glColor3f(1.0f, 1.0f, 1.0f);
            glutWireSphere(50, SPHERE_SLICES, SPHERE_STACKS);
        glPopMatrix();
    }

    glutSwapBuffers();
    glFlush();
}

GLvoid window_reshape(GLsizei width, GLsizei height) {
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0.0f, width, 0.0f, height, 0.0f, Z_DIST);
    WINDOW_WIDTH = width;
    WINDOW_HEIGHT = height;
    glMatrixMode(GL_MODELVIEW);
}

GLvoid draw_graph(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);
    glutInitWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT); // define window size
    glutInitWindowPosition(WINDOW_X_POS, WINDOW_Y_POS); // donde se va a renderizar la pantalla
    glutCreateWindow("Graph"); // titulo de la pantalla
    initGL(); // crear la ventana cargando la matriz identidad
    glEnable(GL_TEXTURE_2D); // habilitar funciones
    glutDisplayFunc(window_display);
    glutReshapeFunc(window_reshape);
    glutKeyboardFunc(key_pressed);
    glutMouseFunc(mouse_click);
    glutSpecialFunc(key_press);
    show_menu();
    glutMainLoop();
    // Unreachable code
}

void print_with_color(const string& color, const string& str) {
    cout << "\033[1;" << color << "m"<< str << "\033[0m\n";
}