// included libraries
#include <iostream>
#include <string>
#include <vector>

// Graph.h | Header file for the Graph Class
#ifndef GRAPH_H
#define GRAPH_H

#include "Stack.h"

// using stuff
using std::string;
using std::vector;

// headers
class Edge;

// Vertex structure
class Vertex {
public:
  // variables
  string name;
  string state;
  int id;
  int height;
  Vertex *right, *left; // Destination airports that are connected to original vertex
  vector<Edge> *edges; // Change to pointer to list of Edge pointers
  // constructors/destructor
  Vertex();
  Vertex(string name, string state, int id);
  Vertex(string name,int id);
  ~Vertex();
};

// State class
class State {
public:
  // variables
  string name;
  vector<string> airports;
  // constructors/destructors
  State();
  State(string name);
  ~State();
};

// Edge structure
class Edge {
public:
  // variables
  Vertex *start; // Starting airport
  Vertex *end; // Ending Airport
  int distance; // flight distance
  int cost; // Weight
  // constructors/destructor
  Edge();
  Edge(Vertex *start, Vertex *end, int distance, int cost);
  ~Edge();
  // operators
  bool operator>(const Edge &other) const;
  bool operator<(const Edge &other) const;
};

// Graph Class
class Graph {
public:
  // variables
  int V;
  int size;
  Vertex* root;
  vector<State> states;

  // headers
  Graph();
  Graph(int V); // constructor
  ~Graph(); // Destructor to deallocate memory
  // functions

  Vertex* addVertex(string name, string state);
  Vertex* addVertex(string name);

  Edge* addEdge(Vertex *start, Vertex *end, int distance, int cost);
  void DFS(Vertex *s);
};

#endif
