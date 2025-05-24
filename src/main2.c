#include <stdio.h>
#include "pcb.c"
#include "queue.c"
#include "../include/lock.h"
#include <time.h>

void kitty() 
{
    for (int i = 0; i < 5; i++) 
    {
        printf("\nKitty terminal iteration %d", i);
        usleep(100000);
    }
}

void firefox() 
{
    for (int i = 0; i < 5; i++) 
    {
        printf("\nFirefox iteration %d", i);
        usleep(200000);
    }
}

void systemd() 
{
    for (int i = 0; i < 5; i++) 
    {
        printf("\nSystemd iteration %d", i);
        usleep(300000);
    }
}

void emergency() 
{
    printf("\n!!! EMERGENCY TASK PREEMPTING !!!");
    for (int i = 0; i < 3; i++) 
    {
        printf("\nEmergency handling iteration %d", i);
        usleep(100000);
    }
}

int main() 
{
    srand(time(NULL));
    initializeQueue(&readyQ);
    initializeQueue(&blockedQ);
    
    // Create the initial context for the scheduler
    getcontext(&scheduler_context);
    scheduler_context = mmap(NULL, STACK_SIZE, PROT_READ | PROT_WRITE, 
                          MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
    scheduler_context.uc_stack.ss_sp = scheduler_context;
    scheduler_context.uc_stack.ss_size = STACK_SIZE;
    scheduler_context.uc_link = NULL;
    makecontext(&scheduler_context, (void (*)(void))OS_Schedule, 0);
    
    // Creating tasks with different priorities
    createTask("systemd", 3, systemd);  // Highest priority
    createTask("kitty", 2, kitty);      // Medium priority
    createTask("firefox", 1, firefox);  // Lowest priority
    
    // Additional task to demonstrate preemption
    printf("\nStarting OS...\n");
    sleep(1);
    printf("\nAdding a high priority task while others are running...\n");
    
    OS_Run();
    
    // After some time, add a high priority task to demonstrate preemption
    sleep(2);
    createTask("emergency", 5, emergency);  // Very high priority task
    
    while(activeTaskCount > 0) {
        sleep(1);  // Just wait for tasks to complete
    }
    
    printf("\nAll tasks completed\n");
    return 0;
}

