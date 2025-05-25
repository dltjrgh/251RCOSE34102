#include "process.h"

// Create a new Process
Process *create_process(int pid, int arrival_time, int priority,
                        Queue *workloads) {
  Process *process = (Process *)malloc(sizeof(Process));
  if (!process) {
    fprintf(stderr, "Failed to allocate memory for process\n");
    exit(EXIT_FAILURE);
  }
  process->pid = pid;
  process->arrival_time = arrival_time;
  process->priority = priority;
  process->workloads = workloads;
  process->io_time = 0;

  return process;
}

int remaining_time(Process *process) {
  if (!process || !process->workloads) {
    fprintf(stderr, "Cannot calculate remaining time for a NULL process or "
                    "empty workloads\n");
    return -1;
  }

  int total_time = 0;
  Node *current = process->workloads->front;
  while (current) {
    Workload *workload = current->data;
    if (workload->is_cpu) {
      total_time += workload->duration;
    }
    current = current->next;
  }
  return total_time;
}

int remaining_io_time(Process *process) {
  if (!process || !process->workloads) {
    fprintf(stderr, "Cannot calculate remaining time for a NULL process or "
                    "empty workloads\n");
    return -1;
  }

  int total_time = 0;
  Node *current = process->workloads->front;
  while (current) {
    Workload *workload = current->data;
    if (!workload->is_cpu) {
      total_time += workload->duration;
    }
    current = current->next;
  }
  return total_time;
}
void add_workload(Queue *workloads, int duration) {
  if (!workloads) {
    fprintf(stderr, "Cannot add workload to a NULL queue\n");
    return;
  }

  Workload *new_workload = (Workload *)malloc(sizeof(Workload));
  if (!new_workload) {
    fprintf(stderr, "Failed to allocate memory for workload\n");
    exit(EXIT_FAILURE);
  }
  new_workload->duration = duration;

  if (is_empty(workloads)) {
    new_workload->is_cpu = 1;
  } else {
    Workload *last_workload = workloads->rear->data;
    new_workload->is_cpu = last_workload->is_cpu ? 0 : 1;
  }

  enqueue(workloads, new_workload);
}

void remove_workload(Process *process) {
  if (!process || !process->workloads) {
    fprintf(stderr,
            "Cannot remove workload from a NULL process or empty workloads\n");
    return;
  }

  Workload *removed_workload = dequeue(process->workloads);
  if (removed_workload) {
    free(removed_workload);
  } else {
    fprintf(stderr, "No workload to remove\n");
  }
}

void print_workloads(Queue *workloads) {
  if (!workloads) {
    fprintf(stderr, "Cannot print workloads of a NULL queue\n");
    return;
  }

  Node *current = workloads->front;
  while (current) {
    Workload *workload = current->data;
    printf("Workload Duration: %d, Is CPU: %d\n", workload->duration,
           workload->is_cpu);
    current = current->next;
  }
}

void print_process(Process *process) {
  if (process) {
    printf("Process ID: %d, Arrival Time: %d, Priority: %d\n", process->pid,
           process->arrival_time, process->priority);
    printf("Workloads:\n");
    print_workloads(process->workloads);
  } else {
    printf("Process is NULL\n");
  }
}

Workload *duplicate_workload(Workload *original) {
  if (original == NULL) {
    return NULL;
  }

  Workload *new_workload = (Workload *)malloc(sizeof(Workload));
  if (new_workload == NULL) {
    fprintf(stderr, "Failed to allocate memory for new workload\n");
    exit(EXIT_FAILURE);
  }

  new_workload->is_cpu = original->is_cpu;
  new_workload->duration = original->duration;

  return new_workload;
}

Queue *duplicate_queue(Queue *original) {
  if (original == NULL) {
    return NULL;
  }

  Queue *new_queue = create_queue();
  if (new_queue == NULL) {
    fprintf(stderr, "Failed to create new queue for duplication\n");
    exit(EXIT_FAILURE);
  }

  Node *current = original->front;
  while (current != NULL) {
    Workload *original_workload = (Workload *)current->data;
    Workload *new_workload = duplicate_workload(original_workload);
    enqueue(new_queue, new_workload);
    current = current->next;
  }

  return new_queue;
}

Process *duplicate_process(Process *original) {
  if (original == NULL) {
    return NULL;
  }

  Process *new_process = (Process *)malloc(sizeof(Process));
  if (new_process == NULL) {
    fprintf(stderr, "Failed to allocate memory for new process\n");
    exit(EXIT_FAILURE);
  }

  new_process->pid = original->pid;
  new_process->arrival_time = original->arrival_time;
  new_process->priority = original->priority;
  new_process->workloads = duplicate_queue(original->workloads);
  new_process->io_time = original->io_time;

  return new_process;
}
