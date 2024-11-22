// Inlcuded libraries
#include <iostream>

#ifndef QUEUE_H
#define QUEUE_H

template<typename T>
class PriorityQueue {
private:
  T *harr; // pointer to array of elements in heap
  int capacity; // maximum possible size of min heap
  int heapSize; // Current number of elements in min heap

  // Helper functions
  void swap(T &x, T &y);
  void percolateUp(int index);
  void percolateDown(int index);
  T minimum(T a, int indexa, T b, int indexb);

public:
  PriorityQueue(int cap); // Constructor
  ~PriorityQueue(); // Destructor

  // Insert a key into the priority queue
  void push(T k);

  // Extract the minimum (top) element from the priority queue
  T extractMin();

  // Check if the priority queue is empty
  bool isEmpty() const;
};


#endif