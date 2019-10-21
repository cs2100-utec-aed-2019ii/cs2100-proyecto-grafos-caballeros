#define node_int_2D Node<int, Coordinate2D>

#include "GraphDrawer.h"

#define adjacency_list_with_node_int_2D_and_vectorized AdjacencyList<node_int_2D, vectorized>

using namespace std;

GLvoid initGL();
GLvoid key_pressed(unsigned char key, int x, int y);
GLvoid mouse_click(int button, int state, int x, int y);
GLvoid window_display();
GLvoid window_reshape(GLsizei width, GLsizei height);
GLvoid initialize(int argc, char** argv);

AdjacencyList<node_int_2D, vectorized> al;
vector<vector<node_int_2D>> adj_mat;

int main(int argc, char** argv) {

    auto a = node_int_2D(3, 10, 10);
    auto b = node_int_2D(4, 50, 50);
    auto c = node_int_2D(1, 100, 100);

    al.insert_node(&a);
    al.insert_node(&b);
    al.insert_node(&c);

    al.link_node(&a,&c);

    al.link_node(&b, &a);

    al.print_adjacency_list();

    // Dibujando el grafo

//    GraphDrawer<adjacency_list_with_node_int_2D_and_vectorized, node_int_2D> graphDrawer(al);
//
//    graphDrawer.initialize(argc, argv);

    adj_mat = al.get_matrix();

    initialize(argc, argv);

    return 0;
}

GLvoid initGL() {
    glClearColor(RED, GREEN, BLUE, ALPHA);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
}

GLvoid key_pressed(unsigned char key, int x, int y) {
    switch (key) { // qué tecla se ha presionado
        case 'q':
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
    for (unsigned i = 0; i < al.size; ++i) {
        glPushMatrix();
            glTranslatef(adj_mat[i][0].coordinate.x, adj_mat[i][0].coordinate.y, 0);
            glColor3f(0.8f, 0.5f, 0.2f);
            // TODO: radius needs to depend on the amount of nodes
            glutWireSphere(5, 50, 10);
        glPopMatrix();
    }

    // Esto dibuja las aristas O(Vˆ2)
    // TODO: preguntarle al profe si necesito ponerle glPushMatrix();
    for (unsigned i = 0; i < al.size; ++i) {
        node_int_2D starting_node = adj_mat[i][0]; // get NODE
        vector<node_int_2D> nodes = al.nodes_linked_to_node(&starting_node); // get nodes linked to NODE
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

GLvoid initialize(int argc, char** argv) {
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
//    glutTimerFunc(DURATION, Timer, 1);
    glutMouseFunc(mouse_click);
    glutMainLoop();
}