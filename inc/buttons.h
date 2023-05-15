#ifndef BUTTONS_H
#define BUTTONS_H

#include "lpc_types.h"

typedef struct
{
	int state;
	uint64_t timestamp;
} Button;

// holds the state of all the buttons
extern volatile Button buttons[18];

// debounce delay, can be changed through configuration
extern volatile uint32_t debounceDelay; // 1 ms

extern void setButtonState(int index, int currentPinState);

#endif
