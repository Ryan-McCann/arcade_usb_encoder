#ifndef TIMERS_H
#define TIMERS_H

#include "lpc_types.h"

extern volatile uint64_t usTicks;

// Use systick interrupt to track time since start in microseconds
extern void SysTick_Handler(void);

#endif
