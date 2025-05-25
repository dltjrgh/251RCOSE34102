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

void addGanttNode(GanttChart *chart, int pid, int start_time, int end_time) {
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

int evalWaitingTime(const GanttChart *chart, Process *process) {
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

  return waiting_time;
}

int evalTurnaroundTime(const GanttChart *chart, Process *process) {
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

void printGanttChart(const GanttChart *chart) {
  GanttNode *temp = chart->head;
  printf("Gantt Chart:\n");
  while (temp != NULL) {
    printf("PID: %d, Start Time: %d, End Time: %d\n", temp->pid,
           temp->start_time, temp->end_time);
    temp = temp->next;
  }
}
