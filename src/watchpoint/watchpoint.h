#ifndef WATCHPOINT_H
#define WATCHPOINT_H
#include <stdint.h>

void debug_dwt_init(void);

void debug_dwt_set(void *addr);

#endif