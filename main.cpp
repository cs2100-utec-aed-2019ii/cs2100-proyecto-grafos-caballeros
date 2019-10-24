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
vector<vector<node_int_2D>> adj_mat;
Lectura<node_int_2D, Coordinate2D>* l1 = new Lectura<node_int_2D, Coordinate2D>() ;

int main(int argc, char** argv) {

    srand(time(nullptr));

    ///Adjacency list
    al = l1->cargar_datos("nodos.vtk");
    l1->print();
    al->print_adjacency_list();
    adj_mat = al->get_matrix();

    // Dibujar grafo
    draw_graph(argc, argv);

    // Mostrar menú para insertar y/o borrar nodos
    show_menu();

    return 0;
}

void show_menu() {
    // TODO: menu with options for insertion and deletion
}

GLvoid initGL() {
    glClearColor(RED, GREEN, BLUE, ALPHA);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
}

GLvoid key_pressed(unsigned char key, int x, int y) {
    switch (key) { // qué tecla se ha presionado
        case 'q':
            char respuesta;
            cout << "¿Quieres guardar? y/n" << endl;
            cin >> respuesta;

            if (respuesta == 'y') {
                al->delete_node_by_value(4);
                l1->save_adjacency_list(al);
                cout << "Guardando adjacency list como nodos_antiguos.vtk." << endl;
            }

            delete l1;
            delete al;
            exit(1);
        default:
            cout << key << " key pressed." << endl;
            break;
    }
}

GLvoid mouse_click(int button, int state, int x, int y) {
    if (state == GLUT_DOWN && button == GLUT_LEFT_BUTTON) {
        cout << "Left button pressed at x: " << x << " y: " << WINDOW_HEIGHT-y << endl;
    }
}

GLvoid window_display() {
    glClear(GL_COLOR_BUFFER_BIT);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0.0f, WINDOW_WIDTH, 0.0f, WINDOW_HEIGHT, 0.0f, 100);

    // Esto dibuja los nodos O(V)
    for (unsigned i = 0; i < al->size; ++i) {
        glPushMatrix();
            glTranslatef(adj_mat[i][0].coordinate.x, adj_mat[i][0].coordinate.y, 0);
            glColor3f(SPHERE_RED, SPHERE_GREEN, SPHERE_BLUE);
            // FIXME: radius needs to depend on the amount of nodes
            glutWireSphere(NODE_RADIUS, SPHERE_SLICES, SPHERE_STACKS);
        glPopMatrix();
    }

    // Esto dibuja las aristas O(Vˆ2)
    // TODO: preguntarle al profe si necesito ponerle glPushMatrix();
    for (unsigned i = 0; i < al->size; ++i) {
        node_int_2D starting_node = adj_mat[i][0]; // get NODE
        vector<node_int_2D> nodes = al->nodes_linked_to_node(&starting_node); // get nodes linked to NODE
        for (auto n : nodes) { // iterate through nodes linked to NODE
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
    glOrtho(0.0f, width, 0.0f, height, 0.0f, 100);
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
    glutMainLoop();
}