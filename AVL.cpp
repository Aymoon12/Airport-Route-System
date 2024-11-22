// Included libraries
#include <iostream>
#include <string>
#include "AVL.h"


// using stuff
using std::cout; using std::endl; using std::max; using std::string;

/*
* @name: height
* @param1: Node* - a node in the tree
* @return: int - the height of the node
* @description: Retuns the height of a given node in a tree
*/
int height(Vertex *N) {
  // base case (there are no nodes, its empty)
  if (N == nullptr) {
    return 0;
  }

  // returns the larger of the two heights on either side
  return max(height(N->left), height(N->right)) + 1;
}

/*
* @name: getBalance
* @param1: Node* - a node in the tree
* @return: int - the balance of the node
* @description: Returns the balance (aka the difference in heights) of a given node 
*/
int getBalance(Vertex *N) {
  return height(N->left) - height(N->right); // returns height difference
}

/*
* @name: rightRotate
* @param1: Node* - a node in the tree
* @return: Node* - the new root of the original node
* @description: Used for AVL organization; rotates a given node and its components to the right
*/
Vertex*rightRotate(Vertex *y) {
  // variables
  Vertex*x = y->left;
  Vertex*T2 = x->right;

  // rotate
  x->right = y;
  y->left = T2;

  // updates height
  y->height = max(height(y->left), height(y->right)) + 1;
  x->height = max(height(x->left), height(x->right)) + 1;

  // return new root
  return x;
}

/**
* @name: leftRotate
* @param1: Node* - a node in the tree
* @return: Node* - the new root of the original node
* @description: Used for AVL organization; rotates a given node and its components to the left
**/
Vertex*leftRotate(Vertex *x) {
  // variables
  Vertex*y = x->right;
  Vertex*T2 = y->left;

  // rotate
  y->left = x;
  x->right = T2;

  // updates height
  y->height = max(height(y->left), height(y->right)) + 1;
  x->height = max(height(x->left), height(x->right)) + 1;

  // returns new root
  return y;
}

/*
* @name: insert
* @param1: Node* - root of the tree
* @param2: string - the name of the state
* @param3: int - value to be inserted
* @return: Node* - the new root of the tree
* @description: Inserts a new node into a tree, using AVL to keep balanced
*/
Vertex*insert(Vertex *node, string value, string state, int id) {
  // performs normal BST ROTATIONS
  if (node == nullptr) {
    return new Vertex(value, state, id);
  }

  // handles where to create the node
  if (value < node->name) {
    node->left = insert(node->left, value, state, id);
  } else if (value > node->name) {
    node->right = insert(node->right, value, state, id);
  } else {
    return node;
  }

  //
  node->height = max(height(node->left), height(node->right)) + 1;
  int balance = getBalance(node);
  //

  // if the node becomes unbalanced
  // LL
  if (balance > 1 && value < node->left->name) {
    return rightRotate(node);
  }

  // RR
  if (balance < -1 && value > node->right->name) {
    return leftRotate(node);
  }

  // LR
  if (balance > 1 && value < node->left->name) {
    node->left = leftRotate(node->left);
    return rightRotate(node);
  }

  // RL
  if (balance < -1 && value > node->right->name) {
    node->right = rightRotate(node->right);
    return leftRotate(node);
  }

  // returns the node
  return node;
}

//Overloaded
/*
* @name: insert
* @param1: Node* - root of the tree
* @param2: int - value to be inserted
* @return: Node* - the new root of the tree
* @description: Inserts a new node into a tree, using AVL to keep balanced
*/
Vertex*insert(Vertex *node, string value, int id) {
  // performs normal BST ROTATIONS
  if (node == nullptr) {
    return new Vertex(value, id);
  }

  // handles where to create the node
  if (value < node->name) {
    node->left = insert(node->left, value, id);
  } else if (value > node->name) {
    node->right = insert(node->right, value,  id);
  } else {
    return node;
  }

  //
  node->height = max(height(node->left), height(node->right)) + 1;
  int balance = getBalance(node);
  //

  // if the node becomes unbalanced
  // LL
  if (balance > 1 && value < node->left->name) {
    return rightRotate(node);
  }

  // RR
  if (balance < -1 && value > node->right->name) {
    return leftRotate(node);
  }

  // LR
  if (balance > 1 && value < node->left->name) {
    node->left = leftRotate(node->left);
    return rightRotate(node);
  }

  // RL
  if (balance < -1 && value > node->right->name) {
    node->right = rightRotate(node->right);
    return leftRotate(node);
  }

  // returns the node
  return node;
}


/*
* @name: find
* @param1: Vertex* - root of the tree
* @param2: string - name to be searched for
* @return: Vertex* - the found vertex node
* @description: Searches for a specific vertex node in the AVL tree
*/
Vertex* find(Vertex *node, string name) {
  // first check
  if (node == nullptr) {
    // cout << "Not Found: " << name << endl;
    return nullptr;
  }

  // checks if the name is the same
  if (node->name == name) {
    return node;
  }

  // checks where to look based on value
  if (node->name > name) {
    return find(node->left, name);
  } else {
    return find(node->right, name);
  }
}