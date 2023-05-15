#include "timers.h"

volatile uint64_t usTicks = 0;

// Use systick interrupt to track time since start in microseconds
void SysTick_Handler(void)
{
	usTicks++;
}
