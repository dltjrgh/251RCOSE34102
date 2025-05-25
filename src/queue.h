#ifndef QUEUE_H
#define QUEUE_H

typedef struct Node {
  void *data;
  struct Node *next;
} Node;

typedef struct Queue {
  Node *front;
  Node *rear;
} Queue;

Queue *create_queue();

void enqueue(Queue *queue, void *data);

void *dequeue(Queue *queue);

void free_queue(Queue *queue);

void *peek(Queue *queue);

int is_empty(Queue *queue);

#endif // QUEUE_H
