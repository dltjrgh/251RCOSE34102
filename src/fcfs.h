#ifndef FCFS_H
#define FCFS_H

#include "gantt.h"
#include "pool.h"
#include "queue.h"
#include <stdio.h>
#include <stdlib.h>

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

// Proceed to the next time step in the FCFS scheduling
void fcfs_next_step(FCFSState *state);

void execute_fcfs(FCFSState *state);

#endif // FCFS_H
