// included libraries
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

//#include "Core_Functions.h"
#include "AVL.h"
#include "Core_Functions.h"
#include "Graph.h"

// using stuff
using std::cout;
using std::endl;
using std::getline;
using std::ifstream;
using std::ofstream;
using std::string;


// main function
int main() {
  // file stuff
  ifstream input_file("airports.txt");

  // graph stuff
  Graph *g = new Graph(1000);
  Create_File_Graph(input_file, g);

  //2nd Task
  cout << "[2nd Task]" << endl;
  string n1 = "ATL";
  string n2 = "MIA";
  print_Flight_Path_SHORT(n1, n2, Shortest_Flight_Path(g, n1, n2));

  // 3rd Task
  cout << "\n[3rd Task]" << endl;
  string n3 = "ATL";
  string s1 = "FL";
  print_Flight_Paths_STATE(n3, s1, Shortest_Flight_Paths_State(g, n3, s1));

  // 4th Task
  string n4 = "IAD";
  string n5 = "MIA";
  string n6 = "PIT";
  string n7 = "ACT";
  int k1 = 3;
  int k2 = 2;
  cout << "\n\n[4th Task]" << endl;
  print_Flight_Path_LIMITED(n4, n5, Shortest_Flight_Path_Limited(g, n4, n5, k1), k1);
  print_Flight_Path_LIMITED(n6, n7, Shortest_Flight_Path_Limited(g, n6, n7, k2), k2);

  // 5th Task
  cout << "\n\n[5th Task]" << endl;
  vector<string> vn = {"ATL", "ORD", "MIA"};
  print_Flight_COUNT(g, vn);

  // 6th Task
  Graph *undirected = new Graph(1000);
  directedToUndirected(g->root, undirected);

  //7th task
  auto* primAlgo = new Graph(1000);
  primAlgorithm(undirected,primAlgo);
  

  //Another test
  //auto* test = new Graph(100);
  //Vertex* A = test->addVertex("A");
  //Vertex* B = test->addVertex("B");
  //Vertex* C = test->addVertex("C");
  //Vertex* D = test->addVertex("D");
  //Vertex* E = test->addVertex("E");

  //test->addEdge(A,C,0,3);
  //test->addEdge(C,B,0,10);
  //test->addEdge(C,E,0,6);
  //test->addEdge(C,D,0,2);
  //test->addEdge(B,D,0,4);
  //test->addEdge(D,E,0,1);
  //auto* testUndirected = new Graph(100);
  //directedToUndirected(test->root,testUndirected);

  //auto* testPrim = new Graph(100);

  //primAlgorithm(testUndirected,testPrim);

  cout<<"\n\n"<<endl;
  //8th task
  auto* kurAlgo = new Graph(1000);
  kruskalAlgorithm(undirected,kurAlgo);
  
  // removes allocated memory
  delete g;
  delete undirected;
  delete primAlgo;
  delete kurAlgo;

  // terminates the pogram
  return 0;
}