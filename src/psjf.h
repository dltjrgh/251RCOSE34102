#ifndef PSJF_H
#define PSJF_H

#include "gantt.h"
#include "pool.h"
#include "queue.h"
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

typedef Heap PSJFRqueue;

int psjf_compare(Process *a, Process *b);

PSJFRqueue *psjf_create_rqueue(int capacity);

typedef struct PSJF_state {
  Pool *pool;
  int current_time;
  Process *running_process;
  int running_since;
  Queue *waiting_queue;
  PSJFRqueue *ready_queue;
  Queue *terminated_queue;
  GanttChart *gantt;
} PSJFState;

PSJFState *psjf_init(Pool *pool, int rqueue_capacity);

void execute_psjf(PSJFState *state);

#endif // PSJF_H
