#ifndef GANTT_H
#define GANTT_H

#include <stdio.h>
#include <stdlib.h>

typedef struct gantt_node {
  int pid;
  int start_time;
  int end_time;
} GanttNode;

typedef struct gantt_chart {
  GanttNode *head;
} GanttChart;

GanttChart *create_gantt_chart();

void addGanttNode(GanttChart *chart, int pid, int start_time, int end_time);

void printGanttChart(const GanttChart *chart);

#endif
