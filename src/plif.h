#ifndef PLIF_H
#define PLIF_H

#include "gantt.h"
#include "pool.h"
#include "queue.h"
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

typedef Heap PLIFRqueue;

int plif_compare(Process *a, Process *b);

PLIFRqueue *plif_create_rqueue(int capacity);

typedef struct PLIF_state {
  Pool *pool;
  int current_time;
  Process *running_process;
  int running_since;
  Queue *waiting_queue;
  PLIFRqueue *ready_queue;
  Queue *terminated_queue;
  GanttChart *gantt;
} PLIFState;

PLIFState *plif_init(Pool *pool, int rqueue_capacity);

void execute_plif(PLIFState *state);

#endif // PLIF_H
