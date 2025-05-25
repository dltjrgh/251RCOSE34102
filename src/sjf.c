#include "sjf.h"

int sjf_compare(Process *a, Process *b) {
  if (remaining_time(a) < remaining_time(b)) {
    return -1; // a comes before b
  } else if (remaining_time(a) > remaining_time(b)) {
    return 1; // b comes before a
  } else {
    return 0; // they are equal
  }
}

SJFRqueue *sjf_create_rqueue(int capacity) {
  SJFRqueue *rqueue = create_heap(capacity, sjf_compare);
  if (!rqueue) {
    fprintf(stderr, "Failed to create SJF_rqueue\n");
    return NULL;
  }

  rqueue->capacity = capacity;
  rqueue->size = 0;
  rqueue->data = (Process **)malloc(capacity * sizeof(Process *));

  if (!rqueue->data) {
    fprintf(stderr, "Failed to allocate memory for SJF_rqueue data\n");
    free(rqueue);
    exit(EXIT_FAILURE);
  }

  return rqueue;
}

SJFState *sjf_init(Pool *pool, int capacity) {
  SJFState *state = (SJFState *)malloc(sizeof(SJFState));
  if (!state) {
    fprintf(stderr, "Failed to allocate memory for SJFState\n");
    return NULL;
  }

  state->pool = pool;
  state->ready_queue = create_heap(capacity, sjf_compare);
  if (!state->ready_queue) {
    free(state);
    return NULL;
  }

  state->current_time = 0;
  state->running_process = NULL;
  state->running_since = 0;
  state->waiting_queue = create_queue();
  state->terminated_queue = create_queue();
  state->gantt = create_gantt_chart();

  return state;
}

void sjf_add_ready(SJFState *state, Process *process) {
  if (!state || !process) {
    return;
  }

  insert_heap(state->ready_queue, process);
}

void sjf_add_waiting(SJFState *state, Process *process) {
  if (!state || !process) {
    return;
  }

  enqueue(state->waiting_queue, process);
  if (process->workloads->front == process->workloads->rear) {
    return; // Final I/O does not count as waiting time
  } else {
    Workload *workload = process->workloads->front->data;
    process->io_time += workload->duration;
  }
}

void sjf_add_terminated(SJFState *state, Process *process) {
  if (!state || !process) {
    return;
  }

  enqueue(state->terminated_queue, process);
}

Process *sjf_remove_ready(SJFState *state) {
  if (!state || state->ready_queue->size == 0) {
    return NULL;
  }

  return extract_min(state->ready_queue);
}

Process *sjf_remove_waiting(SJFState *state) {
  if (!state || is_empty(state->waiting_queue)) {
    return NULL;
  }

  return dequeue(state->waiting_queue);
}

void sjf_consume_time_waiting(SJFState *state) {
  if (!state || is_empty(state->waiting_queue)) {
    return;
  }

  Process *process = state->waiting_queue->front->data;
  if (process) {
    Workload *workload = process->workloads->front->data;
    if (workload->is_cpu == 0 && workload->duration > 0) {
      workload->duration--;
    }
    if (workload->is_cpu == 0 && workload->duration == 0) {
      remove_workload(process);
      sjf_remove_waiting(state);
      if (is_empty(process->workloads)) {
        sjf_add_terminated(state, process);
      } else {
        sjf_add_ready(state, process);
      }
    }
  }
}
void sjf_consume_time_running(SJFState *state) {
  if (!state || !state->running_process) {
    return;
  }

  Workload *workload = state->running_process->workloads->front->data;
  if (workload != NULL && workload->duration > 0) {
    assert(workload->is_cpu == 1);
    workload->duration--;
    if (workload->duration == 0) {
      add_gantt_node(state->gantt, state->running_process->pid,
                     state->running_since, state->current_time + 1);
      remove_workload(state->running_process);
      if (is_empty(state->running_process->workloads)) {
        sjf_add_terminated(state, state->running_process);
        state->running_process = NULL;
      } else {
        sjf_add_waiting(state, state->running_process);
        state->running_process = NULL;
      }
    }
  }
}

void sjf_get_from_pools(SJFState *state) {

  while (get_min_arrival_time(state->pool) <= state->current_time) {

    Process *process = extract_min(state->pool);
    if (process != NULL) {
      sjf_add_ready(state, process);
    }

    if (get_min_arrival_time(state->pool) == -1) {
      break;
    }
  }
}

void sjf_start_running(SJFState *state) {
  if (!state || state->running_process || is_empty_heap(state->ready_queue)) {
    return; // Already running a process
  }
  state->running_process = sjf_remove_ready(state);
  if (state->running_process) {
    state->running_since = state->current_time;
  }
}

void sjf_next_step(SJFState *state) {
  if (!state) {
    return;
  }

  sjf_get_from_pools(state);
  sjf_start_running(state);
  sjf_consume_time_waiting(state);
  sjf_consume_time_running(state);

  state->current_time++;
}

void sjf_print_stat(SJFState *state) {
  if (state == NULL) {
    return; // Invalid state
  }

  // traverse the terminated queue and print the statistics of each process
  Node *current = state->terminated_queue->front;
  float total_waiting_time = 0;
  float total_turnaround_time = 0;
  float num_processes = 0;

  printf("----------------------------------------------\n");
  while (current != NULL) {
    Process *process = current->data;
    printf("Process ID: %d\n", process->pid);
    printf("Total Waiting Time: %d\n",
           eval_waiting_time(state->gantt, process));
    printf("Total Turnaround Time: %d\n",
           eval_turnaround_time(state->gantt, process));
    total_waiting_time += eval_waiting_time(state->gantt, process);
    total_turnaround_time += eval_turnaround_time(state->gantt, process);
    num_processes++;
    current = current->next;
  }
  printf("----------------------------------------------\n");

  printf("Average Waiting Time: %.2f\n", total_waiting_time / num_processes);
  printf("Average Turnaround Time: %.2f\n",
         total_turnaround_time / num_processes);
  printf("CPU Utilization: %.2f%%\n", eval_cpu_utilization(state->gantt));
  printf("Throughput: %.2f processes/unit time\n",
         eval_throughput(state->gantt));
  printf("Context Switch Rate: %.2f switches/unit time\n",
         eval_context_switch_rate(state->gantt));
}

void execute_sjf(SJFState *state) {
  if (!state) {
    return;
  }

  while (!is_empty_heap(state->ready_queue) ||
         !is_empty(state->waiting_queue) || state->running_process != NULL ||
         get_min_arrival_time(state->pool) != -1) {
    sjf_next_step(state);
  }

  print_gantt_chart(state->gantt);
  sjf_print_stat(state);
}
