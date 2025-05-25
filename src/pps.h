#ifndef PPS_H
#define PPS_H

#include "gantt.h"
#include "pool.h"
#include "queue.h"
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

typedef Heap PPSRqueue;

int pps_compare(Process *a, Process *b);

PPSRqueue *pps_create_rqueue(int capacity);

typedef struct PPS_state {
  Pool *pool;
  int current_time;
  Process *running_process;
  int running_since;
  Queue *waiting_queue;
  PPSRqueue *ready_queue;
  Queue *terminated_queue;
  GanttChart *gantt;
} PPSState;

PPSState *pps_init(Pool *pool, int rqueue_capacity);

void execute_pps(PPSState *state);

#endif // PPS_H
