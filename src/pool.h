#ifndef POOL_H
#define POOL_H

#include "process.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// pool of processes implemented as a min-heap
typedef struct pool {
  Process **heap_array;
  int size;     // Current heap size
  int capacity; // Maximum heap size
} Pool;

// Initialize a new Pool structure
Pool *createPool(int capacity);

Process *extractMinProcessFromPool(Pool *pool);

void minHeapifyPool(Pool *pool, int i);

// Add a process to the end of the pool
void insertProcessToPool(Pool *pool, Process *process);

// Helpers for min-heap operations
int getParent(int i);
int getLeftChild(int i);
int getRightChild(int i);
void swapProcesses(Process **a, Process **b);
int compareProcesses(const Process *a, const Process *b);

// Return the arrival_time for the minimum process in the pool (-1 if empty)
int getMinArrivalTime(Pool *pool);

// Print the details of a pool
void printPool(Pool *pool);

// Duplicate a pool
Pool *duplicatePool(Pool *pool);

// Free the entire Pool structure and all its processes
void freePool(Pool *pool);

#endif // POOL_H
