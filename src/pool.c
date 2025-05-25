#include "pool.h"

// Initialize Pool
Pool *create_pool(int capacity) {
  Pool *pool = create_heap(capacity, compare_processes);
  if (!pool) {
    fprintf(stderr, "Failed to allocate memory for pool\n");
    exit(EXIT_FAILURE);
  }

  pool->capacity = capacity;
  pool->size = 0;
  pool->data = (Process **)malloc(capacity * sizeof(Process *));

  if (!pool->data) {
    fprintf(stderr, "Failed to allocate memory for heap array\n");
    free(pool);
    exit(EXIT_FAILURE);
  }

  return pool;
}

int compare_processes(Process *a, Process *b) {
  if (a->arrival_time < b->arrival_time) {
    return -1;
  } else if (a->arrival_time > b->arrival_time) {
    return 1;
  } else {
    if (a->pid < b->pid) {
      return -1;
    } else if (a->pid > b->pid) {
      return 1;
    } else {
      return 0;
    }
  }
}

int get_min_arrival_time(Pool *pool) {
  if (pool->size == 0) {
    return -1; // No processes in the pool
  }
  return pool->data[0]->arrival_time;
}

// Duplicate the pool
Pool *duplicate_pool(Pool *pool) {
  Pool *new_pool = create_pool(pool->capacity);

  for (int i = 0; i < pool->size; i++) {
    Process *original_process = pool->data[i];
    Process *duplicated_process = duplicate_process(original_process);

    if (duplicated_process != NULL) {
      insert_heap(new_pool, duplicated_process);
    } else {
      fprintf(stderr, "Failed to duplicate process PID: %d\n",
              original_process->pid);
      free_heap(new_pool);
      exit(EXIT_FAILURE);
    }
  }

  return new_pool;
}
