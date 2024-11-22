// included libraries
#include <iostream>
#include "Stack.h"
#include "Graph.h"

/**
 * @brief sets up a new Stack::Stack objects
**/
template<typename T>
Stack<T>::Stack() {
  this->t = 0;
}

/**
 * @brief pushes a value (x) to the top of the stack's array list
 * @param (T x): the value being pushed
 * @return void
**/
template<typename T>
void Stack<T>::push(T x){
  // case: overflowing
  if(t >= MAX){
    std::cout<<"Stack Overflow\n";
    return;
  }

  // adds the new value to the array
  arr[++t] = x;
}

/**
 * @brief removes the top value from the stack's array list
 * @param void
 * @return (T): the value being popped
**/
template<typename T>
T Stack<T>::pop(){
  // case: underflowing
  if(empty()){
    std::cout<<"Stack is empty\n";
    return T{};
  }

  // pops and removes the value from the array (moves the top of the array below the value)
  T popped = arr[t];
  t--;
  return popped;
}

/**
 * @brief looks at the top value in the array
 * @param void
 * @return (T): the value being looked at
**/
template<typename T>
T Stack<T>::top(){
  // case: underflowing
  if(empty()){
    std::cout<<"Stack is empty\n";
    return T{};
  }

  // returns the top value
  return arr[t];

  
}

/**
 * @brief checks if the stack is empty or not
 * @param void
 * @return (bool): if its empty or not
**/
template<typename T>
bool Stack<T>::empty(){
  return t == 0;
}

/**
 * @brief returns the current top value (index) of the stack
 * @param void
 * @return (T): the value of (top)
**/
template<typename T>
int Stack<T>::t_i(){
  return t;
}

// exit headers
template class Stack<Vertex*>;