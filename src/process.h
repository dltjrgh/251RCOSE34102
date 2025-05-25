#ifndef PROCESS_H
#define PROCESS_H

#include "queue.h"
#include <stdio.h>
#include <stdlib.h>

typedef struct workload {
  int is_cpu; // 1 for CPU-bound, 0 for I/O-bound
  int duration;
} Workload;

// A single process with attributes
typedef struct process {
  int pid;
  int arrival_time;
  int priority;     // Lower number means higher priority
  Queue *workloads; // Pointer to the first workload in the linked list
} Process;

// Create a new Process structure
Process *createProcess(int pid, int arrival_time, int priority,
                       Queue *workloads);

// Add a workload to the queue of workloads
void addWorkload(Queue *workloads, int duration);

// Cosume timestep t for the process
void consumeTime(Process *process, int t);

// Remove the first workload from the process
void removeWorkload(Process *process);

// Print a Process structure
void printProcess(Process *process);

#endif
