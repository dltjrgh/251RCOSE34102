#ifndef LISC_H
#define LISC_H

#include "gantt.h"
#include "pool.h"
#include "queue.h"
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

typedef Heap LISCRqueue;

int lisc_compare(Process *a, Process *b);

LISCRqueue *lisc_create_rqueue(int capacity);

typedef struct LISC_state {
  Pool *pool;
  int current_time;
  Process *running_process;
  int running_since;
  Queue *waiting_queue;
  LISCRqueue *ready_queue;
  Queue *terminated_queue;
  GanttChart *gantt;
} LISCState;

LISCState *lisc_init(Pool *pool, int rqueue_capacity);

void execute_lisc(LISCState *state);

#endif // LISC_H
