#ifndef HRN_H
#define HRN_H

#include "gantt.h"
#include "pool.h"
#include "queue.h"
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

typedef Heap HRNRqueue;

int hrn_compare(Process *a, Process *b);

HRNRqueue *hrn_create_rqueue(int capacity);

typedef struct HRN_state {
  Pool *pool;
  int current_time;
  Process *running_process;
  int running_since;
  Queue *waiting_queue;
  HRNRqueue *ready_queue;
  Queue *terminated_queue;
  GanttChart *gantt;
} HRNState;

HRNState *hrn_init(Pool *pool, int rqueue_capacity);

void execute_hrn(HRNState *state);

#endif // HRN_H
