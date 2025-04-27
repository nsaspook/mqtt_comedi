/*
 * File:   gemsecs.h
 * Author: root
 *
 * Created on February 26, 2019, 2:58 PM
 */

#ifndef GEMSECS_H
#define	GEMSECS_H

#ifdef	__cplusplus
extern "C" {
#endif

#define ENQ	0x05
#define EOT	0x04
#define ACK	0x06
#define NAK	0x15

#include <stdio.h>
#include <string.h>
#include "vconfig.h"
#include "mcc_generated_files/mcc.h"
#include "mcc_generated_files/uart1.h"
#include "timers.h"
#include "mydisplay.h"
#include "msg_text.h"

#define HOST_UART	1
#define EQUIP_UART	2

	const char msg_gemcmds[] = "Host CMDS: M C R P O L S D E H F";
	const char msg_freecmds[] = "Port baud rate unlocked        ";
	const char msg_gemremote[] = "Host CMDS: ENABLED REMOTE";


	uint16_t block_checksum(uint8_t *, const uint16_t);
	uint16_t run_checksum(const uint8_t, const bool);
	LINK_STATES m_protocol(LINK_STATES *);
	LINK_STATES r_protocol(LINK_STATES *);
	LINK_STATES t_protocol(LINK_STATES *);
	void hb_message(void);
	void terminal_format(DISPLAY_TYPES);
	uint16_t format_display_text(const char *);
	P_CODES s10f1_opcmd(void);
	uint16_t s6f11_opcmd(void);
	bool sequence_messages(const uint8_t);
	void secs_II_monitor_message(const uint8_t, const uint8_t, const uint16_t);
	GEM_STATES secs_gem_state(const uint8_t, const uint8_t);
	void equip_tx(const uint8_t);

#ifdef	__cplusplus
}
#endif

#endif	/* GEMSECS_H */

