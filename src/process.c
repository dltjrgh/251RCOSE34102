#include "process.h"

// Create a new Process
Process *createProcess(int pid, int arrival_time, int priority,
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

  return process;
}

void addWorkload(Queue *workloads, int duration) {
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

void consumeTime(Process *process, int t) {
  if (!process) {
    fprintf(stderr, "Cannot consume time for a NULL process\n");
    return;
  }

  if (t < 0) {
    fprintf(stderr, "Time to consume cannot be negative\n");
    return;
  }

  Workload *current_workload = peek(process->workloads);
  if (current_workload->duration < t) {
    current_workload->duration = 0;
  } else {
    current_workload->duration -= t;
  }
}

void removeWorkload(Process *process) {
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

void printWorkloads(Queue *workloads) {
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

void printProcess(Process *process) {
  if (process) {
    printf("Process ID: %d, Arrival Time: %d, Priority: %d\n", process->pid,
           process->arrival_time, process->priority);
    printf("Workloads:\n");
    printWorkloads(process->workloads);
  } else {
    printf("Process is NULL\n");
  }
}
