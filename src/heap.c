#include "heap.h"

Heap *create_heap(int capacity, HeapCompareFunc compare_func) {
  Heap *heap = (Heap *)malloc(sizeof(Heap));
  if (!heap) {
    return NULL; // Memory allocation failed
  }

  heap->capacity = capacity;
  heap->size = 0;
  heap->data = (Process **)malloc(capacity * sizeof(Process *));
  heap->compare = compare_func;

  if (!heap->data) {
    free(heap);
    return NULL; // Memory allocation failed
  }

  return heap;
}

Process *extract_min(Heap *heap) {
  if (heap->size == 0) {
    return NULL;
  }
  if (heap->size == 1) {
    heap->size--;
    return heap->data[0];
  }

  Process *root = heap->data[0];
  heap->data[0] = heap->data[heap->size - 1];
  heap->size--;
  min_heapify(heap, 0);

  return root;
}

void min_heapify(Heap *heap, int i) {
  int smallest = i;
  int left = get_left_child(i);
  int right = get_right_child(i);
  if (left < heap->size &&
      heap->compare(heap->data[left], heap->data[smallest]) < 0) {
    smallest = left;
  }
  if (right < heap->size &&
      heap->compare(heap->data[right], heap->data[smallest]) < 0) {
    smallest = right;
  }
  if (smallest != i) {
    swap_processes(&heap->data[i], &heap->data[smallest]);
    min_heapify(heap, smallest);
  }
}

// Add a process to the pool
void insert_heap(Heap *heap, Process *process) {
  if (heap->size == heap->capacity) {
    fprintf(stderr, "Heap is full, cannot insert process\n");
    return;
  }

  heap->size++;
  int i = heap->size - 1;
  heap->data[i] = process;

  while (i != 0 &&
         heap->compare(heap->data[get_parent(i)], heap->data[i]) > 0) {
    swap_processes(&heap->data[i], &heap->data[get_parent(i)]);
    i = get_parent(i);
  }
}

int get_parent(int i) { return (i - 1) / 2; }

int get_left_child(int i) { return 2 * i + 1; }

int get_right_child(int i) { return 2 * i + 2; }

void swap_processes(Process **a, Process **b) {
  Process *temp = *a;
  *a = *b;
  *b = temp;
}

int is_empty_heap(Heap *heap) { return heap->size == 0; }

Process *peek_heap(Heap *heap) {
  if (heap->size == 0) {
    return NULL; // Heap is empty
  }
  return heap->data[0]; // Return the root element (minimum)
}

// Print the pool
void print_heap(Heap *heap) {
  printf("--- Min-Priority Process Heap ---\n");
  for (int i = 0; i < heap->size; i++) {
    print_process(heap->data[i]);
  }
  printf("-----------------------------------\n");
}

// Free the pool
void free_heap(Heap *heap) {
  if (heap) {
    if (heap->data) {
      for (int i = 0; i < heap->size; i++) {
        free(heap->data[i]);
      }
      free(heap->data);
    }
    free(heap);
  }
}
