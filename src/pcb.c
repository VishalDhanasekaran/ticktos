// === src/pcb.c ===
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <unistd.h>
#include "../include/pcb.h"
#include "../include/queue.h"
#include "../include/lock.h"
#define MAXPROCESS 10
static int activeTaskCount = 0;
static Queue readyQ;
static Queue blockedQ;

void createTask(char *name, uint32_t priority, uint32_t BT) 
{
    lock();
    if (activeTaskCount >= MAXPROCESS) 
    {
        printf("Process limit exceeded\n");
        unlock();
        return;
    }
    PCB *newTask = (PCB *)malloc(sizeof(PCB));
    newTask->pname = name;
    newTask->priority = priority;
    newTask->pid = rand();
    newTask->state = READY;
    newTask->stack = (uint32_t *)malloc(1024 * sizeof(uint32_t)); // Simulate stack allocation
    newTask->stack_size = 1024;
    newTask->burst_time = BT;
    activeTaskCount++;
    enqueue(&readyQ, newTask);
    unlock();
}

void haltTask(PCB *task) 
{
    lock();
    printf("Process %d halting...\n", task->pid);
    free(task->stack);
    free(task);
    activeTaskCount--;
    unlock();
}

void OS_Run() 
{
    while (!isEmpty(&readyQ)) 
    {
        //lock();
        PCB *currentTask = front(&readyQ);
        dequeue(&readyQ);
        currentTask->state = RUNNING;

        printf("\nRunning Process:\n");
        printf("Name: %s, PID: %u, Priority: %u\n", currentTask->pname, currentTask->pid, currentTask->priority);
        //unlock();

        
        sleep(currentTask->burst_time); // Simulate task execution
        
        //lock();
        printf("\n Terminating process...");
        currentTask->state = TERMINATED;
        haltTask(currentTask);
        printf("\nTermination success!!");
        //unlock();
    }
}



