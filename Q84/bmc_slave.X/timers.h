/** \file timers.h
 */
#ifndef TIMERS_H
#define TIMERS_H

#include <stdint.h>
#include <stdbool.h>

//Software timers - use these to refer to timers rather than
//integers.  Add more as needed.
//NOTE: 32767 ms max delay

enum APP_TIMERS {
	TMR_INTERNAL = 0, //Used in timers.c - do not remove or use elsewhere
	TMR_ADC,
	TMR_INFO,
	TMR_HELP,
	TMR_HELPDIS,
	TMR_DISPLAY,
	TMR_SEQ,
	TMR_FLIPPER,
	TMR_MBMASTER,
	TMR_MBTEST,
	TMR_SPIN,
	TMR_DERE,
	TMR_RESTART,
	//
	//(Add timers here as needed)
	//
	TMR_COUNT //number of timers - always keep at end of enum!
};

inline void StartTimer(uint8_t timer, uint16_t count);
inline bool TimerDone(uint8_t timer);
void WaitMs(uint16_t numMilliseconds);
void timer_ms_tick(const uint32_t, const uintptr_t);

#endif //TIMERS_H
