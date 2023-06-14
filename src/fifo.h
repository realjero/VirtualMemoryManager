//
// Created by student on 14.06.23.
//

#ifndef VMM_FIFO_H
#define VMM_FIFO_H

#define MAX_SIZE 64

typedef struct {
    int data[MAX_SIZE];
    int front;
    int rear;
} Queue;

// Function to initialize the queue
void initialize(Queue *queue);

void enqueue(Queue *queue, int item);

int dequeue(Queue *queue);

void display(Queue *queue);

#endif //VMM_FIFO_H
