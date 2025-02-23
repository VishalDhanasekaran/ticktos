// === src/main.c ===
#include <stdio.h>
#include "pcb.c"
#include "queue.c"
#include "../include/lock.h"
#include <time.h>

void kitty() {printf("\nKitty terminal opened");}
void firefox() {printf("\nfirefox opened");}
void systemd() {printf("\nsystemd initiated");}
int main() 
{
    srand(time(NULL));
    initializeQueue(&readyQ);
    createTask("kitty", 10, kitty);
    createTask("firefox", 30, firefox);
    createTask("systemd", 200, systemd);

    OS_Run();
    return 0;
}


