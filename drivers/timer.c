#include "timer.h"
#include "registers.h"
#include "serial.h"

int timer_ticks = 0;

/* NOTE to contributors 
 * this is not a actuall systemd/timers, it should it be enchaned based on ISR routine we follow*/

void timer_driver(register_t *regs)
{
    timer_ticks++;

    if(timer_ticks % 100 == 0)
    {
        serial_writestring("Timer tick: ");
        serial_writeint(timer_ticks);
        serial_writestring("\n");
    }
}
