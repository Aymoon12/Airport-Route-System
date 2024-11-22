// included libraries
#include <fstream>
#include <iostream>
#include <vector>
#include "Graph.h"

// using stuff
using std::ofstream;
using std::ifstream;
using std::vector;

#ifndef CORE_FUNCTIONS_H
#define CORE_FUNCTIONS_H
//

class Flight_Path {
public:
  // variables
  vector<Vertex*> path;
  int total_distance;
  int total_cost;
  int id;

  // functions / constructors
  Flight_Path();
  Flight_Path(const Flight_Path &other);
  ~Flight_Path();

  // operators
  bool operator>(const Flight_Path &other) const;
  bool operator<(const Flight_Path &other) const;

};

// headers
void Create_File_Graph(ifstream & input_file, Graph* &graph);
vector<Flight_Path> Dijkstra(Graph* &graph, string start, string end);
Flight_Path Shortest_Flight_Path(Graph* &graph, string start, string end);
vector<Flight_Path> Shortest_Flight_Paths_State(Graph* &graph, string start, string end);
Flight_Path Shortest_Flight_Path_Limited(Graph* &graph, string start, string end, int k);
void directedToUndirected(Vertex* &avl_root, Graph* &newGraph);
void print_Flight_Path_SHORT(string n1, string n2, Flight_Path path);
void print_Flight_Path_LIMITED(string n1, string n2, Flight_Path path, int limit);
void print_Flight_Paths_STATE(string n, string s, vector<Flight_Path> paths);
void print_Flight_COUNT(Graph *&g, vector<string> vn);
int CountFlightPaths(Vertex* vertex, string name);
void kruskal(Graph *&g);
void directedToUndirected(Vertex *&root, Graph *&newGraph);
void primAlgorithm(Graph* &graph, Graph*& prim);
void primMST(Graph& graph);
void kruskalAlgorithm(Graph*graph,Graph* kruskal);

//
#endif