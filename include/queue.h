
/*-----------------------------------------------------------------------------
 |                             FOSS-CIT                                       |
 *-----------------------------------------------------------------------------
 */

#ifndef QUEUE_H
#define QUEUE_H
#include <stdbool.h>
#include "pcb.h"

#define MAXQSIZE 10

typedef struct Queue 
{
    int head;
    int tail;
    PCB *queue[MAXQSIZE];
    int size;
} Queue;

void initializeQueue(Queue *q);
bool isFull(Queue *q);
bool isEmpty(Queue *q);
void enqueue(Queue *q, PCB *newTask);
void dequeue(Queue *q);
PCB *front(Queue *q);

#endif // QUEUE_H


