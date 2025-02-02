// === src/main.c ===
#include <stdio.h>
#include "pcb.c"
#include "queue.c"
#include "../include/lock.h"
#include <time.h>
int main() 
{
    srand(time(NULL));
    initializeQueue(&readyQ);
    createTask("kitty", 10, 2);
    createTask("firefox", 30, 3);
    createTask("systemd", 200, 5);

    OS_Run();
    return 0;
}


