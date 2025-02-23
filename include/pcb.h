
// === include/pcb.h ===
#ifndef PCB_H
#define PCB_H
#include <stdint.h>
#include<ucontext.h>

typedef enum 
{
    READY,
    RUNNING,
    BLOCKED,
    TERMINATED
} TaskState;

typedef struct PCB 
{
    uint32_t pid;
    char *pname;
    uint32_t priority;
    TaskState state;
    uint32_t burst_time;
    uint32_t *stack;
    uint32_t stack_size;
    ucontext_t context;
} PCB;

#endif // PCB_H

