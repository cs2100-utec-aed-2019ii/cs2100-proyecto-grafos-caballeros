#define node_int_2D Node<int, Coordinate2D>
#define GL_SILENCE_DEPRECATION

#include "Lectura.h"

#define adjacency_list_with_node_int_2D_and_vectorized AdjacencyList<node_int_2D, vectorized>

using namespace std;

GLvoid initGL();
GLvoid key_pressed(unsigned char key, int x, int y);
GLvoid mouse_click(int button, int state, int x, int y);
GLvoid window_display();
GLvoid window_reshape(GLsizei width, GLsizei height);
GLvoid draw_graph(int argc, char** argv);
void show_menu();

AdjacencyList<node_int_2D, vectorized>* al;
Lectura<node_int_2D, Coordinate2D>* lector; // read and save (.vtk format)
vector<vector<node_int_2D>> adj_mat;

int main(int argc, char** argv) {

    srand(time(nullptr));

    lector = new Lectura<node_int_2D, Coordinate2D>();

    al = new AdjacencyList<node_int_2D, vectorized>();
/*
    auto* n1 = new node_int_2D(100);
    auto* n2 = new node_int_2D(200);
    auto* n3 = new node_int_2D(300);
    auto* n4 = new node_int_2D(400);

    al->insert_node_by_address(n1);
    al->insert_node_by_address(n2);
    al->insert_node_by_address(n3);
    al->insert_node_by_address(n4);

    al->link_node_by_address(n1, n2);
    al->link_node_by_address(n2, n3);

    al->link_node_by_address(n1, n4);
    al->link_node_by_address(n4, n3);
*/
    // Adjacency list
    al = lector->cargar_datos("nodos.vtk");
    al->print_adjacency_list();
    //al->print_adjacency_list();

    adj_mat = *(al->adjacency_list);

    // Dibujar grafo
    draw_graph(argc, argv);
    cout << "Termina el main" << endl;

   /* delete n1;
    delete n2;
    delete n3;
    delete n4; */
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

GLvoid key_pressed(unsigned char key, int x, int y) {
    switch (key) {
        case '1': {
            cout << "Opción 1.\n";
            break;
        }
        case '2': {
            cout << "Opción 2.\n";
            break;
        }
        case '3':
            cout << "Opción 3.\n";
            break;
        case '4':
            cout << "Opción 4.\n";
            break;
        case '5':
            cout << "Opción 5.\n";
            break;
        case '6':
            cout << "Opción 6.\n";
            break;
        case '7': // MST Kruskal: pintar el minimum spanning tree de rojo
            cout << "Opción 7.\n";
            break;
        case '8': // MST Prim: pintar el minimum spanning tree de rojo
            cout << "Opción 8.\n";
            break;
        case '9': // BFS: mostrar la secuencia de nodos
            cout << "Opción 9.\n";
            break;
        case 'a': // DFS: mostrar la secuencia de nodos
            cout << "Opción a.\n";
            break;
        case 'b':
            cout << "Opción b.\n";
            break;
        case 'c':
            cout << "Opción c.\n";
            break;
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
            cout << key << endl;
            break;
    }
}

GLvoid mouse_click(int button, int state, int x, int y) {
    if (state == GLUT_DOWN && button == GLUT_LEFT_BUTTON) {
        cout << "x: " << x << " y: " << WINDOW_HEIGHT-y << endl;
        // TODO: detectar si el click fue en las coordenadas de algun nodo
        // TODO: binary search que busque el nodo que fue clickeado
        unsigned s = al->size;
        for (unsigned i = 0; i < s; ++i) {
            node_int_2D _node = adj_mat[i][0];
            if (_node.coordinate.x == (float)x && _node.coordinate.y == WINDOW_HEIGHT-y) {
                cout << "NODE ABOUT TO BE DELETED: " << _node.get_value() << endl;
                al->delete_node_by_value(_node.value);
                cout << "NODE DELETEEEEEEEDDDD" << endl;
                s = al->size;
                adj_mat = *(al->adjacency_list);
                al->print_adjacency_list();
                glutPostRedisplay(); // refresh window to see the change
            }
        }
    }
}

GLvoid window_display() {
    glClear(GL_COLOR_BUFFER_BIT);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0.0f, WINDOW_WIDTH, 0.0f, WINDOW_HEIGHT, 0.0f, Z_DIST);
    // Esto dibuja los nodos O(V)
    for (unsigned i = 0; i < al->size; ++i) {
        glPushMatrix();
            glTranslatef(adj_mat[i][0].coordinate.x, adj_mat[i][0].coordinate.y, 0);
            glColor3f(SPHERE_RED, SPHERE_GREEN, SPHERE_BLUE);
            glutWireSphere(NODE_RADIUS, SPHERE_SLICES, SPHERE_STACKS);
        glPopMatrix();
    }

    // Esto dibuja las aristas O(Vˆ2)
    // TODO: preguntarle al profe si necesito ponerle glPushMatrix();
    for (unsigned i = 0; i < al->size; ++i) {
        node_int_2D starting_node = adj_mat[i][0]; // get NODE
        vector<node_int_2D> nodes = al->nodes_linked_to_node(&starting_node); // get nodes linked to NODE
        for (auto& n : nodes) { // iterate through nodes linked to NODE
            glBegin(GL_LINES);
                glVertex2f(starting_node.coordinate.x, starting_node.coordinate.y);
                glVertex2f(n.coordinate.x, n.coordinate.y);
            glEnd();
        }
    }
    glutSwapBuffers();
    glFlush();
}

GLvoid window_reshape(GLsizei width, GLsizei height) {
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0.0f, width, 0.0f, height, 0.0f, Z_DIST);
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