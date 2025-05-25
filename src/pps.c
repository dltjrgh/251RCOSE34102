#include "pps.h"

int pps_compare(Process *a, Process *b) {
  if (a->priority < b->priority) {
    return -1; // a comes before b
  } else if (a->priority > b->priority) {
    return 1; // b comes before a
  } else {
    return 0; // they are equal
  }
}

PPSRqueue *pps_create_rqueue(int capacity) {
  PPSRqueue *rqueue = create_heap(capacity, pps_compare);
  if (!rqueue) {
    fprintf(stderr, "Failed to create PPSRqueue\n");
    return NULL;
  }

  rqueue->capacity = capacity;
  rqueue->size = 0;
  rqueue->data = (Process **)malloc(capacity * sizeof(Process *));

  if (!rqueue->data) {
    fprintf(stderr, "Failed to allocate memory for PPSRqueue data\n");
    free(rqueue);
    exit(EXIT_FAILURE);
  }

  return rqueue;
}

PPSState *pps_init(Pool *pool, int capacity) {
  PPSState *state = (PPSState *)malloc(sizeof(PPSState));
  if (!state) {
    fprintf(stderr, "Failed to allocate memory for PPSState\n");
    return NULL;
  }

  state->pool = pool;
  state->ready_queue = create_heap(capacity, pps_compare);
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

void pps_add_ready(PPSState *state, Process *process) {
  if (!state || !process) {
    return;
  }

  insert_heap(state->ready_queue, process);
}

void pps_add_waiting(PPSState *state, Process *process) {
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

void pps_add_terminated(PPSState *state, Process *process) {
  if (!state || !process) {
    return;
  }

  enqueue(state->terminated_queue, process);
}

Process *pps_remove_ready(PPSState *state) {
  if (!state || state->ready_queue->size == 0) {
    return NULL;
  }

  return extract_min(state->ready_queue);
}

Process *pps_remove_waiting(PPSState *state) {
  if (!state || is_empty(state->waiting_queue)) {
    return NULL;
  }

  return dequeue(state->waiting_queue);
}

void pps_consume_time_waiting(PPSState *state) {
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
      pps_remove_waiting(state);
      if (is_empty(process->workloads)) {
        pps_add_terminated(state, process);
      } else {
        pps_add_ready(state, process);
      }
    }
  }
}
void pps_consume_time_running(PPSState *state) {
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
        pps_add_terminated(state, state->running_process);
        state->running_process = NULL;
      } else {
        pps_add_waiting(state, state->running_process);
        state->running_process = NULL;
      }
    }
  }
}

void pps_get_from_pools(PPSState *state) {

  while (get_min_arrival_time(state->pool) <= state->current_time) {

    Process *process = extract_min(state->pool);
    if (process != NULL) {
      pps_add_ready(state, process);
    }

    if (get_min_arrival_time(state->pool) == -1) {
      break;
    }
  }
}

void pps_start_running(PPSState *state) {
  if (!state || state->running_process || is_empty_heap(state->ready_queue)) {
    return; // Already running a process
  }
  state->running_process = pps_remove_ready(state);
  if (state->running_process) {
    state->running_since = state->current_time;
  }
}

void pps_preempt_process(PPSState *state) {
  if (!state || !state->running_process) {
    return;
  }

  Process *running_process = state->running_process;
  Process *min_process = peek_heap(state->ready_queue);
  if (min_process && pps_compare(min_process, running_process) < 0) {
    // Preempt the running process
    add_gantt_node(state->gantt, running_process->pid, state->running_since,
                   state->current_time);
    pps_remove_ready(state);
    pps_add_ready(state, running_process);
    state->running_process = min_process;
    state->running_since = state->current_time;
  }
}

void pps_next_step(PPSState *state) {
  if (!state) {
    return;
  }

  pps_get_from_pools(state);
  pps_preempt_process(state);
  pps_start_running(state);
  pps_consume_time_waiting(state);
  pps_consume_time_running(state);

  state->current_time++;
}

void pps_print_stat(PPSState *state) {
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
}

void execute_pps(PPSState *state) {
  if (!state) {
    return;
  }

  while (!is_empty_heap(state->ready_queue) ||
         !is_empty(state->waiting_queue) || state->running_process != NULL ||
         get_min_arrival_time(state->pool) != -1) {
    pps_next_step(state);
  }

  print_gantt_chart(state->gantt);
  pps_print_stat(state);
}
