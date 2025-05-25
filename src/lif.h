#ifndef LIF_H
#define LIF_H

#include "gantt.h"
#include "pool.h"
#include "queue.h"
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

typedef Heap LIFRqueue;

int lif_compare(Process *a, Process *b);

LIFRqueue *lif_create_rqueue(int capacity);

typedef struct LIF_state {
  Pool *pool;
  int current_time;
  Process *running_process;
  int running_since;
  Queue *waiting_queue;
  LIFRqueue *ready_queue;
  Queue *terminated_queue;
  GanttChart *gantt;
} LIFState;

LIFState *lif_init(Pool *pool, int rqueue_capacity);

void execute_lif(LIFState *state);

#endif // LIF_H
