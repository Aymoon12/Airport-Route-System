// included libraries
#include <fstream>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <algorithm>

#include "AVL.h"
#include "Core_Functions.h"
#include "Graph.h"
#include "Queue.h"
#define INT_MAX 100000000

// using stuff
using std::cin;
using std::cout;
using std::endl;
using std::getline;
using std::greater;
using std::ifstream;
using std::istringstream;
using std::left;
using std::right;
using std::setprecision;
using std::setw;
using std::string;
using std::stringstream;
using std::vector;

////////////////////////////////////////////////////////
// Flight_Path implementation
////////////////////////////////////////////////////////

// constructors
Flight_Path::Flight_Path() {
  this->total_distance = 0;
  this->total_cost = 0;
}
Flight_Path::Flight_Path(const Flight_Path &other) {
  // sets up path
  for (int i = 0; i < other.path.size(); i++) {
    this->path.push_back(other.path[i]);
  }

  // sets up the rest
  this->total_distance = other.total_distance;
  this->total_cost = other.total_cost;
}
Flight_Path::~Flight_Path() {}
bool Flight_Path::operator>(const Flight_Path &other) const {
  return total_distance > other.total_distance;
}
bool Flight_Path::operator<(const Flight_Path &other) const {
  return total_distance < other.total_distance;
}

/**
 * @brief loops through a file and inputs the data to a graph
 * @param1 input file
 * @param2 the graph
 * @param3 the AVL tree
 * @return void
 **/
int find_index(Graph *&g, string state) {
  if (g->states.size() > 0) {
    for (int i = 0; i < g->states.size(); i++) {
      if (g->states[i].name == state) {
        return i;
      }
    }
  }
  return -1;
}
bool find_index_a(State s, string name) {
  if (s.airports.size() > 0) {
    for (int i = 0; i < s.airports.size(); i++) {
      if (s.airports[i] == name) {
        return true;
      }
    }
  }
  return false;
}
void Create_File_Graph(ifstream &input_file, Graph *&graph) {
  // Check if the file opened successfully
  if (!input_file.is_open()) {
    cout << "Error opening file!" << endl;
    return;
  }

  string line; // Variable to store each line of the file

  // Skip the header line
  getline(input_file, line);

  // Loop through each line of the file
  while (getline(input_file, line)) {
    // Create a stringstream from the current line
    stringstream ss(line);

    // variables to track
    string Origin_airport, Destination_airport, Origin_city, Origin_state,
        Destination_city, Destination_state;
    int Distance, Cost;
    //,424,374
    // GETS ALL OF THE DATA FROM THE LINE
    /////////////////////////////////////
    getline(ss, Origin_airport, ',');
    getline(ss, Destination_airport, ',');
    ss.ignore(); // ignores the '"'
    getline(ss, Origin_city, ',');
    ss.ignore(); // ignores the ' '
    getline(ss, Origin_state, '"');
    ss.ignore(); // ignores ','
    ss.ignore(); // ignores '"'
    getline(ss, Destination_city, ',');
    ss.ignore(); // ignores the ' '
    getline(ss, Destination_state, '"');
    ss.ignore(); // ignores ','
    ss >> Distance;
    ss.ignore(); // ignores ','
    ss >> Cost;
    /////////////////////////////////////

    // inserts the vertexes into the AVL tree
    Vertex *v1 = graph->addVertex(Origin_airport, Origin_state);
    Vertex *v2 = graph->addVertex(Destination_airport, Destination_state);

    // works on the state
    int index1 = find_index(graph, Origin_state);
    if (index1 == -1) {
      // there is already a state
      graph->states.push_back(Origin_state);
    } else {
      if (!find_index_a(graph->states[index1], Origin_airport)) {
        graph->states[index1].airports.push_back(Origin_airport);
      }
    }

    int index2 = find_index(graph, Destination_state);
    if (index2 == -1) {
      // there is already a state
      graph->states.push_back(Destination_state);
    } else {
      if (!find_index_a(graph->states[index2], Destination_airport)) {
        graph->states[index2].airports.push_back(Destination_airport);
      }
    }

    // adds the edge to the graph
    graph->addEdge(v1, v2, Distance, Cost);
    // graph->addEdge(v2, v1, Distance, Cost); // to go back?
  }

  input_file.close(); // Close the file
}

// Function for finding a name in a group of edges
Edge *find_edge_end(Vertex *v, string name) {
  // first checks for empty edges
  if (v->edges->size() == 0) {
    return nullptr;
  }

  // then checks for the name
  for (Edge &e : *v->edges) {
    if (e.end->name == name) {
      return &e;
    }
  }

  return nullptr;
}

// function for getting the count of flights inward
int find_inward(Vertex *v, string name) {
  // base case
  if (v == nullptr) {
    return 0;
  }

  // recursive case
  int count = 0;
  if (find_edge_end(v, name)) {
    count = 1;
  }

  // returns the count
  return find_inward(v->left, name) + find_inward(v->right, name) + count;
}
int CountFlightPaths(Vertex *vertex, string name) {
  if (vertex == nullptr) {
    return -1;
  }

  return vertex->edges->size() + find_inward(vertex, name);
}

/**
 * @brief uses Dikstra's algorithm to find the shortest path between two
 *vertexes
 * @param1 (Graph*& graph) - the graph
 * @param2 (Vertex* avl_root) - the AVL tree
 * @param3 (string start) - the starting vertex
 * @param4 (string end) - the ending vertex
 * @return (vector<Flight_Path>) a list of vertexes that create the shortest
 *path
 **/
void d_r(string start, string end, Vertex *currentNode,
         vector<Flight_Path> &paths, Flight_Path last_path,
         vector<bool> &visited, int dist, int cost, int type) {
  // case: the current node is null
  if (currentNode == nullptr) {
    return;
  }

  // case: the node has been visited
  if (visited[currentNode->id] == true) {
    return;
  }

  // case: the current node is the end:
  // BASED ON TYPE
  // TYPE1 - AIRPORT MODE; TYPE2 - STATE MODE
  // TYPE 1
  if (type == 1) {
    if (currentNode->name == end) {
      last_path.path.push_back(currentNode);
      last_path.total_cost += cost;
      last_path.total_distance += dist;
      paths.push_back(last_path);
      return;
    }
  } else {
    // TYPE 2
    if (currentNode->state == end) {
      last_path.path.push_back(currentNode);
      last_path.total_cost += cost;
      last_path.total_distance += dist;
      paths.push_back(last_path);
      return;
    }
  }

  // case: normal node
  visited[currentNode->id] = true;
  last_path.path.push_back(currentNode);
  last_path.total_cost += cost;
  last_path.total_distance += dist;
  paths.push_back(last_path);

  // loops through the edges
  for (Edge edge : *currentNode->edges) {
    d_r(start, end, edge.end, paths, last_path, visited, edge.distance,
        edge.cost, type);
  }
}
vector<Flight_Path> Dijkstra(Graph *&graph, string start, string end,
                             int type) {
  // Create necessary data structures
  PriorityQueue<int> pq(1000);
  vector<Flight_Path> paths;
  vector<bool> visited(graph->V,
                       false); // Initialize visited vector with false values

  // Call the recursive function to perform Dijkstra's algorithm
  Vertex *v = find(graph->root, start);
  Flight_Path first_path;
  d_r(start, end, v, paths, first_path, visited, 0, 0, type);

  // gets rid of all those that don't end at the right spot
  // CHECKS TYPES
  // TYPE1 - AIRPORT MODE; TYPE2 - STATE MODE
  vector<Flight_Path> temp;

  // TYPE 1
  if (type == 1) {
    for (Flight_Path p : paths) {
      if (p.path.back()->name == end) {
        temp.push_back(p);
      }
    }
  } else {
    // TYPE 2
    for (Flight_Path p : paths) {
      if (p.path.back()->state == end) {
        temp.push_back(p);
      }
    }
  }

  return temp;
}

bool isInEdgeVector(vector<Edge> v, Edge e) {
  for (Edge e1 : v) {
    if (e1.start->name == e.start->name && e1.end->name == e.end->name) {
      return true;
    }
  }
  return false;
}
void kruskal_r(Vertex *v, PriorityQueue<Edge> &pq) {
  // base case
  if (v == nullptr) {
    return;
  }

  // recursive case
  kruskal_r(v->left, pq);
  kruskal_r(v->right, pq);

  // adds the edges to the priority queue
  for (Edge e : *v->edges) {
    pq.push(e);
  }
  *v->edges = vector<Edge>();
}
void kruskal(Graph *&g) {
  // loops through the whole tree
  PriorityQueue<Edge> pq(1000);
  vector<Edge> edges;
  kruskal_r(g->root, pq);

  // moves from pq to edges
  if (!pq.isEmpty()) {
    while (!pq.isEmpty()) {
      Edge e = pq.extractMin();
      if (!isInEdgeVector(edges, e)) {
        edges.push_back(e);
      }
    }
  }

  // adds the edges to the graph
  for (Edge e : edges) {
    g->addEdge(e.start, e.end, e.distance, e.cost);
    g->addEdge(e.end, e.start, e.distance, e.cost);
  }
}

/**
 * @brief finds the shortest path between multiple flight paths
 * @param1 (Graph *&graph) a reference to the graph we are using
 * @param2 (string start) name of the starting vertex
 * @param3 (string end) name of the ending vertex
 * @return the shortened flight path
 **/
Flight_Path Shortest_Flight_Path(Graph *&graph, string start, string end) {
  // sets up priority queue
  PriorityQueue<Flight_Path> pq(1000);

  // gets list of flight paths
  vector<Flight_Path> paths = Dijkstra(graph, start, end, 1);

  // sorts the list of flight paths
  for (Flight_Path p : paths) {
    pq.push(p);
  }

  // there is nothing inside (no clear path)
  if (pq.isEmpty()) {
    return Flight_Path();
  }

  // returns samllest
  return pq.extractMin();
}

/**
 * @brief finds the shortest path between multiple flight paths (with limited
 *flights)
 * @param1 (Graph *&graph) a reference to the graph we are using
 * @param2 (string start) name of the starting vertex
 * @param3 (string end) name of the ending vertex
 * @param4 (int k) the max flights allowed
 * @return the shortened flight path
 **/
Flight_Path Shortest_Flight_Path_Limited(Graph *&graph, string start,
                                         string end, int k) {
  // sets up priority queue
  PriorityQueue<Flight_Path> pq(1000);

  // gets list of flight paths
  vector<Flight_Path> paths = Dijkstra(graph, start, end, 1);

  // sorts the list of flight paths
  for (Flight_Path p : paths) {
    pq.push(p);
  }

  // there is nothing inside (no clear path)
  if (pq.isEmpty()) {
    return Flight_Path();
  }

  // returns the path of the required k
  Flight_Path p = pq.extractMin();
  while (p.path.size() != k) {
    // first checks if we're out of paths
    if (pq.isEmpty()) {
      return Flight_Path();
    }
    p = pq.extractMin();
  }

  return p;
}

/**
 * @brief finds the shortest path between an airport and a state
 * @param1 (Graph *&graph) a reference to the graph we are using
 * @param2 (string start) name of the starting vertex
 * @param3 (string end) name of the ending vertex
 * @return a list of shortened flights
 **/
vector<Flight_Path> Shortest_Flight_Paths_State(Graph *&graph, string start,
                                                string end) {
  // the index of the state
  int state_index = find_index(graph, end);
  vector<Flight_Path> paths;

  // checks and loops through the airports in the graph
  if (graph->states.size() > 0) {
    for (string name : graph->states[state_index].airports) {
      // gets the shortened path
      Flight_Path p = Shortest_Flight_Path(graph, start, name);

      // adds to the list
      paths.push_back(p);
    }
  }

  // returns the list
  return paths;
}

/**
 * @brief uses the original directed Graph to make a undirected graph
 * @param1 (Vertex* & root) - the root of the directed graphs avl
 * @param2 (Graph* &newGraph) - empty undirected graph
 * @return (void) - only changes the newGraph (undirected Graph)
 **/
void directedToUndirected(Vertex *&root, Graph *&newGraph) {
  //

  // I might be confused on what this function is supposed to do, so I tried
  // implementing what I thought was a fix. But I commented out the original so
  // we can add it back if needed

  //
  if (root == nullptr) {
    return;
  }

  // // loops through the edges
  // for (Edge e : *root->edges) {

  //   Vertex *v1 = find(newGraph->root, e.start->name);
  //   // handles checking on vertexes
  //   if (v1 == nullptr) {
  //     // vertex is not in the new graph add it
  //     v1 = newGraph->addVertex(e.start->name, e.start->state);
  //   }

  //   // now for next one
  //   Vertex *v2 = find(newGraph->root, e.end->name);
  //   if (v2 == nullptr) {
  //     // vertex is not in the new graph add it
  //     v2 = newGraph->addVertex(e.end->name, e.end->state);
  //   }

  //   // now for the edges
  //   // first check if its a pair
  //   Edge* r = find_edge_end(e.end, e.start->name);
  //   if (r != nullptr) {
  //     // there is an edge pointing back, meaning its a pair
  //     if (e.cost > r->cost) {
  //       // cost is greater, so we add e
  //       newGraph->addEdge(v1, v2, 0, e.cost);
  //       newGraph->addEdge(v2, v1, 0, e.cost);
  //     } else {
  //       newGraph->addEdge(v1, v2, 0, r->cost);
  //       newGraph->addEdge(v2, v1, 0, r->cost);
  //     }
  //   } else {
  //     // there is only 1
  //     // add a second one so there is another one
  //     newGraph->addEdge(v1, v2, 0, e.cost);
  //     newGraph->addEdge(v2, v1, 0, e.cost);
  //   }
  // }

  for (auto &edge : *root->edges) {
    Vertex *start = edge.start;
    Vertex *end = edge.end;

    int cost = edge.cost;

    // We need to check if there is already an edge between these vertexes
    bool existingEdge = false;
    int existingCost = 10000000;

    // Check if the edge is already in our undirected graph
    Vertex *udEnd = find(newGraph->root, end->name);
    if (udEnd !=
        nullptr) { // if the Vertex exists, we will update the cost of the edge
      for (auto &udEndEdge : *udEnd->edges) {
        if (udEndEdge.end->name == start->name) {
          existingEdge = true;
          existingCost = std::min(cost, udEndEdge.cost);
          udEndEdge.cost = existingCost;
          // cout << "Updated edge cost for flight from " << start->name << " to
          // "
          //      << end->name << endl;
          break;
        }
      }
    }

    // case where there is no existing edge between the vertexes
    if (!existingEdge) {
      Vertex *newStart = newGraph->addVertex(start->name, start->state);
      Vertex *newEnd = newGraph->addVertex(end->name, start->state);

      newGraph->addEdge(newStart, newEnd, 0, cost);
      newGraph->addEdge(newEnd, newStart, 0, cost);
    }
  }

  directedToUndirected(root->left, newGraph);
  directedToUndirected(root->right, newGraph);
}

/**
 * @brief prints the graph for SHORTENED flight paths
 * @param1 (string n1) - name of the starting vertex
 * @param2 (string n2) - name of the ending vertex
 * @param3 (Flight_Path the flight path provided)
 * @return void
 **/
void print_Flight_Path_SHORT(string n1, string n2, Flight_Path path) {
  // checks if path is empty
  if (path.path.empty()) {
    cout << "Shortest route from " << n1 << " to " << n2 << ": None\n";
    return;
  }

  // path exists
  if (path.total_distance > 0) {
    cout << "Shortest route from " << n1 << " to " << n2 << ": ";
    for (Vertex *v : path.path) {
      if (v->name != n2) {
        cout << v->name << "->";
      } else {
        cout << v->name;
        ;
      }
    }
  }
  cout << ". The length is " << path.total_distance << ". The cost is "
       << path.total_cost << ".\n\n";
}

void print_Flight_Paths_STATE(string n, string s, vector<Flight_Path> paths) {
  if (paths.empty()) {
    return;
  }

  const int path_col_width = 10;   // for path names
  const int length_col_width = 10; // for total distance
  const int cost_col_width = 10;   // for total cost

  cout << "Shortest routes from " << n << " to " << s << " are:\n\n";

  // Set alignments and column widths for headers
  cout << setw(path_col_width) << left << "Path" << setw(length_col_width)
       << "Length" << setw(cost_col_width) << "Cost" << endl;

  // Print the paths
  for (Flight_Path p : paths) {
    if (p.total_distance == 0) {
      continue;
    }

    // Join the vertex names with "->"
    string path_str;
    for (Vertex *v : p.path) {
      path_str += v->name;
      if (v != p.path.back()) {
        path_str += "->";
      }
    }

    // Set alignment and column widths for data
    cout << setw(path_col_width) << left << path_str << setw(length_col_width)
         << p.total_distance << setw(cost_col_width) << p.total_cost << endl;
  }
}

void print_Flight_Path_LIMITED(string n1, string n2, Flight_Path path,
                               int limit) {
  // checks if path is empty
  if (path.path.empty()) {
    cout << "Shortest route from " << n1 << " to " << n2 << " in " << limit
         << " stops: None\n";
    return;
  }

  // path exists
  if (path.total_distance > 0) {
    cout << "Shortest route from " << n1 << " to " << n2 << " in " << limit
         << " stops: ";
    for (Vertex *v : path.path) {
      if (v->name != n2) {
        cout << v->name << "->";
      } else {
        cout << v->name;
        ;
      }
    }
  }
  cout << ". The length is " << path.total_distance << ". The cost is "
       << path.total_cost << ".\n\n";
}

void print_Flight_COUNT(Graph *&g, vector<string> vn) {
  Vertex *vertex = g->root;

  if (vertex == nullptr || vn.size() == 0) {
    cout << "No valid airport/names";
    return;
  }

  const int airport_col_width = 10; // for path names
  cout << setw(airport_col_width) << left << "Airport";
  cout << "Connections" << endl;
  for (string s : vn) {
    cout << setw(airport_col_width) << left << s;
    cout << CountFlightPaths(vertex, s) << endl;
  }
}

Edge* minimumEdge(vector<Edge*>& edges, vector<bool>& visited) {

    Edge *minEdge = nullptr;

    //search the edges vector
    for (auto &edge: edges) {
        //if the id of the destination of the edge is not visited yet
        if (!visited[edge->end->id]) {
            //if minEdge has not been assigned, automatically assign the first one
            //or if the cost of the current minEdge is higher than the one we just found
            if (minEdge == nullptr || edge->cost <= minEdge->cost) {
                //mark as new minEdge
                minEdge = edge;
            }
        }
    }
    return minEdge;
}


void treeToVectorUtility(Vertex* root,vector<Vertex*>& vec){
    if(root == nullptr){
        return;
    }

    vec.push_back(root);

    treeToVectorUtility(root->left,vec);
    treeToVectorUtility(root->right,vec);
}


//Puts pointer of Vertexes from AVL into a vector
vector<Vertex*> treeToVector(Vertex* root){
    vector<Vertex*> vec;
    treeToVectorUtility(root,vec);
    return vec;
}

void printPrim(Vertex* root){

    vector<Vertex*> vec = treeToVector(root);
    vector<bool> visited(INT_MAX,false);
    int total_cost = 0;
    int numVertices = 0;
    int numEdges = 0;

    cout<<"[6th task]"<<endl;
    cout<<"Edge\t\tWeight"<<endl;
    for(auto& vertex : vec){
        numVertices++;
        for(auto& edge : *vertex->edges){
            if(!visited[edge.end->id]){
                cout<<edge.start->name<<" - "<<edge.end->name<<"\t\t"<<edge.cost<<endl;
                visited[edge.end->id] = true;
                numEdges++;
                total_cost += edge.cost;
            }
        }
    }
    cout<<"Number of Vertices: "<<numVertices<<"\tNumber of Edges: "<<numEdges<<endl;
    cout<<"Total Cost of MST: "<<total_cost<<endl;
    if(numEdges == numVertices -1){
        cout<<"MST is connected"<<endl;
    }
    else
        cout<<"MST is not connected"<<endl;
}

void printKru(Vertex* root){

    vector<Vertex*> vec = treeToVector(root);
    vector<bool> visited(INT_MAX,false);
    int total_cost = 0;
    int numVertices = 0;
    int numEdges = 0;

    cout<<"[7th Task]"<<endl;
    cout<<"Edge\t\tWeight"<<endl;
    for(auto& vertex : vec){
        numVertices++;
        for(auto& edge : *vertex->edges){
            if(!visited[edge.end->id]){
                if(!visited[(edge.start->id+edge.end->id)*100]){
                    cout << edge.start->name << " - " << edge.end->name << "\t" << edge.cost << endl;
                    visited[edge.end->id] = true;
                    total_cost += edge.cost;
                    visited[(edge.start->id+edge.end->id)*100] = true;
                    numEdges++;
                }
            }
        }
    }
    
    cout<<"Total Cost of MST: "<<total_cost<<endl;
    if(numEdges == numVertices - 1){
        cout<<"MST is connected"<<endl;
    }
    else
        cout<<"MST is not connected"<<endl;

}


void primAlgorithm(Graph* &graph, Graph* &prim) {

    //Number of vertices
    int V = graph->size;

    //Vector of visited vertices, searchable using ID
    vector<bool> visited(100000, false);


    //Vector of actual visited vertexes
    vector<Vertex *> visit(V, nullptr);

    //Starting vertex
    Vertex *start = prim->addVertex(graph->root->name);


    //mark vertex as visited using id as index
    visited[graph->root->id] = true;

    //add vertex to visit as id as index
    visit[graph->root->id] = start;

    //keeps track of size of MST
    int mstSize = 1;

    //vector of edges that we need to traverse
    vector<Edge *> edges;

    //Add all the edges of starting vertex to vector
    for (auto &edge: *graph->root->edges) {
        auto *star = new Vertex(edge.start->name, edge.start->id);
        auto *end = new Vertex(edge.end->name, edge.end->id);
        edges.push_back(new Edge(star, end, 0, edge.cost));

    }


    while (prim->size <= graph->size) {

        //find the minimum cost edge that is not visited yet
        Edge *minEdge = minimumEdge(edges, visited);

        if (minEdge == nullptr)
            break;

        //cout<<"Min Edge: "<<minEdge->end->name<<endl;
        //Mark vertex as visited
        visited[minEdge->end->id] = true;
        //visited[(minEdge->end->id+minEdge->start->id)*100] = true; //avoids disconnected graph

        //Add vertex to MST
        Vertex *newVertex = prim->addVertex(minEdge->end->name);

        //add vertex to visited Vertex vector
        visit[minEdge->end->id] = newVertex;

        //find the beginning Vertex which should already be in the MST
        Vertex *begin = find(prim->root, minEdge->start->name);

        // Add adge to MST
        prim->addEdge(begin, newVertex, 0, minEdge->cost);

        //Add other edge, since undirected are bidirectional regardless if there is an actual flight to the destination and back
       // prim->addEdge(newVertex, begin, 0, minEdge->cost);

        //cout<<"Start: "<<begin->name<<"\tDestination: "<<newVertex->name<<"\tCost: "<<minEdge->cost<<endl;
        mstSize++; // increase size of MST

        //find the vertex in the original undirected graph so we can add its edges
        //If we try to use the edge that we found in the vector of edges, the edges vector will be null
        Vertex *originalVertex = find(graph->root, minEdge->end->name);

        //search the vertex that we just added through its edges
        for (auto &edge: *originalVertex->edges) {
            bool exists = false;
            for (auto &vertex: visit) {
                //if the vertex is not null and the destination of edge is equal to the vertex we are on, it means that the
                //edge already exists in our vector
                if (vertex != nullptr && edge.end->name == vertex->name) {
                    exists = true;
                    break;
                }
            }
            //if the edge doesn't exist, we add it
            if (!exists) { // Avoid self-loops
                auto *end = new Vertex(edge.end->name, edge.end->id);
                edges.push_back(new Edge(newVertex, end, 0, edge.cost));
            }
        }

    }

    cout << "----------------------------" << endl;

    printPrim(prim->root);


}

void edgeUtilityFunction(vector<Edge>& vec , Vertex*root){
    if(root == nullptr)
        return;

    for(auto& edge : *root->edges){
        vec.push_back(edge);
    }
    edgeUtilityFunction(vec,root->left);
    edgeUtilityFunction(vec,root->right);

}
vector<Edge> vertexToEdgeVector(Vertex* root){
    vector<Edge> edges;
    edgeUtilityFunction(edges,root);
    return edges;
}

//Comparator function
bool compareEdges(const Edge& a, const Edge& b){
    return a.cost < b.cost; // Ascending order
}

void sortEdges(vector<Edge>& edges) {
    std::sort(edges.begin(), edges.end(), compareEdges);
}
void kruskalAlgorithm(Graph*graph, Graph* kruskal){

    //Vector of edges;
    vector<Edge> edges = vertexToEdgeVector(graph->root);
    sortEdges(edges);
    vector<bool> visited(100000000,false);



    //if the edge start and end are both either a destination or source, skip them

    Vertex* start = kruskal->addVertex(edges[0].start->name);
    Vertex* end = kruskal->addVertex(edges[0].end->name);
    kruskal->addEdge(start,end,0,edges[0].cost);
    kruskal->addEdge(end,start,0,edges[0].cost);
    visited[edges[0].start->id] = true;
    visited[edges[0].end->id] = true;
    visited[(edges[0].start->id+edges[0].end->id)*100] = true; // add both ids, so i can track if the exact edge is already in the MST

    for(auto& edge : edges){
        if((visited[edge.start->id] && !visited[edge.end->id]) || (!visited[edge.start->id] && visited[edge.end->id])){
            if(!visited[(edge.start->id+edge.end->id)*100]) {
                Vertex *start1 = kruskal->addVertex(edge.start->name);
                Vertex *end1 = kruskal->addVertex(edge.end->name);
                kruskal->addEdge(start1, end1, 0, edge.cost);
                kruskal->addEdge(end1, start1, 0, edge.cost);
                visited[edge.start->id] = true;
                visited[edge.end->id] = true;
                visited[(edge.start->id + edge.end->id)*100] = true;
            }
        }
    }

    cout<<"Kruskal Algorithm"<<endl;
    printKru(kruskal->root);
}