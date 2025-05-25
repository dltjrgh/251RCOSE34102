#ifndef POOL_H
#define POOL_H

#include "heap.h"
#include "process.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef Heap Pool;

// Initialize a new Pool structure
Pool *create_pool(int capacity);

int compare_processes(Process *a, Process *b);

// Return the arrival_time for the minimum process in the pool (-1 if empty)
int get_min_arrival_time(Pool *pool);

// Duplicate a pool
Pool *duplicate_pool(Pool *pool);

#endif // POOL_H
