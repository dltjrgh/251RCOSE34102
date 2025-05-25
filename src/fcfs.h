#ifndef FCFS_H
#define FCFS_H

#include "gantt.h"
#include "pool.h"
#include "queue.h"
#include <stdio.h>
#include <stdlib.h>

typedef struct FCFSRQueue {
  Process *process;
  struct FCFSQueue *next;
} FCFSRQueue;

typedef struct FCFSWQueue {
  Process *process;
  struct FCFSWQueue *next;
} FCFSWQueue;

typedef struct FCFS_state {
  Pool *pool;
  int current_time;
  Process *running_process;
  int running_since;
  Queue *ready_queue;
  Queue *waiting_queue;
  Queue *terminated_queue;
  GanttChart *gantt;
} FCFSState;

// Initialize the FCFS state
FCFSState *fcfs_init(Pool *pool);

// Add a process to the ready queue
void fcfs_add_ready(FCFSState *state, Process *process);

// Add a process to the waiting queue
void fcfs_add_waiting(FCFSState *state, Process *process);

// Remove a process from the ready queue
Process *fcfs_remove_ready(FCFSState *state);

// Remove a process from the waiting queue
Process *fcfs_remove_waiting(FCFSState *state);

// Consume time 1 unit for the waiting process
void fcfs_consume_time_waiting(FCFSState *state);

// Proceed to the next time step in the FCFS scheduling
void fcfs_next_step(FCFSState *state);

void execute_fcfs(FCFSState *state);

#endif // FCFS_H
