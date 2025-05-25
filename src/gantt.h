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

void add_gantt_node(GanttChart *chart, int pid, int start_time, int end_time);

int eval_waiting_time(const GanttChart *chart, Process *process);

int eval_turnaround_time(const GanttChart *chart, Process *process);

void print_gantt_chart(const GanttChart *chart);

void visual_print_gantt_chart(const GanttChart *chart);

#endif
