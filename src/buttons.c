#include "buttons.h"

#include "timers.h"

volatile uint32_t debounceDelay = 1000;

volatile Button buttons[18];

void setButtonState(int index, int currentPinState)
{
	// Checks if button was recently pressed or if the state is the same
	// Otherwise, sets new state
	if(currentPinState != buttons[index].state && (usTicks - buttons[index].timestamp) > debounceDelay)
	{
		buttons[index].state = currentPinState;
		buttons[index].timestamp = usTicks;
	}
}
