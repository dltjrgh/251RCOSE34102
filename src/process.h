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
  int io_time;    // Total I/O time consumed
  int wait_time;  // Total wait time in the queue
  Queue *workloads; // Pointer to the first workload in the linked list
} Process;

// Create a new Process structure
Process *create_process(int pid, int arrival_time, int priority,
                       Queue *workloads);

int remaining_time(Process *process);

int remaining_io_time(Process *process);

// Add a workload to the queue of workloads
void add_workload(Queue *workloads, int duration);

// Remove the first workload from the process
void remove_workload(Process *process);

// Print a Process structure
void print_process(Process *process);

Workload *duplicate_workload(Workload *original);

Queue *duplicate_queue(Queue *original);

Process *duplicate_process(Process *original);

#endif
