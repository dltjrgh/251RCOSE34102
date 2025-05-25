#ifndef HEAP_H
#define HEAP_H

#include "process.h"
#include <stdio.h>
#include <stdlib.h>

typedef int (*HeapCompareFunc)(Process *a, Process *b);

typedef struct {
  Process **data;
  int size;
  int capacity;
  HeapCompareFunc compare;
} Heap;

// Initialize a heap with a given capacity and compare function
Heap *create_heap(int capacity, HeapCompareFunc compareFunc);

Process *extract_min(Heap *heap);

void min_heapify(Heap *heap, int i);

void insert_heap(Heap *heap, Process *process);

int get_parent(int i);
int get_left_child(int i);
int get_right_child(int i);
void swap_processes(Process **a, Process **b);

Process *peek_heap(Heap *heap);

int is_empty_heap(Heap *heap);

void print_heap(Heap *heap);

void free_heap(Heap *heap);

#endif // HEAP_H
