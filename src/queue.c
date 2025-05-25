#include "queue.h"
#include <stdlib.h>

Queue *create_queue() {
  Queue *queue = (Queue *)malloc(sizeof(Queue));
  if (!queue)
    return NULL;
  queue->front = NULL;
  queue->rear = NULL;
  return queue;
}

void enqueue(Queue *queue, void *data) {
  Node *new_node = (Node *)malloc(sizeof(Node));
  if (!new_node)
    return;
  new_node->data = data;
  new_node->next = NULL;

  if (queue->rear) {
    queue->rear->next = new_node;
  } else {
    queue->front = new_node;
  }
  queue->rear = new_node;
}

void *dequeue(Queue *queue) {
  if (!queue->front)
    return NULL;

  Node *temp = queue->front;
  void *data = temp->data;
  queue->front = temp->next;

  if (!queue->front) {
    queue->rear = NULL;
  }

  free(temp);
  return data;
}

void free_queue(Queue *queue) {
  Node *current = queue->front;
  Node *next_node;

  while (current) {
    next_node = current->next;
    free(current);
    current = next_node;
  }

  free(queue);
}

void *peek(Queue *queue) {
  if (!queue->front)
    return NULL;
  return queue->front->data;
}

int is_empty(Queue *queue) { return queue->front == NULL; }
