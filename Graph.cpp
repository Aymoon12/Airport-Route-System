// included libraries
#include <iostream>
#include <string>
#include <vector>

#include "Graph.h"
#include "AVL.h"

// using stuff
using std::cout;
using std::endl;
using std::string;
using std::vector;

/**
 * @brief Construct a new Vertex::Vertex object
 **/
Vertex::Vertex(string name, string state, int id) {
  this->id = id; // sets the id
  this->name = name;
  this->state = state;
  this->height = 0;
  edges = new vector<Edge>(); // creates a list of edges
  right = nullptr;
  left = nullptr;
}
Vertex::Vertex() {
  this->id = -1;
  this->name = "";
  this->state = "";
  this->height = 0;
  this->right = nullptr;
  this->left = nullptr;
  edges = new vector<Edge>();
}


Vertex::Vertex(string name, int id){
  this->id = id;
  this->name = name;
  this-> state = "";
  this->height = 0;
  edges = new vector<Edge>();
  right = nullptr;
  left = nullptr;
}
Vertex::~Vertex() {
  // Delete the edges list and its elements
  delete edges;
}

/**
 * @brief Construct a new State::State object
 **/
State::State() {
  
}
State::State(string name) {
  this->name = name;
}
State::~State() {
  
}

/**
 * @brief Add an edge to the vertex
 **/
Edge::Edge(Vertex *start, Vertex *end, int distance, int cost) {
  // updates all the variables
  this->start = start;
  this->end = end;
  this->distance = distance;
  this->cost = cost;
}
Edge::Edge() {
  this->start = nullptr;
  this->end = nullptr;
  this->distance = 0;
  this->cost = 0;
}
Edge::~Edge() {}
bool Edge::operator<(const Edge &other) const {
  if (this->cost < other.cost) {
    return true;
  }
  return false;
}
bool Edge::operator>(const Edge &other) const {
  if (this->cost > other.cost) {
    return true;
  }
  return false;
}

/**
 * @brief Construct a new Graph::Graph object
 **/
Graph::Graph(int V) {
  // updates the variables
  this->V = V;
  this->size = 0;
  this->root = nullptr;
}
Graph::Graph() {}

void recursive_find(Vertex* root) {
  // base case
  if (root == nullptr) {
    return;
  }

  // recursive case
  recursive_find(root->left);
  recursive_find(root->right);

  // deletes the stuff
  delete root;
}
Graph::~Graph() {
  // Delete the root vertex and its edges
  recursive_find(root);
}

/**
 * @brief adds an edge to the graph
 * @param (int v): the vertex, (int w1): the first weight, (int w2): the second
 *weight
 * @return void
 **/
Edge* Graph::addEdge(Vertex *start, Vertex *end, int distance, int cost) {
  // null check
  if (start == nullptr || end == nullptr) {
    cout << "Invalid vertexes; edge not created" << endl;
    return nullptr;
  }

  // pushes the weights back into the 2 adjacency lists
  Edge *newEdge = new Edge(start, end, distance, cost);
  start->edges->push_back(*newEdge);
  return newEdge;
}

/**
 * @brief adds a verticie to the graph
 * @param (string name): the vertex's name
 * @return void
 **/
Vertex* Graph::addVertex(string name, string state) {
  // checks for size
  if (this->size >= this->V) {
    cout << "Graph is full; vertex not created" << endl;
    return nullptr;
  }

  // inserts the vertex
  this->root = insert(this->root, name, state, this->size);
  Vertex* foundRoot = find(this->root, name);

  // updates size if the id is 1 higher (meaning it just got added)
  if (foundRoot->id == this->size) {
    this->size++;
  }

  return foundRoot;
}

Vertex* Graph::addVertex(string name){
  // checks for size
    if (this->size >= this->V) {
      cout << "Graph is full; vertex not created" << endl;
      return nullptr;
    }

    // inserts the vertex
    this->root = insert(this->root, name,this->size);
    Vertex* foundRoot = find(this->root, name);

    // updates size if the id is 1 higher (meaning it just got added)
    if (foundRoot->id == this->size) {
      this->size++;
    }

    return foundRoot;
  }