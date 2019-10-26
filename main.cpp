#define _node_type int
#define generic_node Node<_node_type, Coordinate2D>
#define GL_SILENCE_DEPRECATION

#include "Lectura.h"

#define adjacency_list_with_node_int_2D_and_vectorized AdjacencyList<generic_node, vectorized>

using namespace std;

GLvoid initGL();
GLvoid key_pressed(unsigned char key, int x, int y);
GLvoid mouse_click(int button, int state, int x, int y);
GLvoid window_display();
GLvoid window_reshape(GLsizei width, GLsizei height);
GLvoid draw_graph(int argc, char** argv);
void show_menu();

AdjacencyList<generic_node, vectorized>* al;
Lectura<generic_node, Coordinate2D>* lector; // read and save (.vtk format)
vector<vector<generic_node>> adj_mat;

bool draw_MST_Kruskal = false;
bool draw_MST_Prim = false;
bool draw_BFS_path = false;
bool draw_DFS_path = false;
bool insert_mode = false;

short nodes_selected_for_DFS = 0;
short nodes_selected_for_BFS = 0;

pair<generic_node, generic_node> nodes_for_BFS_or_DFS;
vector<generic_node> nodes_to_link;

int main(int argc, char** argv) {

    srand(time(nullptr));

    lector = new Lectura<generic_node, Coordinate2D>();

    // Adjacency list
    al = lector->cargar_datos("nodos.vtk");
    al->print_adjacency_list();

    adj_mat = *(al->adjacency_list);

    // Dibujar grafo
    draw_graph(argc, argv);
    cout << "Termina el main" << endl;

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
// FIXME: solo inserta bien una vez? Chequear
generic_node new_node = generic_node();

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
            for (auto& n : nodes_to_link) {
                al->link_node_by_address(&new_node, &n);
            }
            adj_mat = *(al->adjacency_list);
            al->print_adjacency_list();
            nodes_to_link.clear();
            insert_mode = false;
            cout << "Modo insertar deshabilitado.\n";
            glutPostRedisplay();
            break;
        }
        case '3': {
            cout << "Opción 3.\n";
            break;
        }
        case '4': {
            cout << "Opción 4.\n";
            break;
        }
        case '5': {
            cout << "Opción 5.\n";
            break;
        }
        case '6': {
            cout << "Opción 6.\n";
            break;
        }
        case '7': { // MST Kruskal: pintar el minimum spanning tree de rojo
            if (!draw_MST_Kruskal) {
                cout << "Modo MST Kruskal habilitado.\n";
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
                cout << "Modo MST Prim habilitado.\n";
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
                cout << "Modo BFS habilitado.\n";
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
                cout << "Modo DFS habilitado.\n";
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
            cout << "Opción b.\n";
            break;
        }
        case 'c': {
            cout << "Opción c.\n";
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
                    cout << "NODE ABOUT TO BE DELETED HAS VALUE: " << _node.get_value() << endl;
                    al->delete_node_by_value(_node.get_value());
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

    /// Esto dibuja las aristas O(Vˆ2)

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
    glutCreateWindow("Graphos"); // titulo de la pantalla
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