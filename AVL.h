// Included libraries
#include <iostream>
#include <string>
#include "Graph.h"

#ifndef AVL_H
#define AVL_H

// using stuff
using std::cout; using std::endl; using std::max; using std::string;

// headers
int height(Vertex *N);
int getBalance(Vertex *N);
Vertex* insert(Vertex *node, string name, string state, int id);
Vertex* insert(Vertex *node, string name, int id);
Vertex* find(Vertex* root, string name);
Vertex* rightRotate(Vertex *y);
Vertex* leftRotate(Vertex *x);

#endif