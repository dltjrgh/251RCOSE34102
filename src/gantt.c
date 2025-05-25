#include "gantt.h"

GanttChart *create_gantt_chart() {
  GanttChart *chart = (GanttChart *)malloc(sizeof(GanttChart));
  if (chart == NULL) {
    fprintf(stderr, "Memory allocation failed\n");
    return NULL;
  }
  chart->head = NULL;

  return chart;
}

void add_gantt_node(GanttChart *chart, int pid, int start_time, int end_time) {
  GanttNode *new_node = (GanttNode *)malloc(sizeof(GanttNode));
  if (new_node == NULL) {
    fprintf(stderr, "Memory allocation failed\n");
    return;
  }
  new_node->pid = pid;
  new_node->start_time = start_time;
  new_node->end_time = end_time;
  new_node->next = NULL;

  if (chart->head == NULL) {
    chart->head = new_node;
  } else {
    GanttNode *temp = chart->head;
    while (temp->next != NULL) {
      temp = temp->next;
    }
    temp->next = new_node;
  }
}

int eval_waiting_time(const GanttChart *chart, Process *process) {
  int pid = process->pid;
  int arrival_t = process->arrival_time;
  GanttNode *temp = chart->head;
  int waiting_time = 0;
  int last_end_time = arrival_t;

  while (temp != NULL) {
    if (temp->pid == pid) {
      waiting_time += temp->start_time - last_end_time;
      last_end_time = temp->end_time;
    }
    temp = temp->next;
  }

  return waiting_time - process->io_time;
}

int eval_turnaround_time(const GanttChart *chart, Process *process) {
  int pid = process->pid;
  GanttNode *temp = chart->head;
  int turnaround_time = 0;

  while (temp != NULL) {
    if (temp->pid == pid) {
      turnaround_time = temp->end_time - process->arrival_time;
    }
    temp = temp->next;
  }

  return turnaround_time;
}

float eval_cpu_utilization(const GanttChart *chart) {
  GanttNode *temp = chart->head;
  float total_time = 0;
  float busy_time = 0;

  while (temp != NULL) {
    total_time = temp->end_time;
    busy_time += (temp->end_time - temp->start_time);
    temp = temp->next;
  }

  if (total_time == 0) {
    return 0; // Avoid division by zero
  }

  return (busy_time * 100) / total_time;
}

float eval_throughput(const GanttChart *chart) {
  GanttNode *temp = chart->head;
  float total_processes = 0;
  float total_time = 0;

  while (temp != NULL) {
    total_processes = (temp->pid + 1) > total_processes
                          ? temp->pid + 1
                          : total_processes; // Assuming PID starts from 0
    total_time = temp->end_time;             // Last end time gives total time
    temp = temp->next;
  }

  if (total_time == 0) {
    return 0; // Avoid division by zero
  }

  return total_processes / total_time; // Throughput as processes per unit time
}

float eval_context_switch_rate(const GanttChart *chart) {
  GanttNode *temp = chart->head;
  float context_switches = 0;
  float total_time = 0;

  if (temp == NULL) {
    return 0; // No processes, no context switches
  }

  while (temp != NULL) {
    total_time = temp->end_time;
    context_switches++;
    temp = temp->next;
  }

  if (total_time == 0) {
    return 0; // Avoid division by zero
  }

  return context_switches / total_time;
}

void print_gantt_chart(const GanttChart *chart) {
  GanttNode *temp = chart->head;
  printf("Gantt Chart:\n");
  while (temp != NULL) {
    printf("PID: %d, Start Time: %d, End Time: %d\n", temp->pid,
           temp->start_time, temp->end_time);
    temp = temp->next;
  }
}

void visual_print_gantt_chart(const GanttChart *chart) {
  GanttNode *temp = chart->head;
  int current_time = 0;
  printf("Gantt Chart Visualization:\n");
  while (temp != NULL) {
    if (temp->start_time > current_time) {
      printf("----------%d----------\n", current_time);
      printf("         idle         \n");
    }
    current_time = temp->start_time;
    printf("----------%d----------\n", current_time);

    printf("      [PID %d]      \n", temp->pid);
    current_time = temp->end_time;

    temp = temp->next;
  }
  printf("----------%d----------\n", current_time);
}
