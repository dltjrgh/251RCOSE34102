#ifndef PSA_H
#define PSA_H

#include "gantt.h"
#include "pool.h"
#include "queue.h"
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

typedef Heap PSARqueue;

int psa_compare(Process *a, Process *b);

PSARqueue *psage_create_rqueue(int capacity);

typedef struct PSA_state {
  Pool *pool;
  int current_time;
  Process *running_process;
  int running_since;
  Queue *waiting_queue;
  PSARqueue *ready_queue;
  Queue *terminated_queue;
  GanttChart *gantt;
} PSAState;

PSAState *psa_init(Pool *pool, int rqueue_capacity);

void execute_psa(PSAState *state);

#endif // PSA_H
