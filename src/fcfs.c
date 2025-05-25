#include "fcfs.h"

FCFSState *fcfs_init(Pool *pool) {
  FCFSState *state = (FCFSState *)malloc(sizeof(FCFSState));
  if (state == NULL) {
    return NULL; // Memory allocation failed
  }

  state->pool = pool;
  state->current_time = 0;
  state->running_process = NULL;
  state->running_since = 0;
  state->ready_queue = create_queue();
  state->waiting_queue = create_queue();
  state->terminated_queue = create_queue();
  state->gantt = create_gantt_chart();

  return state;
}

void fcfs_add_ready(FCFSState *state, Process *process) {
  if (state == NULL || process == NULL) {
    return; // Invalid state or process
  }

  enqueue(state->ready_queue, process);
}

void fcfs_add_waiting(FCFSState *state, Process *process) {
  if (state == NULL || process == NULL) {
    return; // Invalid state or process
  }

  enqueue(state->waiting_queue, process);
  if (process->workloads->front == process->workloads->rear) {
    return; // Final I/O does not count as waiting time
  } else {
    Workload *workload = process->workloads->front->data;
    process->io_time += workload->duration;
  }
}

void fcfs_add_terminated(FCFSState *state, Process *process) {
  if (state == NULL || process == NULL) {
    return; // Invalid state or process
  }

  enqueue(state->terminated_queue, process);
}

Process *fcfs_remove_ready(FCFSState *state) {
  if (state == NULL || is_empty(state->ready_queue)) {
    return NULL; // Invalid state or empty queue
  }

  return dequeue(state->ready_queue);
}

Process *fcfs_remove_waiting(FCFSState *state) {
  if (state == NULL || is_empty(state->waiting_queue)) {
    return NULL; // Invalid state or empty queue
  }

  return dequeue(state->waiting_queue);
}

void fcfs_consume_time_waiting(FCFSState *state) {
  if (state == NULL || is_empty(state->waiting_queue)) {
    return; // Invalid state or empty queue
  }

  // Consume time for the first process in the waiting queue
  Process *process = state->waiting_queue->front->data;
  if (process != NULL) {
    Workload *workload = process->workloads->front->data;
    if (workload->is_cpu == 0 && workload->duration > 0) {
      workload->duration--;
    }
    if (workload->is_cpu == 0 && workload->duration == 0) {
      // If the workload is finished, remove it
      remove_workload(process);
      fcfs_remove_waiting(state);
      if (is_empty(process->workloads)) {
        // If no workloads left, move to terminated queue
        fcfs_add_terminated(state, process);
      } else {
        // If still has workloads, move to ready queue
        fcfs_add_ready(state, process);
      }
    }
  }
}

void fcfs_consume_time_running(FCFSState *state) {
  if (state == NULL || state->running_process == NULL) {
    return; // Invalid state or no running process
  }

  Workload *workload = state->running_process->workloads->front->data;
  if (workload != NULL && workload->is_cpu == 1 && workload->duration > 0) {
    workload->duration--;
    if (workload->duration == 0) {
      // If the CPU burst is finished, log the gantt chart
      add_gantt_node(state->gantt, state->running_process->pid,
                     state->running_since, state->current_time + 1);
      // If the CPU burst is finished, remove it
      remove_workload(state->running_process);
      // If no workloads left, move to terminated queue
      if (is_empty(state->running_process->workloads)) {
        fcfs_add_terminated(state, state->running_process);
        state->running_process = NULL;
      } else {
        // If still has workloads, move to waiting queue
        fcfs_add_waiting(state, state->running_process);
        state->running_process = NULL;
      }
    }
  }
}

void fcfs_get_from_pools(FCFSState *state) {
  // Bring the processes from the pool that has arrived before or at the
  // current time
  while (get_min_arrival_time(state->pool) <= state->current_time) {

    Process *process = extract_min(state->pool);
    if (process != NULL) {
      fcfs_add_ready(state, process);
    }

    if (get_min_arrival_time(state->pool) == -1) {
      break; // No more processes to bring from the pool
    }
  }
}

void fcfs_start_running(FCFSState *state) {
  if (state == NULL || state->running_process != NULL ||
      is_empty(state->ready_queue)) {
    return; // Invalid state, no ready processes, or already running
  }

  // Start running the first process in the ready queue
  state->running_process = fcfs_remove_ready(state);
  if (state->running_process != NULL) {
    state->running_since = state->current_time;
  }
}

// Proceed current_time by 1
void fcfs_next_step(FCFSState *state) {
  if (state == NULL) {
    return; // Invalid state
  }

  fcfs_get_from_pools(state);
  fcfs_start_running(state);
  fcfs_consume_time_waiting(state);
  fcfs_consume_time_running(state);

  state->current_time++;
}

void fcfs_print_stat(FCFSState *state) {
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

void execute_fcfs(FCFSState *state) {
  if (state == NULL) {
    return; // Invalid state
  }

  while (!is_empty(state->ready_queue) || !is_empty(state->waiting_queue) ||
         state->running_process != NULL ||
         get_min_arrival_time(state->pool) != -1) {
    fcfs_next_step(state);
  }

  print_gantt_chart(state->gantt);
  visual_print_gantt_chart(state->gantt);
  fcfs_print_stat(state);
}
