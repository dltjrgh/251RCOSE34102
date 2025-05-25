#ifndef PS_H
#define PS_H

#include "gantt.h"
#include "pool.h"
#include "queue.h"
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

typedef Heap PSRqueue;

int ps_compare(Process *a, Process *b);

PSRqueue *ps_create_rqueue(int capacity);

typedef struct PS_state {
  Pool *pool;
  int current_time;
  Process *running_process;
  int running_since;
  Queue *waiting_queue;
  PSRqueue *ready_queue;
  Queue *terminated_queue;
  GanttChart *gantt;
} PSState;

PSState *ps_init(Pool *pool, int rqueue_capacity);

void execute_ps(PSState *state);

#endif // PS_H
