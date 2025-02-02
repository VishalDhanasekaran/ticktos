
// === include/lock.h ===
#ifndef LOCK_H
#define LOCK_H
#include <stdbool.h>
static bool lock_flag = false;

void lock() 
{
    while (__sync_lock_test_and_set(&lock_flag, true)) {}
}
void unlock() 
{
    __sync_lock_release(&lock_flag);
}
#endif // LOCK_H


