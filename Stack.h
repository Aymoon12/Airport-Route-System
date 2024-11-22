// included libraries
#include <iostream>

#ifndef STACK_H
#define STACK_H
//

// pre-defined variables
#define MAX 100

// Stack Class
template<typename T>
class Stack{
  int t;
public:
  // array
  T arr[MAX];
  // constructors
  Stack<T>();
  // functions
  void push(T x);
  int t_i();
  T pop();
  T top();
  bool empty();
};

//
#endif