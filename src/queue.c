#include "../include/queue.h"
#include <stdbool.h>
//#include "pcb.h"

void initializeQueue(Queue* q) 
{
    q->head = -1;
    q->tail = -1;
}

// Check if the queue is full
bool isFull(Queue* q) 
{
    return q->tail == MAXQSIZE - 1;
}

// Check if the queue is empty
bool isEmpty(Queue* q) 
{
    return q->tail == -1;
}

// Enqueue a new task
void enqueue(Queue* q, PCB* newTask) 
{
    if (isFull(q)) 
    {
        printf("Queue is full\n");
        return;
    }
    if (q->head == -1) q->head = 0;
    q->queue[++(q->tail)] = newTask;
}

// Dequeue a task
void dequeue(Queue* q) 
{
    if (isEmpty(q)) 
    {
        printf("Queue is empty\n");
        return;
    }
    for (int i = 0; i < q->tail; i++) 
    {
        q->queue[i] = q->queue[i + 1];
    }
    (q->tail)--;
    if (q->tail == -1) q->head = -1;
}

// Get the front task in the queue
PCB* front(Queue* q) 
{
    if (isEmpty(q)) 
    {
        printf("Queue is empty\n");
        return NULL;
    }
    return q->queue[q->head];
}




