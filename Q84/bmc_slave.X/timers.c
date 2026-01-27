/** \file timers.c
 */

#include <xc.h>
#include <stdint.h>
#include <stdbool.h>
#include "vconfig.h"
#include "timers.h"

extern volatile uint16_t tickCount[TMR_COUNT];

//**********************************************************************************************************************
// Start one of the software timers

inline void StartTimer(const uint8_t timer, const uint16_t count)
{
	tickCount[timer] = count << 1; //Interrupt is every 500us but StartTimer() takes multiple of 1ms so multiply by 2
}

//**********************************************************************************************************************
// Check if one of the software software timers has timed out

inline bool TimerDone(const uint8_t timer)
{
	if (tickCount[timer] == 0) { //Check if counted down to zero
		return true; //then return true
	}
	return false; //else return false
}

//**********************************************************************************************************************
// Simple delay for n milliseconds (blocking)

void WaitMs(const uint16_t numMilliseconds)
{
	StartTimer(TMR_INTERNAL, numMilliseconds); //Start software timer and wait for it to count down
	while (!TimerDone(TMR_INTERNAL)) {
		Nop();
		Nop();
		Nop();
		Nop();
	}
}

/*
 * runs in timer #6 interrupt from FM_io(void)
 */
void timer_ms_tick(const uint32_t status, const uintptr_t context)
{
	//Decrement each software timer
	for (uint8_t i = 0; i < TMR_COUNT; i++) {
		if (tickCount[i] != 0) {
			tickCount[i]--;
		}
	}
	/*
	 * check for button presses
	 */
//	button_press_check();
}