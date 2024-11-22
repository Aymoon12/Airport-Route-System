// Included libraries
#include <iostream>
#include "Queue.h"
#include "Core_Functions.h"

#define INT_MAX 1000

// using stuff
using std::cout;

// Constructor
template<typename T>
PriorityQueue<T>::PriorityQueue(int cap) {
  capacity = cap;
  heapSize = 0;
  harr = new T[capacity + 1]; // Allocate memory for the heap array
}

// Destructor
template<typename T>
PriorityQueue<T>::~PriorityQueue() {
  delete[] harr; // Deallocate memory for the heap array
}

/**
 * @brief swaps the values of two elements in the heap
 * @param (T x) value 1
 * @param (T y) value 2
 * @return void
**/
template<typename T>
void PriorityQueue<T>::swap(T &x, T &y) {
  T temp = x;
  x = y;
  y = temp;
}

/**
 * @brief precolates up a specific index
 * @param (int index) the index of the element to be percolated up
 * @return void
**/
template<typename T>
void PriorityQueue<T>::percolateUp(int index) {
  while (index > 1 && harr[index / 2] > harr[index]) {
    swap(harr[index], harr[index / 2]);
    index /= 2;
  }
}

/**
 * @brief precolates down a specific index
 * @param (int index) the index of the element to be percolated down
 * @return void
**/
template<typename T>
void PriorityQueue<T>::percolateDown(int index) {
  int left = 2 * index;
  int right = 2 * index + 1;
  int smallest = index;

  if (left <= heapSize && harr[left] < harr[smallest])
    smallest = left;
  if (right <= heapSize && harr[right] < harr[smallest])
    smallest = right;

  if (smallest != index) {
    swap(harr[index], harr[smallest]);
    percolateDown(smallest);
  }
}

/**
 * @brief pushes a value (x) to the top of the queue
 * @param (T x): the value being pushed
 * @return void
**/
template<typename T>
void PriorityQueue<T>::push(T k) {
  if (heapSize == capacity) {
    cout << "Overflow: Could not insertKey\n";
    return;
  }

  heapSize++;
  harr[heapSize] = k; // Insert the new key at the end
  percolateUp(heapSize); // Fix the min-heap property if violated
}

/**
 * @brief extracts the minimum element from the priority queue
 * @return the element
**/
template<typename T>
T PriorityQueue<T>::extractMin() {
  if (heapSize == 0) {
    cout << "Underflow: Priority queue is empty\n";
    throw std::underflow_error("Priority queue is empty");
  }

  T minElement = harr[1]; // Store the minimum element
  harr[1] = harr[heapSize]; // Move the last element to the root
  heapSize--; // Reduce the heap size
  percolateDown(1); // Fix the min-heap property if violated
  return minElement; // Return the minimum element
}

/**
 * @brief checks if the queue is empty or not
 * @return (bool) whether or not the queue is empty
**/
template<typename T>
bool PriorityQueue<T>::isEmpty() const {
  return (heapSize == 0);
}

// exit headers
template class PriorityQueue<Edge>;
template class PriorityQueue<int>;
template class PriorityQueue<Flight_Path>;