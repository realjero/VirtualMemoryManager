//
// Created by student on 14.06.23.
//

#include <stdio.h>
#include "fifo.h"

// Function to initialize the queue
void initialize(Queue *queue) {
    queue->front = -1;
    queue->rear = -1;
}

// Function to check if the queue is empty
int isEmpty(Queue *queue) {
    return (queue->front == -1 && queue->rear == -1);
}

// Function to check if the queue is full
int isFull(Queue *queue) {
    return (queue->rear + 1) % MAX_SIZE == queue->front;
}

// Function to add an element to the queue
void enqueue(Queue *queue, int item) {
    if (isFull(queue)) {
        printf("Queue is full. Cannot enqueue.\n");
        return;
    } else if (isEmpty(queue)) {
        queue->front = 0;
        queue->rear = 0;
    } else {
        queue->rear = (queue->rear + 1) % MAX_SIZE;
    }

    queue->data[queue->rear] = item;
}

// Function to remove an element from the queue
int dequeue(Queue *queue) {
    if (isEmpty(queue)) {
        printf("Queue is empty. Cannot dequeue.\n");
        return -1;
    } else if (queue->front == queue->rear) {
        int item = queue->data[queue->front];
        queue->front = -1;
        queue->rear = -1;
        return item;
    } else {
        int item = queue->data[queue->front];
        queue->front = (queue->front + 1) % MAX_SIZE;
        return item;
    }
}