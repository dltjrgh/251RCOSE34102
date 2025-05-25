#ifndef PLISC_H
#define PLISC_H

#include "gantt.h"
#include "pool.h"
#include "queue.h"
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

typedef Heap PLISCRqueue;

int plisc_compare(Process *a, Process *b);

PLISCRqueue *plisc_create_rqueue(int capacity);

typedef struct PLISC_state {
  Pool *pool;
  int current_time;
  Process *running_process;
  int running_since;
  Queue *waiting_queue;
  PLISCRqueue *ready_queue;
  Queue *terminated_queue;
  GanttChart *gantt;
} PLISCState;

PLISCState *plisc_init(Pool *pool, int rqueue_capacity);

void execute_plisc(PLISCState *state);

#endif // PLISC_H
