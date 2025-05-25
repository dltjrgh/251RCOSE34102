#ifndef GANTT_H
#define GANTT_H

#include "process.h"
#include <stdio.h>
#include <stdlib.h>

typedef struct gantt_node {
  int pid;
  int start_time;
  int end_time;
  struct gantt_node *next;
} GanttNode;

typedef struct gantt_chart {
  GanttNode *head;
} GanttChart;

GanttChart *create_gantt_chart();

void addGanttNode(GanttChart *chart, int pid, int start_time, int end_time);

int evalWaitingTime(const GanttChart *chart, Process *process);

int evalTurnaroundTime(const GanttChart *chart, Process *process);

void printGanttChart(const GanttChart *chart);

#endif
