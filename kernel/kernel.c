#include "kernel.h"

#define MAXTHREADS 3
#define STACKSIZE  100

#define BUS_FREQ 16000000
unit32_t MILLIS_PRESCALAR

typedef struct 
{
    int32_t *stackPtr;
    struct TCB *nextPtr;
} TCB;

TCB tasks[MAXTHREADS];
TCB *currentPtr;

int32_t stack[MAXTHREADS][STACKSIZE];

void osStackInit(int i)
{
    tasks[i].stackPtr = &stack[i][STACKSIZE - 16];
    TCB_STACK[i][STACKSIZE - 1] = 0x01000000;
}

uint8_t osAddThreads(void (*task0)(void), void (*task1)(void), void (*task2)(void))
{
    tasks[0].nextPtr = &tasks[1];
    tasks[1].nextPtr = &tasks[2];
    tasks[2].nextPtr = &tasks[0];
    
    osInit(0);
    stack[0][STACKSIZE - 2] = (int32_t) (task0);

    osInit(1);
    stack[1][STACKSIZE - 2] = (int32_t) (task1);

    osInit(2);
    stack[2][STACKSIZE - 2] = (int32_t) (task2);
 
    return 1;
}

void osInit(void)
{
   MILLIS_PRESCALAR = BUS_FREQ / 1000;
}

void osLaunch(uint32_t quantum)
{
    /*SysTick->CTRL = 0;
    SysTick->VAL  = 0;
    SYSPRI3       = (SYSPRI3&0x00FFFFFF) | (0xE0000000);
    SysTick->LOAD = (quantum * MILLIS_PRESCALER);
    SysTick->CTRL = 0x00000007; */
    sleep(quantum);

    //osScheduler();
}
