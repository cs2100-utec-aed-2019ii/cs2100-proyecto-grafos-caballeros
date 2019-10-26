#define _node_type int
#define generic_node Node<_node_type, Coordinate2D>
#define _dirigido false
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

AdjacencyList<generic_node, vectorized>* al; // adjacency list object
Lectura<generic_node, Coordinate2D, _dirigido>* lector; // read and save (.vtk format)
vector<vector<generic_node>> adj_mat; // adjacency list
Grafo<generic_node, _dirigido>* graph;

bool draw_MST_Kruskal = false;
bool draw_MST_Prim = false;
bool draw_BFS_path = false;
bool draw_DFS_path = false;
bool insert_mode = false;
bool degree_of_node_mode = false; bool degree_of_incidence = false; bool degree_of_outcidence = false;
bool neighborhood_mode = false; bool permission_to_draw_neighborhood = false;
bool validate_neighborhood = false;

short nodes_selected_for_DFS = 0;
short nodes_selected_for_BFS = 0;

pair<generic_node, generic_node> nodes_for_BFS_or_DFS;
vector<generic_node> nodes_to_link;
vector<generic_node> possible_neighbors;

short possible_neighbors_selected = 0;

int main(int argc, char** argv) {

    srand(time(nullptr));

    lector = new Lectura<generic_node, Coordinate2D, _dirigido>();

    /// Read adjacency list from .vtk
    al = lector->cargar_datos("binary_fire_symbolmesh_image.vtk");
    al->print_adjacency_list();

    /// Creando el grafo
    graph = new Grafo<generic_node, no_dirigido>(al);

    adj_mat = *(al->adjacency_list);

    /// Dibujar grafo
    draw_graph(argc, argv);

    cout << "Programa terminando..." << endl;

    delete al;
    delete lector;

    return 0;
}

void show_menu() {
    cout << "--------------------Grapho--------------------\n";
    cout << "Hacer click en un nodo para eliminarlo.\n";
    cout << "1. Insertar nodo.\n";
    cout << "3. Grado de un nodo.\n";
    cout << "4. Comprobar si el grafo es conexo.\n";
    cout << "5. Comprobar si el grafo es bipartito.\n";
    cout << "6. Comprobar si el grafo es denso.\n";
    cout << "7. MST de Kruskal.\n";
    cout << "8. MST de Prim.\n";
    cout << "9. Breadth First Search.\n";
    cout << "a. Depth First Search.\n";
    cout << "b. Mostrar vecindad de un nodo.\n";
    cout << "c. Validar la vecindad de un nodo.\n";
    cout << "q. Terminar el programa.\n\n";
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
                cout << "Modo insertar habilitado.\n";
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
                cout << "Modo insertar deshabilitado.\n";
                glutPostRedisplay(); // re-draw window
                break;
            }
            break;
        }
        case '3': {
            if (!degree_of_node_mode) {
                degree_of_node_mode = true;
                cout << "Modo grado de un nodo activado. Presione 3 para desactivar.\n";

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
                cout << "Modo grado de un nodo desactivado.\n";
            }
            break;
        }
        case '4': {
            // TODO: conexo
            break;
        }
        case '5': {
            // TODO: bipartito
            break;
        }
        case '6': {
            if (graph->calculate_density() >= 0.9f) {
                cout << "El grafo es denso!\n";
            } else {
                cout << "El grafo no es denso!\n";
            }
            break;
        }
        case '7': { // MST Kruskal: pintar el minimum spanning tree de rojo
            if (!draw_MST_Kruskal) {
                cout << "Modo MST Kruskal habilitado. Presione 7 para desactivar.\n";
                draw_MST_Kruskal = true;
                // refresh window so that path can be drawn
                glutPostRedisplay();
            } else {
                draw_MST_Kruskal = false;
                cout << "Modo MST Kruskal deshabilitado.\n";
                // refresh window so that path can be erased from the window
                glutPostRedisplay();
            }
            break;
        }
        case '8': { // MST Prim: pintar el minimum spanning tree de rojo
            if (!draw_MST_Prim) {
                cout << "Modo MST Prim habilitado. Presione 8 para desactivar.\n";
                draw_MST_Prim = true;
                // refresh window so that path can be drawn
                glutPostRedisplay();
            } else {
                draw_MST_Prim = false;
                cout << "Modo MST Prim deshabilitado.\n";
                // refresh window so that path can be erased from the window
                glutPostRedisplay();
            }
            break;
        }
        case '9': { // BFS: mostrar la secuencia de nodos
            if (!draw_BFS_path) {
                cout << "Modo BFS habilitado. Presione 9 para desactivar.\n";
                draw_BFS_path = true;
                // refresh window so that path can be drawn
                glutPostRedisplay();
            } else {
                draw_BFS_path = false;
                nodes_selected_for_BFS = 0;
                cout << "Modo BFS deshabilitado.\n";
                // refresh window so that path can be erased from the window
                glutPostRedisplay();
            }
            break;
        }
        case 'a': { // DFS: mostrar la secuencia de nodos
            if (!draw_DFS_path) {
                cout << "Modo DFS habilitado. Presione 'a' para desactivar.\n";
                draw_DFS_path = true;
                // refresh window so that path can be drawn
                glutPostRedisplay();
            } else {
                draw_DFS_path = false;
                nodes_selected_for_DFS = 0;
                cout << "Modo DFS deshabilitado.\n";
                // refresh window so that path can be erased from the window
                glutPostRedisplay();
            }
            break;
        }
        case 'b': {
            if (!neighborhood_mode) {
                neighborhood_mode = true;
                cout << "Modo vecindad de un nodo habilitado. Presione 'b' para deshabilitarlo.\n";
                cout << "Seleccione un nodo para descubrir su vecindario.\n";
            } else {
                cout << "Modo vecindad deshabilitado.\n";
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
                cout << "Modo validar vecindad habilitado. Presione 'c' para deshabiliarlo.\n";
            } else {
                cout << "Modo validar vecindad deshabilitado.\n";
                possible_neighbors_selected = 0;
                validate_neighborhood = false;
                possible_neighbors.clear();
            }
            break;
        }
        case 'q':
            char respuesta;
            cout << "¿Quieres guardar el grafo en disco? [y/n]" << endl;
            cin >> respuesta;

            if (respuesta == 'y') {
                //al->delete_node_by_value(4);
                lector->save_adjacency_list(al);
                cout << "Grafo guardado como nodos_antiguos.vtk." << endl;
            }

            delete lector;
            delete al;
            exit(1);
        default:
            cout << "Esa opción no existe!\n";
            break;
    }
}

GLvoid mouse_click(int button, int state, int x, int y) {
    if (state == GLUT_DOWN && button == GLUT_LEFT_BUTTON) {
        cout << "x: " << x << " y: " << WINDOW_HEIGHT-y << endl;
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
                    //erase node from list
                    vector<generic_node>::iterator it = std::find(nodes_linked_to_selected_node_for_neighborhood.begin(), nodes_linked_to_selected_node_for_neighborhood.end(), _node);
                    int index = std::distance(nodes_linked_to_selected_node_for_neighborhood.begin(), it);
                    nodes_linked_to_selected_node_for_neighborhood.erase(nodes_linked_to_selected_node_for_neighborhood.begin()+index);
                    //erase node from list
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
    show_menu();
    glutMainLoop();
    // Unreachable code
}