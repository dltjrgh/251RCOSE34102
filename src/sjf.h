#ifndef SJF_H
#define SJF_H

#include "gantt.h"
#include "pool.h"
#include "queue.h"
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

typedef Heap SJFRqueue;

int sjf_compare(Process *a, Process *b);

SJFRqueue *sjf_create_rqueue(int capacity);

typedef struct SJF_state {
  Pool *pool;
  int current_time;
  Process *running_process;
  int running_since;
  Queue *waiting_queue;
  SJFRqueue *ready_queue;
  Queue *terminated_queue;
  GanttChart *gantt;
} SJFState;

SJFState *sjf_init(Pool *pool, int rqueue_capacity);

void execute_sjf(SJFState *state);

#endif // SJF_H
