#ifndef RR_H
#define RR_H

#include "gantt.h"
#include "pool.h"
#include "queue.h"
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

typedef struct RR_state {
  Pool *pool;
  int current_time;
  Process *running_process;
  int running_since;
  int quantum;
  int remaining_quantum;
  Queue *ready_queue;
  Queue *waiting_queue;
  Queue *terminated_queue;
  GanttChart *gantt;
} RRState;

// Initialize the RR state
RRState *rr_init(Pool *pool, int quantum);

void execute_rr(RRState *state);

#endif // RR_H
