// === src/queue.c ===
#include <stdbool.h>
#include "../include/queue.h"

void initializeQueue(Queue *q) 
{
    q->size = 0;
}

bool isFull(Queue *q) 
{
    return q->size == MAXQSIZE;
}

bool isEmpty(Queue *q) 
{
    return q->size == 0;
}

void enqueue(Queue *q, PCB *newTask) 
{
    if (isFull(q)) 
    {
        printf("Queue is full\n");
        return;
    }
    int i = q->size - 1;
    while (i >= 0 && q->queue[i]->priority < newTask->priority) 
    {
        q->queue[i + 1] = q->queue[i];
        i--;
    }
    q->queue[i + 1] = newTask;
    q->size++;
}

void dequeue(Queue *q) 
{
    if (isEmpty(q)) 
    {
        printf("Queue is empty\n");
        return;
    }
    for (int i = 0; i < q->size - 1; i++) 
    {
        q->queue[i] = q->queue[i + 1];
    }
    q->size--;
}

PCB *front(Queue *q) 
{
    if (isEmpty(q)) 
    {
        printf("Queue is empty\n");
        return NULL;
    }
    return q->queue[0];
}
