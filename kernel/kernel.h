#ifndef _OS_KERNEL_H
#define _OS_KERNEL_H

void osLaunch(uint32_t quantum);

void osInit(void);

uint8_t osAddThreads(void (*task0)(void), 
                     void (*task1)(void)
                     void (*task2)(void));

#endif
