//
// Created by Gabriel Spranger Rojas on 10/19/19.

#ifndef GRAPHOS_GRAPHDRAWER_H
#define GRAPHOS_GRAPHDRAWER_H

#define GL_SILENCE_DEPRECATION

#define adjacency_list_with_vector AdjacencyList<node_type, vectorized>
#define adjacency_list_with_linked_list AdjacencyList<node_type, linked_list>

#include <GLUT/glut.h>
#include <OpenGL/gl.h>
#include <OpenGL/glu.h>

#include "Macros.h"
#include "AdjacencyList.h"

template <typename graph_type, typename node_type>
class GraphDrawer {};

template <typename node_type>
class GraphDrawer<adjacency_list_with_vector, node_type> {
    adjacency_list_with_vector adjacencyList;

    GLvoid initGL() {
        glClearColor(RED, GREEN, BLUE, ALPHA);
        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
    }

    static GLvoid window_display() {
        glClear(GL_COLOR_BUFFER_BIT);
        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();

        // TODO: dibujar los nodos y aristas del adjacency list

        glutSwapBuffers();
        glFlush();
    }

    static GLvoid window_reshape(GLsizei width, GLsizei height) {
        gluLookAt(1, 1, 1, 1, 1, 1, 1, 1, 1);
        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        glOrtho(0.0f, WINDOW_WIDTH, 0.0f, WINDOW_HEIGHT, 1.0f, 0);
        glMatrixMode(GL_MODELVIEW);
    }

public:
    explicit GraphDrawer(adjacency_list_with_vector al): adjacencyList{al} {}

    GLvoid initialize(int argc, char** argv) {
        glutInit(&argc, argv);
        glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);
        glutInitWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT); // define window size
        glutInitWindowPosition(200, 200); // donde se va a renderizar la pantalla
        glutCreateWindow("Graphos"); // titulo de la pantalla
        initGL(); // crear la ventana cargando la matriz identidad
        glEnable(GL_TEXTURE_2D); // habilitar funciones
        glutDisplayFunc(window_display);
        glutReshapeFunc(window_reshape);
//        glutKeyboardFunc(&key_pressed);
//        glutTimerFunc(DURATION, Timer, 1);
//        glutMouseFunc(&mouse_click);
        glutMainLoop();
    }

};

template <typename node_type>
class GraphDrawer<adjacency_list_with_linked_list, node_type> {

};

#endif //GRAPHOS_GRAPHDRAWER_H
