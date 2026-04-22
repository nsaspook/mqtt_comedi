#include "powerm.h"

static volatile uint8_t cc_stream_file, *cc_buffer, cc_buffer_0[MAX_DATA], cc_buffer_tx[MAX_DATA]; // RX and TX command buffers

static volatile M_data M = {
	.blink_lock = false,
	.power_on = true,
};

/*
 * register data frames
 */
POWERM_data1 pz, *powerm_ptr;

void powerm_version(void)
{
	strncpy(em_info, "POWERM Driver      ", 32);
	strncpy(modbus_name [1], "POWERM", 12);
}

int8_t powerm_controller_work(C_data * client)
{
	return client->trace;
}

/*
 * state machine hardware timers interrupt ISR functions setup
 */
void init_powerm_mb_master_timers(void)
{
	cc_buffer = cc_buffer_0;
	powerm_ptr = (POWERM_data1*) & cc_buffer[3];
	TMR3_SetInterruptHandler(timer_2ms_tick);
	TMR3_StartTimer();
}