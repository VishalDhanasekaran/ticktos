// === src/pcb.c ===
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>
#include <sys/time.h>
#include <unistd.h>
#include <signal.h>
#include <sys/mman.h>
#include <setjmp.h>
#include <ucontext.h>
#include "../include/pcb.h"
#include "../include/queue.h"
#include "../include/lock.h"

#define STACK_SIZE 8192 // 8KB for light weighted tasks
#define MAXPROCESS 10
static int activeTaskCount = 0;
static Queue readyQ;
static PCB *currentTask = NULL;
static struct itimerval timer;
static ucontext_t scheduler_context;

void OS_Schedule();

void taskWrapper(void (*taskFunc)())
{
    taskFunc();
    currentTask->state = TERMINATED;
    OS_Schedule();
    exit(0);
}
static Queue blockedQ;

void timerHandler(int signum)
{
    OS_Schedule();
}


void createTask(char *name, uint32_t priority, void (*taskFunc)()) 
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
    newTask->stack = mmap(NULL, STACK_SIZE, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
    newTask->stack_size = STACK_SIZE;
    getcontext(&newTask->context);

    newTask->context.uc_stack.ss_sp = newTask->stack;
    newTask->context.uc_stack.ss_size = STACK_SIZE;
    newTask->context.uc_link = &scheduler_context;
    activeTaskCount++;
   
    makecontext(&newTask->context, (void (*)(void))taskWrapper, 1, taskFunc);

    enqueue(&readyQ, newTask);
    unlock();
}

void startTimer()
{
    signal(SIGALRM, timerHandler);
    timer.it_value.tv_sec = 0;
    timer.it_value.tv_usec = 50000;
    timer.it_interval.tv_sec = 0;
    timer.it_interval.tv_usec = 50000;
    setitimer(ITIMER_REAL, &timer, NULL);

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
void OS_Schedule() 
{
    lock();
    if (isEmpty(&readyQ)) 
    {
        printf("\nNo more tasks to run\n");
        unlock();
        return;
    }
    currentTask = front(&readyQ);
    if (currentTask == NULL) 
    {
        printf("\nNo valid task to schedule.\n");
        unlock();
        return;
    }

    PCB *prevTask = currentTask;
    currentTask = front(&readyQ);
    dequeue(&readyQ);
    if (prevTask && prevTask->state != TERMINATED) 
    {
        prevTask->state = READY;
        enqueue(&readyQ, prevTask);
    }
    currentTask->state = RUNNING;
    unlock();
    setcontext(&currentTask->context);
}

void OS_Run() 
{
    startTimer();
    OS_Schedule();
}
void sampleTask() 
{
    if(currentTask == NULL)
        return;

    for (int i = 0; i < 5; i++) {
        printf("Task %s running iteration %d\n", currentTask->pname, i + 1);
        usleep(100000);
    }
}
/*void OS_Run() 
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
}*/
