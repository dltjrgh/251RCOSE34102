#include "pool.h"

// Initialize Pool
Pool *createPool(int capacity) {
  Pool *pool = (Pool *)malloc(sizeof(Pool));
  if (!pool) {
    fprintf(stderr, "Failed to allocate memory for pool\n");
    exit(EXIT_FAILURE);
  }

  pool->capacity = capacity;
  pool->size = 0;
  pool->heap_array = (Process **)malloc(capacity * sizeof(Process *));

  if (!pool->heap_array) {
    fprintf(stderr, "Failed to allocate memory for heap array\n");
    free(pool);
    exit(EXIT_FAILURE);
  }

  return pool;
}

Process *extractMinProcessFromPool(Pool *pool) {
  if (pool->size <= 0) {
    return NULL;
  }
  if (pool->size == 1) {
    pool->size--;
    return pool->heap_array[0];
  }

  Process *root = pool->heap_array[0];
  pool->heap_array[0] = pool->heap_array[pool->size - 1];
  pool->size--;
  minHeapifyPool(pool, 0);

  return root;
}

void minHeapifyPool(Pool *pool, int i) {
  int smallest = i;
  int left = getLeftChild(i);
  int right = getRightChild(i);

  if (left < pool->size && compareProcesses(pool->heap_array[left],
                                            pool->heap_array[smallest]) < 0) {
    smallest = left;
  }
  if (right < pool->size && compareProcesses(pool->heap_array[right],
                                             pool->heap_array[smallest]) < 0) {
    smallest = right;
  }
  if (smallest != i) {
    swapProcesses(&pool->heap_array[i], &pool->heap_array[smallest]);
    minHeapifyPool(pool, smallest);
  }
}

// Add a process to the pool
void insertProcessToPool(Pool *pool, Process *process) {
  if (pool->size == pool->capacity) {
    fprintf(stderr, "Pool is full, cannot insert process\n");
    return;
  }

  pool->size++;
  int i = pool->size - 1;
  pool->heap_array[i] = process;

  while (i != 0 && compareProcesses(pool->heap_array[getParent(i)],
                                    pool->heap_array[i]) > 0) {
    swapProcesses(&pool->heap_array[i], &pool->heap_array[getParent(i)]);
    i = getParent(i);
  }
}

int getParent(int i) { return (i - 1) / 2; }

int getLeftChild(int i) { return 2 * i + 1; }

int getRightChild(int i) { return 2 * i + 2; }

void swapProcesses(Process **a, Process **b) {
  Process *temp = *a;
  *a = *b;
  *b = temp;
}

int compareProcesses(const Process *a, const Process *b) {
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

int getMinArrivalTime(Pool *pool) {
  if (pool->size == 0) {
    return -1; // No processes in the pool
  }
  return pool->heap_array[0]->arrival_time;
}

// Print the pool
void printPool(Pool *pool) {
  printf("--- Min-Priority Process Heap ---\n");
  for (int i = 0; i < pool->size; i++) {
    printProcess(pool->heap_array[i]);
  }
  printf("-----------------------------------\n");
}

// Duplicate the pool
Pool *duplicatePool(Pool *pool) {
  Pool *new_pool = createPool(pool->capacity);
  new_pool->size = pool->size;

  for (int i = 0; i < pool->size; i++) {
    new_pool->heap_array[i] = (Process *)malloc(sizeof(Process));
    if (!new_pool->heap_array[i]) {
      fprintf(stderr, "Failed to allocate memory for process\n");
      freePool(new_pool);
      exit(EXIT_FAILURE);
    }
    memcpy(new_pool->heap_array[i], pool->heap_array[i], sizeof(Process));
  }

  return new_pool;
}

// Free the pool
void freePool(Pool *pool) {
  if (pool) {
    if (pool->heap_array) {
      for (int i = 0; i < pool->size; i++) {
        free(pool->heap_array[i]);
      }
      free(pool->heap_array);
    }
    free(pool);
  }
}
