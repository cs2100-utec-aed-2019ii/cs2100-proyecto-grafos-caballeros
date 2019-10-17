#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <cstring>
#include <stdio.h>
#include <string.h>
#include "Node.h"

using namespace std;

int main () {
    Node<int> nodo1(8);
    vector <Node<int>> nodos;
    int cantidad_de_nodos = 0;
    string cantidad;
    string line;
    ifstream myfile ("test.vtk");
    char separador = ' ';
    
    if (myfile.is_open()) {
        for (int i = 0; i < 5; ++i)
            getline(myfile, line);
        
        string sline = line;
        bool temp = true;
        
        while (getline(myfile, line) && temp) {
            sline = line;
            temp = false;
        }
        
        for(size_t p = 0, q = 0, i = 0; i < 2; p = q, i++)
            cantidad = sline.substr(p+(p!=0),(q=sline.find(separador, p+1))-p-(p!=0));
        
        cantidad_de_nodos = stoi(cantidad);
        
        cout << "line: " << cantidad_de_nodos << endl;
        
        while ( getline (myfile,line) ) {
            cout << line << '\n';
        }
        myfile.close();
    } else {
        cout << "Unable to open file" << endl;
    }
    
    //cout << *nodo1 << endl;
    return 0;
}
