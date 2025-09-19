/*
 *     comedi/drivers/daq_bmc.c
 *
 *
 *     COMEDI - Linux Control and Measurement Device Interface
 *     Copyright (C) 1998 David A. Schleef <ds@schleef.org>
 *
 *     This program is free software; you can redistribute it and/or modify
 *     it under the terms of the GNU General Public License as published by
 *     the Free Software Foundation; either version 2 of the License, or
 *     (at your option) any later version.
 *
 *     This program is distributed in the hope that it will be useful,
 *     but WITHOUT ANY WARRANTY; without even the implied warranty of
 *     MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *     GNU General Public License for more details.
 *
 *     You should have received a copy of the GNU General Public License
 *     along with this program; if not, write to the Free Software
 *     Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 *
 */

/*
 * TODO: get some SoC board rev and other info from the OPi zero 3
 *
Driver: "experimental" daq_bmc in progress ...
 * for 6.1.31+ kernels with device-tree enabled for Orange PI Zero 3
 * see README.md for install instructions
 *
Description: BMCBOARD daq_bmc spibmc
Author: nsaspook <nsaspooksma2@gmail.com>
 *

Devices: [] BMCBOARD (daq_bmc)
Status: inprogress
Updated: Sep 18 12:07:20 +0000

The DAQ-BMC appears in Comedi as a digital I/O subdevices with
16 DO and 24 DI,

a analog input subdevice with 16 possible single-ended channels set by the SPI slave device
and a analog output subdevice with 1 channel with onboard dac
 *
 a serial r/w memory subdevice with two possible ports, one RS232, the other TTL
 * the TTL port is currently not used remotely on the Q84 by Comedi
 *
 *
 * Caveats:
 *
 *
 * 0 = force PIC slave PIC18F57Q84 mode
 * 1 = force PIC slave PIC18F57Q84 mode with NO DI or DO
 * This is normally autodetected
 *
 * Module parameters are found in the /sys/modules/daq_bmc/parameters directory
 *
 * The input  range is 0 to 4095 for 0.0 to 4.096(Vdd) onboard devices
 * In the async command mode transfers can be handled in HUNK mode by creating a SPI message
 * of many conversion sequences into one message, this allows for close to native driver wire-speed
 * comedi analog output testing command: ./ao_waveform -v -c 0 -f /dev/comedi0_subd2 -n1
 * The transfer array is currently static but can easily be made into
 * a config size parameter runtime value if needed with kmalloc for the required space

 *  PIC Slave Info:
 *
 * bits 3..0 select of ADC channel in the lower nibble
 *
 */

#include <linux/module.h>
#include <linux/comedi/comedidev.h>
#include <linux/interrupt.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/kthread.h>
#include <linux/sched.h>
#include <linux/spi/spi.h>
#include <linux/delay.h>
#include <linux/device.h>
#include <linux/timer.h>
#include <linux/list.h>
#include <linux/completion.h>

#define bmc_version "version 0.98 "
#define spibmc_version "version 1.4 "

static const uint32_t CHECKMARK = 0x1957;
static const uint32_t CHECKBYTE = 0x57;

/*
 * SPI transfer buffer size
 * must be a define to init buffer sizes
 * normally 1024
 */
#define HUNK_LEN 1024
#define SPECIAL_LEN 64
#define Q84_BYTES 9

/*
 * branch macros for ARM7
 */
#define likely(x)      __builtin_expect(!!(x), 1)
#define unlikely(x)    __builtin_expect(!!(x), 0)

/*
 * core node state bits
 */
enum daqbmc_state_bits {
	AI_CMD_RUNNING = 0,
	AO_CMD_RUNNING,
	SPI_AI_RUN,
	SPI_AO_RUN,
	CMD_TIMER,
	CMD_RUN,
};

/*
 * this is the Comedi SPI device queue
 */
static LIST_HEAD(device_list);

/* Driver board type default */
static const uint32_t BMCBOARD = 0;

/* analog chip types  index to daqbmc_device array */
static const uint32_t PICSL12 = 0; // index into chip config array
static const uint32_t PICSL12_AO = 1;

static const uint32_t SUBDEV = 5; // with all sub-devices
static const uint32_t SUBDEV_OAO = 2; // only analog sub-devices

static const uint32_t SUBDEV_AI = 0;
static const uint32_t SUBDEV_AO = 1;
static const uint32_t SUBDEV_DI = 2;
static const uint32_t SUBDEV_DO = 3;
static const uint32_t SUBDEV_MEM = 4;
static const uint32_t SMP_CORES = 4;
static const uint32_t CONF_Q84 = 3;
static const uint32_t MEM_BLOCKS = 8; // 0..3 CLCD display lines, 4..7 serial comms for FM80, MODBUS, etc ...
static const uint32_t SPI_GAP = 5000; // time for the Q84 to process each received SPI byte
static const uint32_t SPI_GAP_LONG = 10000; // time for the Q84 to process each received SPI byte

static const uint32_t I8254_MAX_COUNT = 0x10000;

static const uint32_t PIC18_CONVD_57Q84 = 24;
static const uint32_t PIC18_CMDD_57Q84 = 4;
static const uint32_t SPI_BUFF_SIZE = 128000; // normally 5000
static const uint32_t SPI_BUFF_SIZE_NOHUNK = 4096; // normally 64
static const uint32_t MAX_CHANLIST_LEN = 256;
static const uint32_t CONV_SPEED = 50000; /* 10s of nsecs: the true rate is ~3000/5000 so we need a fixup,  two conversions per result */
static const uint32_t CONV_SPEED_FIX = 19; /* usecs: round it up to ~50usecs total with this */
static const uint32_t CONV_SPEED_FIX_Q84 = 39; /* usecs: round it up to ~50usecs total with this */
static const uint32_t CONV_SPEED_FIX_FREERUN = 1; /* usecs: round it up to ~30usecs total with this */
static const uint32_t CONV_SPEED_FIX_FAST = 9; /* used for the MCP3002 ADC */
static const uint32_t CONV_ADS8330 = 0; /* used for the ADS8330 ADC */
static const uint32_t MAX_BOARD_RATE = 1000000000;
static const struct spi_delay CS_CHANGE_DELAY_USECS0 = {
	.value = 1,
	.unit = SPI_DELAY_UNIT_USECS,
};
static const struct spi_delay CS_CHANGE_DELAY_USECS10 = {
	.value = 10,
	.unit = SPI_DELAY_UNIT_USECS,
};

/*
 * This selects the chip select, not the interface number like spi0, spi1
 */
static const uint32_t CSnA = 1; /*  BMCboard Q84 slave CS, spi1 on the OPi */

/*
 * PIC Slave commands
 */
static const uint8_t CMD_ZERO = 0x00;
static const uint8_t CMD_ADC_GO = 0x80; /* add lower nibble channel number */
static const uint8_t CMD_DAC_GO = 0x90;
static const uint8_t CMD_PORT_GO = 0xa0; /* send data to port output DO buffer */
static const uint8_t CMD_CHAR_GO = 0xb0;
static const uint8_t CMD_ADC_DATA = 0xc0;
static const uint8_t CMD_PORT_DATA = 0xd0;
static const uint8_t CMD_CHAR_DATA = 0xe0;
static const uint8_t CMD_PORT_GET = 0xf0; /* read data from input DI buffer */
static const uint8_t CMD_CHAR_GET = 0x10; /* Get RX buffer */
static const uint8_t CMD_DUMMY_CFG = 0x40; /* stuff config data in SPI buffer */
static const uint8_t CMD_DEAD = 0xff; /* This is usually a bad response */

/*
 * SPI packet link BYTE id's
 */
enum daqbmc_packet_index {
	BMC_CMD = 0,
	BMC_D0,
	BMC_D1,
	BMC_D2,
	BMC_D3,
	BMC_D4,
	BMC_EXT,
	BMC_CKSUM,
	BMC_DUMMY,
};

/*
 * driver hardware numbers
 */
static const uint32_t NUM_DO_CHAN = 16;
static const uint32_t NUM_DI_CHAN = 24;

static DECLARE_COMPLETION(done);

/*
 * module configuration and data variables
 * found at /sys/modules/daq_bmc/parameters
 */
static int32_t daqbmc_conf = PICSL12; // value 0
module_param(daqbmc_conf, int, S_IRUGO);
MODULE_PARM_DESC(daqbmc_conf, "hardware configuration: default 0=BMCboard standard, 1=BMCboard without DI or DO");
static int32_t di_conf = 1; // default true
module_param(di_conf, int, S_IRUGO);
MODULE_PARM_DESC(di_conf, "make digital input subdevice");
static int32_t do_conf = 1; // default true
module_param(do_conf, int, S_IRUGO);
MODULE_PARM_DESC(do_conf, "make digital output subdevice");
static uint32_t ai_count = 0;
module_param(ai_count, uint, S_IRUGO);
MODULE_PARM_DESC(ai_count, "total adc samples");
static uint32_t ao_count = 0;
module_param(ao_count, uint, S_IRUGO);
static uint32_t do_count = 0;
module_param(do_count, uint, S_IRUGO);
static uint32_t di_count = 0;
module_param(di_count, uint, S_IRUGO);
static uint32_t serial_count = 0;
module_param(serial_count, uint, S_IRUGO);
MODULE_PARM_DESC(ao_count, "total dac samples");
static uint32_t hunk_count = 0;
module_param(hunk_count, uint, S_IRUGO);
static int32_t hunk_len = HUNK_LEN;
module_param(hunk_len, int, S_IRUGO);
static int32_t bmc_autoload = 1;
module_param(bmc_autoload, int, S_IRUGO);
MODULE_PARM_DESC(bmc_autoload, "boot autoload: default 1=load module");
static int32_t bmc_type = BMCBOARD;
module_param(bmc_type, int, S_IRUGO);
MODULE_PARM_DESC(bmc_type, "i/o board type: default 0=BMCboard");
static int32_t bmc_rev = 3;
module_param(bmc_rev, int, S_IRUGO);
MODULE_PARM_DESC(bmc_rev, "board revision: default 3=OPI Zero 3 4G");
static int32_t speed_test = 0;
module_param(speed_test, int, S_IRUGO);
MODULE_PARM_DESC(speed_test, "sample timing test: 1=enable");
static int32_t special_test = 0;
module_param(special_test, int, S_IRUGO);
MODULE_PARM_DESC(special_test, "special timing test: 1=enable");
static int32_t lsamp_size = 0;
module_param(lsamp_size, int, S_IRUGO);
MODULE_PARM_DESC(lsamp_size, "16 or 32 bit lsampl size: 0=16 bit");
static int32_t use_hunking = 0;
module_param(use_hunking, int, S_IRUGO);

struct bmc_packet_type {
	uint8_t bmc_byte_t[Q84_BYTES];
	uint8_t bmc_byte_r[Q84_BYTES];
	struct spi_transfer one_t;
	struct spi_message m[1]; // make this a pointer
};

struct daqbmc_device {
	const char *name;
	int32_t ai_subdev_flags;
	int32_t ao_subdev_flags;
	uint32_t min_acq_ns;
	uint32_t rate_min;
	uint32_t max_speed_hz;
	uint32_t spi_mode;
	uint32_t spi_bpw;
	uint32_t n_chan_bits;
	uint32_t n_chan;
	uint32_t n_transfers;
	uint32_t n_subdev;
};

/*
 * Use only MODE 3 for Orange PI SPI connections, MODE 0 seems to have issues on this board
 * Max SCK 12MHz, normally runs at 10MHz
 */
static const struct daqbmc_device daqbmc_devices[] = {
	{
		.name = "PICSL12",
		.ai_subdev_flags = SDF_READABLE | SDF_GROUND | SDF_COMMON,
		.ao_subdev_flags = SDF_GROUND | SDF_CMD_WRITE | SDF_WRITABLE,
		.max_speed_hz = 10000000,
		.min_acq_ns = 180000,
		.rate_min = 1000,
		.spi_mode = 3,
		.spi_bpw = 8,
		.n_chan_bits = 12,
		.n_chan = 16,
		.n_transfers = Q84_BYTES,
		.n_subdev = SUBDEV,
	},
	{
		.name = "PICSL12_AO",
		.ai_subdev_flags = SDF_READABLE | SDF_GROUND | SDF_COMMON,
		.ao_subdev_flags = SDF_GROUND | SDF_CMD_WRITE | SDF_WRITABLE,
		.max_speed_hz = 10000000,
		.min_acq_ns = 180000,
		.rate_min = 1000,
		.spi_mode = 3,
		.spi_bpw = 8,
		.n_chan_bits = 12,
		.n_chan = 16,
		.n_transfers = Q84_BYTES,
		.n_subdev = SUBDEV_OAO,
	},
	{
		.name = "special",
		.ai_subdev_flags = SDF_READABLE | SDF_GROUND | SDF_CMD_READ | SDF_COMMON,
		.max_speed_hz = 64000000,
		.min_acq_ns = 30000,
		.rate_min = 30000,
		.spi_mode = 3,
		.spi_bpw = 8,
		.n_chan_bits = 12,
		.n_chan = 2,
		.n_transfers = SPECIAL_LEN,
	},
};

struct daqbmc_board {
	const char *name;
	int32_t board_type;
	int32_t n_aichan;
	uint8_t n_aichan_bits;
	int32_t n_aochan, n_aochan_mask;
	uint8_t n_aochan_bits;
	uint32_t ai_ns_min_calc;
	uint32_t ao_ns_min_calc;
	uint32_t ai_cs;
	uint32_t ao_cs;
	int32_t ai_node;
	int32_t ao_node;
};

static const struct daqbmc_board daqbmc_boards[] = {
	{
		.name = "BMCboard",
		.board_type = 0,
		.n_aichan = 16,
		.n_aichan_bits = 12,
		.n_aochan = 1,
		.n_aochan_mask = 0x01,
		.n_aochan_bits = 8,
		.ai_ns_min_calc = 20000000,
		.ao_ns_min_calc = 20000000,
		.ai_cs = 1,
		.ao_cs = 1,
		.ai_node = 1,
		.ao_node = 2,
	},
	{
		.name = "Testboard",
		.board_type = 0,
		.n_aichan = 16,
		.n_aichan_bits = 12,
		.n_aochan = 1,
		.n_aochan_mask = 0x01,
		.n_aochan_bits = 12,
		.ai_ns_min_calc = 1000000,
		.ao_ns_min_calc = 1000000,
		.ai_cs = 1,
		.ao_cs = 1,
		.ai_node = 1,
		.ao_node = 2,
	},
};

static const struct comedi_lrange daqbmc_ai_range3_300 = {1,
	{
		UNI_RANGE(3.300),
	}};
static const struct comedi_lrange daqbmc_ai_rangeq84 = {1,
	{
		UNI_RANGE(4.096),
	}};
static const struct comedi_lrange daqbmc_ai_range2_048 = {1,
	{
		UNI_RANGE(2.048),
	}};

static const struct comedi_lrange daqbmc_ao_range = {1,
	{/* gains 1,2 */
		UNI_RANGE(4.096),
	}};

/*
 * SPI attached devices used by Comedi for I/O
 */
struct spi_param_type {
	uint32_t range : 2;
	uint32_t bits : 2;
	uint32_t pic18 : 2;
	uint32_t chan : 8;
	struct spi_device *spi;
	int32_t device_type;
	const struct daqbmc_device *device_spi;
	int32_t hunk_size; /* the number of needed values returned as data */
	struct timer_list my_timer;
	struct task_struct *daqbmc_task;
};

/*
 * Comedi SPI device I/O buffer control structure
 */
struct comedi_spibmc {
	uint8_t *tx_buff;
	uint8_t *rx_buff;
	struct spi_transfer t[HUNK_LEN], one_t; // ping buffer
	uint32_t ping_pong, upper_lower;
	struct spi_message m[2];
	uint32_t delay_usecs;
	uint32_t delay_usecs_freerun;
	uint32_t mix_delay_usecs;
	uint32_t delay_usecs_calc;
	uint32_t mix_delay_usecs_calc;
	struct list_head device_entry;
	struct spi_param_type slave;
	ktime_t kmin;
	uint32_t delay_nsecs;
	struct spi_delay delay;
	struct spi_delay cs_delay;
	struct spi_delay word_delay;
	struct comedi_subdevice *sub;
};

/*
 * OPi board control state variables
 */
struct daqbmc_private {
	uint32_t checkmark;
	int32_t board_rev;
	int32_t num_subdev;
	unsigned long state_bits;
	uint32_t ao_timer;
	uint32_t ai_count, ao_count, ao_counter, hunk_count, do_count, di_count, mode_count;
	uint32_t ai_conv_delay_usecs, ai_conv_delay_10nsecs, ai_cmd_delay_usecs;
	int32_t ai_chan, ao_chan, ai_range, ao_range;
	struct mutex drvdata_lock, cmd_lock;
	uint32_t ai_hunk;
	uint32_t run;
	uint32_t use_hunking : 1;
	uint32_t ai_mix : 1;
	uint32_t ai_neverending : 1;
	uint32_t ao_neverending : 1;
	uint32_t timer : 1;
	uint32_t ai_cmd_canceled : 1;
	uint32_t ao_cmd_canceled : 1;
	uint32_t timing_lockout : 1;
	int32_t mix_chan;
	uint32_t ai_scans; /*  length of scanlist */
	int32_t ai_scans_left; /*  number left to finish */
	uint32_t ao_scans; /*  length of scanlist */
	struct spi_param_type *ai_spi;
	struct spi_param_type *ao_spi;
	int32_t ai_node;
	int32_t ao_node;
	uint32_t cpu_nodes;
	bool smp;
	struct comedi_device *dev;
	struct timer_list ai_timer;
	void (*digitalWrite) (struct comedi_device *dev, uint32_t pin, uint32_t * value);
	int (*digitalRead) (struct comedi_device *dev, uint32_t * data);
	float *scalar4_ptr, *scalar5_ptr, scalar4, scalar5;
};

static int32_t daqbmc_spi_setup(struct spi_param_type *);
static int32_t daqbmc_spi_probe(struct comedi_device *,
	struct spi_param_type *);
static int32_t daqbmc_ao_cancel(struct comedi_device *,
	struct comedi_subdevice *);
static void daqbmc_handle_ao_eoc(struct comedi_device *,
	struct comedi_subdevice *);
static void my_timer_ai_callback(struct timer_list *);
static int32_t daqbmc_ai_get_sample(struct comedi_device *,
	struct comedi_subdevice *);
static void daqbmc_ao_put_sample(struct comedi_device *,
	struct comedi_subdevice *,
	uint16_t);
static void daqbmc_ao_put_samples(struct comedi_device *,
	struct comedi_subdevice *,
	uint16_t *);
static int32_t bmc_spi_exchange(struct comedi_device *, struct bmc_packet_type *);

/*
 * piBoardRev:
 *	Return a number representing the hardware revision of the board.
 *	Revision is currently 3 only. -1 is returned on error.
 *********************************************************************
 */
static int32_t piBoardRev(struct comedi_device *dev)
{
	int32_t boardRev = 3; // hardwired for now
//	uint32_t *efuse = (uint32_t *) 0x01c14200;

	/*
	 * set module param
	 */
//	dev_info(dev->class_dev, "board SID 0X%X\n", *efuse);
	bmc_rev = boardRev;
	return boardRev;
}

/*
 * My standard 9 byte SPI data packet with ~20us spacing between bytes
 */
static int32_t bmc_spi_exchange(struct comedi_device *dev, struct bmc_packet_type * packet)
{
	struct comedi_subdevice *s = dev->read_subdev;
	struct spi_param_type *spi_data = s->private;
	struct spi_device *spi = spi_data->spi;
	int32_t ret = 0;
	ktime_t slower = SPI_GAP_LONG;

	if (spi == NULL) {
		ret = -ESHUTDOWN;
		return ret;
	}
	/*
	 * use nine spi transfers for the complete SPI transaction
	 * we need the inter-byte processing time on the slave side
	 * with only a two byte FIFO
	 */
	packet->one_t.speed_hz = spi->max_speed_hz;

	/*
	 * send the cmd and 4-bit channel data
	 */
	packet->one_t.tx_buf = &packet->bmc_byte_t[BMC_CMD];
	packet->one_t.rx_buf = &packet->bmc_byte_r[BMC_CMD];
	packet->one_t.cs_change = false;
	packet->one_t.len = 1;
	spi_message_init_with_transfers(packet->m, &packet->one_t, 1); //one transfer per message
	spi_bus_lock(spi->master);
	ret = spi_sync_locked(spi, packet->m);
	spi_bus_unlock(spi->master);
	if (ret == 0) {
		ret = packet->m->actual_length;
	}
	__set_current_state(TASK_UNINTERRUPTIBLE);
	schedule_hrtimeout_range(&slower, 0, HRTIMER_MODE_REL_PINNED);

	/*
	 * send the data
	 */
	packet->one_t.tx_buf = &packet->bmc_byte_t[BMC_D0];
	packet->one_t.rx_buf = &packet->bmc_byte_r[BMC_D0];
	packet->one_t.cs_change = false;
	packet->one_t.len = 1;
	spi_message_init_with_transfers(packet->m, &packet->one_t, 1); //one transfer per message
	spi_bus_lock(spi->master);
	spi_sync_locked(spi, packet->m);
	spi_bus_unlock(spi->master);
	slower = SPI_GAP;
	__set_current_state(TASK_UNINTERRUPTIBLE);
	schedule_hrtimeout_range(&slower, 0, HRTIMER_MODE_REL_PINNED);

	packet->one_t.tx_buf = &packet->bmc_byte_t[BMC_D1];
	packet->one_t.rx_buf = &packet->bmc_byte_r[BMC_D1];
	packet->one_t.cs_change = false;
	packet->one_t.len = 1;
	spi_message_init_with_transfers(packet->m, &packet->one_t, 1); //one transfer per message
	spi_bus_lock(spi->master);
	spi_sync_locked(spi, packet->m);
	spi_bus_unlock(spi->master);
	__set_current_state(TASK_UNINTERRUPTIBLE);
	schedule_hrtimeout_range(&slower, 0, HRTIMER_MODE_REL_PINNED);

	packet->one_t.tx_buf = &packet->bmc_byte_t[BMC_D2];
	packet->one_t.rx_buf = &packet->bmc_byte_r[BMC_D2];
	packet->one_t.cs_change = false;
	packet->one_t.len = 1;
	spi_message_init_with_transfers(packet->m, &packet->one_t, 1); //one transfer per message
	spi_bus_lock(spi->master);
	spi_sync_locked(spi, packet->m);
	spi_bus_unlock(spi->master);
	__set_current_state(TASK_UNINTERRUPTIBLE);
	schedule_hrtimeout_range(&slower, 0, HRTIMER_MODE_REL_PINNED);

	packet->one_t.tx_buf = &packet->bmc_byte_t[BMC_D3];
	packet->one_t.rx_buf = &packet->bmc_byte_r[BMC_D3];
	packet->one_t.cs_change = false;
	packet->one_t.len = 1;
	spi_message_init_with_transfers(packet->m, &packet->one_t, 1); //one transfer per message
	spi_bus_lock(spi->master);
	spi_sync_locked(spi, packet->m);
	spi_bus_unlock(spi->master);
	__set_current_state(TASK_UNINTERRUPTIBLE);
	schedule_hrtimeout_range(&slower, 0, HRTIMER_MODE_REL_PINNED);

	packet->one_t.tx_buf = &packet->bmc_byte_t[BMC_D4];
	packet->one_t.rx_buf = &packet->bmc_byte_r[BMC_D4];
	packet->one_t.cs_change = false;
	packet->one_t.len = 1;
	spi_message_init_with_transfers(packet->m, &packet->one_t, 1); //one transfer per message
	spi_bus_lock(spi->master);
	spi_sync_locked(spi, packet->m);
	spi_bus_unlock(spi->master);
	__set_current_state(TASK_UNINTERRUPTIBLE);
	schedule_hrtimeout_range(&slower, 0, HRTIMER_MODE_REL_PINNED);

	/*
	 * extended channel data
	 */
	packet->one_t.tx_buf = &packet->bmc_byte_t[BMC_EXT];
	packet->one_t.rx_buf = &packet->bmc_byte_r[BMC_EXT];
	packet->one_t.cs_change = false;
	packet->one_t.len = 1;
	spi_message_init_with_transfers(packet->m, &packet->one_t, 1); //one transfer per message
	spi_bus_lock(spi->master);
	spi_sync_locked(spi, packet->m);
	spi_bus_unlock(spi->master);
	__set_current_state(TASK_UNINTERRUPTIBLE);
	schedule_hrtimeout_range(&slower, 0, HRTIMER_MODE_REL_PINNED);
	/*
	 * packet [0..6] 8-bit added checksum
	 */
	packet->one_t.tx_buf = &packet->bmc_byte_t[BMC_CKSUM];
	packet->one_t.rx_buf = &packet->bmc_byte_r[BMC_CKSUM];
	packet->one_t.cs_change = false;
	packet->one_t.len = 1;
	spi_message_init_with_transfers(packet->m, &packet->one_t, 1); //one transfer per message
	spi_bus_lock(spi->master);
	spi_sync_locked(spi, packet->m);
	spi_bus_unlock(spi->master);
	__set_current_state(TASK_UNINTERRUPTIBLE);
	schedule_hrtimeout_range(&slower, 0, HRTIMER_MODE_REL_PINNED);

	/*
	 * dummy byte to transfer last byte of data
	 */
	packet->one_t.tx_buf = &packet->bmc_byte_t[BMC_DUMMY];
	packet->one_t.rx_buf = &packet->bmc_byte_r[BMC_DUMMY];
	packet->one_t.cs_change = false;
	packet->one_t.len = 1;
	spi_message_init_with_transfers(packet->m, &packet->one_t, 1); //one transfer per message
	spi_bus_lock(spi->master);
	spi_sync_locked(spi, packet->m);
	spi_bus_unlock(spi->master);
	__set_current_state(TASK_UNINTERRUPTIBLE);
	schedule_hrtimeout_range(&slower, 0, HRTIMER_MODE_REL_PINNED);

	return ret;
}


/*
 * A client must be connected with a valid comedi cmd
 * and *data a pointer to that comedi structure
 * for this not to segfault
 */
/*
 * AO async thread
 */
static DECLARE_WAIT_QUEUE_HEAD(daqbmc_ao_thread_wq);

static int32_t daqbmc_ao_thread_function(void *data)
{
	struct comedi_device *dev = (void*) data;
	struct comedi_subdevice *s = dev->write_subdev;
	struct daqbmc_private *devpriv = dev->private;
	struct spi_param_type *spi_data = s->private;
	struct spi_device *spi = spi_data->spi;
	struct comedi_spibmc *pdata = spi->dev.platform_data;

	if (!dev) {
		return -EFAULT;
	}
	dev_info(dev->class_dev, "ao device thread start\n");

	while (!kthread_should_stop()) {
		if (likely(test_bit(AO_CMD_RUNNING, &devpriv->state_bits))) {
			daqbmc_handle_ao_eoc(dev, s);
			pdata->kmin = ktime_set(0, pdata->delay_nsecs);
			__set_current_state(TASK_UNINTERRUPTIBLE);
			schedule_hrtimeout_range(&pdata->kmin, 0,
				HRTIMER_MODE_REL_PINNED);
		} else {
			clear_bit(SPI_AO_RUN, &devpriv->state_bits);
			smp_mb__after_atomic();
			wait_event_interruptible(daqbmc_ao_thread_wq, test_bit(AO_CMD_RUNNING, &devpriv->state_bits));
			smp_mb__before_atomic();
			set_bit(SPI_AO_RUN, &devpriv->state_bits);
			smp_mb__after_atomic();
		}
	}
	return 0;
}

/*
 * AO async thread timer
 *
 */
static void daqbmc_ao_start_pacer(struct comedi_device *dev,
	bool load_timers)
{
	struct daqbmc_private *devpriv = dev->private;

	if (load_timers) {
		/* setup timer interval to 100 msecs */
		mod_timer(&devpriv->ao_spi->my_timer, jiffies
			+ msecs_to_jiffies(100));
	}
}

/*
 * DAC SPI channel and voltage gains
 */
static void daqbmc_ao_set_chan_range(struct comedi_device *dev,
	uint32_t chanspec,
	char wait)
{
	struct daqbmc_private *devpriv = dev->private;
	devpriv->ao_chan = CR_CHAN(chanspec);
	devpriv->ao_range = CR_RANGE(chanspec);

	if (wait) {
		udelay(1);
	}
}

/*
 * transfers one 16 bit value to the DAC device
 */
static void daqbmc_ao_put_sample(struct comedi_device *dev,
	struct comedi_subdevice *s,
	uint16_t value)
{
	const struct daqbmc_board *board = dev->board_ptr;
	struct daqbmc_private *devpriv = dev->private;
	uint32_t chan, range;

	struct bmc_packet_type *packet = kzalloc(SPI_BUFF_SIZE_NOHUNK, GFP_KERNEL | GFP_NOWAIT | GFP_ATOMIC);
	if (!packet) {
		return;
	}

	mutex_lock(&devpriv->drvdata_lock);
	chan = devpriv->ao_chan & board->n_aochan_mask;
	range = devpriv->ao_range;

	/* use single transfer for all bytes of the complete SPI transaction */
	packet->bmc_byte_t[BMC_CMD] = CMD_DAC_GO;
	packet->bmc_byte_t[BMC_D0] = (uint8_t) (value & 0xff);
	packet->bmc_byte_t[BMC_D1] = (uint8_t) ((value >> 8)&0xff);
	packet->bmc_byte_t[BMC_D2] = (uint8_t) (value & 0xff);
	packet->bmc_byte_t[BMC_D3] = BMC_D3;
	packet->bmc_byte_t[BMC_D4] = BMC_D4;
	packet->bmc_byte_t[BMC_EXT] = (uint8_t) range;
	packet->bmc_byte_t[BMC_CKSUM] = CHECKBYTE;
	packet->bmc_byte_t[BMC_DUMMY] = CHECKBYTE;
	bmc_spi_exchange(dev, packet);

	s->readback[chan] = value;
	devpriv->ao_count++;
	mutex_unlock(&devpriv->drvdata_lock);
	smp_mb__after_atomic();

	kfree(packet);
}

/*
 *
 */
static void daqbmc_ao_put_samples(struct comedi_device *dev,
	struct comedi_subdevice *s,
	uint16_t *value)
{
	daqbmc_ao_put_sample(dev, s, value[0]);
}

/*
 * returns one 12 bit value from the ADC device
 */
static int32_t daqbmc_ai_get_sample(struct comedi_device *dev,
	struct comedi_subdevice *s)
{
	struct daqbmc_private *devpriv = dev->private;
	uint32_t chan;
	int32_t val = 0;

	struct bmc_packet_type *packet = kzalloc(SPI_BUFF_SIZE_NOHUNK, GFP_KERNEL | GFP_NOWAIT | GFP_ATOMIC);
	if (!packet) {
		return 0;
	}

	mutex_lock(&devpriv->drvdata_lock);
	chan = devpriv->ai_chan;

	/* use single transfer for all bytes of the complete SPI transaction */
	packet->bmc_byte_t[BMC_CMD] = CMD_ADC_GO + chan;
	packet->bmc_byte_t[BMC_D0] = BMC_D0;
	packet->bmc_byte_t[BMC_D1] = BMC_D1;
	packet->bmc_byte_t[BMC_D2] = BMC_D2;
	packet->bmc_byte_t[BMC_D3] = BMC_D3;
	packet->bmc_byte_t[BMC_D4] = BMC_D4;
	packet->bmc_byte_t[BMC_EXT] = BMC_EXT;
	packet->bmc_byte_t[BMC_CKSUM] = CHECKBYTE;
	packet->bmc_byte_t[BMC_DUMMY] = CHECKBYTE;
	bmc_spi_exchange(dev, packet);

	val = (packet->bmc_byte_r[BMC_D1]);
	val += (packet->bmc_byte_r[BMC_D2] << 8);

	devpriv->ai_count++;
	mutex_unlock(&devpriv->drvdata_lock);
	clear_bit(SPI_AI_RUN, &devpriv->state_bits);
	smp_mb__after_atomic();

	kfree(packet);
	return val & s->maxdata;
}

/*
 * start chan set in ao_cmd
 * see comedi driver amplc_pci224.c
 */
static void daqbmc_handle_ao_eoc(struct comedi_device *dev,
	struct comedi_subdevice * s)
{
	struct daqbmc_private *devpriv = dev->private;
	struct comedi_cmd *cmd = &s->async->cmd;
	uint16_t sampl_val;

	if (!comedi_buf_read_samples(s, &sampl_val, cmd->chanlist_len)) {
		s->async->events |= COMEDI_CB_OVERFLOW;
		comedi_handle_events(dev, s);
		return;
	}

	if (cmd->chanlist_len < 2) {
		daqbmc_ao_set_chan_range(dev, cmd->chanlist[0], false);
		daqbmc_ao_put_sample(dev, s, sampl_val);
	} else {
		daqbmc_ao_put_samples(dev, s, &sampl_val);
	}

	if (cmd->stop_src == TRIG_COUNT &&
		s->async->scans_done >= cmd->stop_arg) {
		if (!devpriv->ao_neverending) {
			/* all data sampled */
			daqbmc_ao_cancel(dev, s);
			s->async->events |= COMEDI_CB_EOA;
		}
	}
}

static int32_t daqbmc_ao_inttrig(struct comedi_device *dev,
	struct comedi_subdevice *s,
	uint32_t trig_num)
{
	struct daqbmc_private *devpriv = dev->private;
	struct comedi_cmd *cmd = &s->async->cmd;
	int32_t ret = 1;

	if (unlikely(!devpriv)) {
		return -EFAULT;
	}

	if (trig_num != cmd->start_arg) {
		return -EINVAL;
	}

	mutex_lock(&devpriv->cmd_lock);
	dev_info(dev->class_dev, "ao inttrig\n");

	if (!test_bit(AO_CMD_RUNNING, &devpriv->state_bits)) {
		smp_mb__before_atomic();
		set_bit(AO_CMD_RUNNING, &devpriv->state_bits);
		wake_up_interruptible(&daqbmc_ao_thread_wq);
		smp_mb__after_atomic();
		devpriv->ao_cmd_canceled = false;
		s->async->inttrig = NULL;
	} else {
		ret = -EBUSY;
	}

	mutex_unlock(&devpriv->cmd_lock);
	smp_mb__after_atomic();
	return ret;
}

static int32_t daqbmc_ao_cmd(struct comedi_device *dev,
	struct comedi_subdevice * s)
{
	struct comedi_cmd *cmd = &s->async->cmd;
	struct daqbmc_private *devpriv = dev->private;
	struct spi_param_type *spi_data = s->private;
	struct spi_device *spi = spi_data->spi;
	struct comedi_spibmc *pdata = spi->dev.platform_data;
	int ret = 0;

	if (unlikely(!devpriv)) {
		return -EFAULT;
	}

	/* Cannot handle null/empty chanlist. */
	if (!cmd->chanlist || cmd->chanlist_len == 0) {
		return -EINVAL;
	}

	mutex_lock(&devpriv->cmd_lock);
	dev_info(dev->class_dev, "ao_cmd\n");
	if (test_bit(AO_CMD_RUNNING, &devpriv->state_bits)) {
		dev_info(dev->class_dev, "ao_cmd busy\n");
		ret = -EBUSY;
		goto ao_cmd_exit;
	}

	/*
	 * inter-spacing speed adjustments update from cmd_test
	 *
	 * delay between any single conversion
	 */
	pdata->delay_usecs = pdata->delay_usecs_calc;
	pdata->delay_nsecs = pdata->delay_usecs * NSEC_PER_USEC;
	/* delay for alt mix command conversions */
	pdata->mix_delay_usecs = pdata->mix_delay_usecs_calc;

	/* for possible hunking of AO */
	if (cmd->stop_src == TRIG_COUNT) {
		devpriv->ao_scans = cmd->chanlist_len * cmd->stop_arg;
		devpriv->ao_neverending = false;
	} else {
		devpriv->ao_scans = hunk_len;
		devpriv->ao_neverending = true;
	}

	/* 1ms */
	/* timing of the scan: we set all channels at once */
	devpriv->ao_timer = cmd->scan_begin_arg / 1000;
	if (devpriv->ao_timer < 1) {
		ret = -EINVAL;
		goto ao_cmd_exit;
	}
	devpriv->ao_counter = devpriv->ao_timer;
	s->async->cur_chan = 0;
	daqbmc_ao_set_chan_range(dev, cmd->chanlist[s->async->cur_chan], false);

	if (cmd->start_src == TRIG_NOW) {
		s->async->inttrig = NULL;
		/* enable this acquisition operation */
		smp_mb__before_atomic();
		set_bit(AI_CMD_RUNNING, &devpriv->state_bits);
		wake_up_interruptible(&daqbmc_ao_thread_wq);
		smp_mb__after_atomic();
		devpriv->ao_cmd_canceled = false;
	} else {
		/* TRIG_INT */
		/* wait for an internal signal */
		s->async->inttrig = daqbmc_ao_inttrig;
	}

ao_cmd_exit:
	mutex_unlock(&devpriv->cmd_lock);
	smp_mb__after_atomic();
	return ret;
}

/*
 * get close to a good sample spacing for one second,
 * test_mode is to see what the max sample rate is
 */
static int32_t daqbmc_ao_delay_rate(struct comedi_device *dev,
	int32_t rate,
	int32_t device_type,
	bool test_mode)
{
	int32_t spacing_usecs = 0, sample_freq, total_sample_time, delay_time;
	if (test_mode) {
		dev_info(dev->class_dev,
			"ao speed testing: rate %i, spacing usecs %i\n",
			rate, spacing_usecs);
		return spacing_usecs;
	}

	if (rate > MAX_BOARD_RATE) {
		rate = MAX_BOARD_RATE;
	}
	sample_freq = MAX_BOARD_RATE / rate;
	/* ns time needed for all samples in one second */
	total_sample_time = 30000 * sample_freq;
	delay_time = MAX_BOARD_RATE - total_sample_time; /* what's left */
	if (delay_time >= sample_freq) { /* something */
		spacing_usecs = (delay_time / sample_freq) / NSEC_PER_USEC;
		if (spacing_usecs < 0)
			spacing_usecs = 0;
	} else { /* or nothing */
		spacing_usecs = 0;
	}
	dev_info(dev->class_dev, "ao rate %i, spacing usecs %i\n", rate, spacing_usecs);
	return spacing_usecs;
}

static int32_t daqbmc_ao_cmdtest(struct comedi_device *dev,
	struct comedi_subdevice *s,
	struct comedi_cmd * cmd)
{
	const struct daqbmc_board *board = dev->board_ptr;
	struct daqbmc_private *devpriv = dev->private;
	struct spi_param_type *spi_data = s->private;
	struct spi_device *spi = spi_data->spi;
	struct comedi_spibmc *pdata = spi->dev.platform_data;
	int32_t i, err = 0;
	uint32_t tmp_timer;

	if (unlikely(!devpriv)) {
		return -EFAULT;
	}

	/* Step 1 : check if triggers are trivially valid */

	err |= comedi_check_trigger_src(&cmd->start_src, TRIG_NOW | TRIG_INT);
	err |= comedi_check_trigger_src(&cmd->convert_src, TRIG_NOW);
	err |= comedi_check_trigger_src(&cmd->scan_begin_src, TRIG_TIMER);
	err |= comedi_check_trigger_src(&cmd->scan_end_src, TRIG_COUNT);
	err |= comedi_check_trigger_src(&cmd->stop_src, TRIG_COUNT | TRIG_NONE);

	if (err) {
		return 1;
	}

	/* Step 2a : make sure trigger sources are unique */

	err |= comedi_check_trigger_is_unique(cmd->start_src);
	err |= comedi_check_trigger_is_unique(cmd->stop_src);

	/* Step 2b : and mutually compatible */

	if (err) {
		return 2;
	}

	/* Step 3: check if arguments are trivially valid */
	switch (cmd->start_src) {
	case TRIG_NOW:
	case TRIG_EXT:
		err |= comedi_check_trigger_arg_is(&cmd->start_arg, 0);
		break;
	case TRIG_INT:
		/* start_arg is the internal trigger (any value) */
		break;
	}

	if (cmd->scan_begin_src == TRIG_FOLLOW) { /* internal trigger */
		err |= comedi_check_trigger_arg_is(&cmd->scan_begin_arg, 0);
	}

	if (cmd->scan_begin_src == TRIG_TIMER) {
		i = 1;
		/* find a power of 2 for the number of channels */
		while (i < (cmd->chanlist_len)) {
			i = i * 2;
		}
		tmp_timer = (board->ao_ns_min_calc / 2) * i;
		err |= comedi_check_trigger_arg_min(&cmd->scan_begin_arg,
			tmp_timer); /* fastest */
		/* now calc the real sampling rate with all the
		 * rounding errors */
		tmp_timer = ((uint32_t) (cmd->scan_begin_arg
			/ 20000)) * 20000 / 10;
		pdata->delay_usecs_calc = daqbmc_ao_delay_rate(dev,
			tmp_timer,
			spi_data->device_type,
			speed_test);
		err |= comedi_check_trigger_arg_max(&cmd->scan_begin_arg,
			MAX_BOARD_RATE);
	} else {
		pdata->delay_usecs_calc = 0;
	}
	pdata->delay_nsecs = pdata->delay_usecs_calc * NSEC_PER_USEC;

	err |= comedi_check_trigger_arg_is(&cmd->scan_end_arg,
		cmd->chanlist_len);

	if (cmd->stop_src == TRIG_COUNT) {
		err |= comedi_check_trigger_arg_min(&cmd->stop_arg, 1);
	} else { /* TRIG_NONE */
		err |= comedi_check_trigger_arg_is(&cmd->stop_arg, 0);
	}

	if (err) {
		return 3;
	}

	/*
	 *  step 4: fix up any arguments
	 * no fixups
	 */

	return 0;
}

/*
 * Possible DMA timer that's not currently useful except for speed benchmarks
 */
static void my_timer_ai_callback(struct timer_list *t)
{
	struct daqbmc_private *devpriv = from_timer(devpriv, t, ai_timer);
	struct comedi_device *dev = devpriv->dev;

	if (!dev) {
		return;
	}

	if (!devpriv->run) {
		devpriv->run = true;
		devpriv->timer = true;
	}
	daqbmc_ao_start_pacer(dev, true);
	if (speed_test) {
	}
}

static int32_t daqbmc_ao_cancel(struct comedi_device *dev,
	struct comedi_subdevice * s)
{
	struct daqbmc_private *devpriv = dev->private;
	int32_t count = 500;

	if (unlikely(!devpriv)) {
		return -EFAULT;
	}

	if (!test_bit(AO_CMD_RUNNING, &devpriv->state_bits)) {
		return 0;
	}

	dev_info(dev->class_dev, "ao cancel start\n");
	ao_count = devpriv->ao_count;
	s->async->cur_chan = 0;
	clear_bit(AO_CMD_RUNNING, &devpriv->state_bits);
	smp_mb__after_atomic();
	do { /* wait if needed to SPI to clear or timeout */
		schedule(); /* force a context switch to stop the AO thread */
		usleep_range(750, 1000);
	} while (test_bit(SPI_AO_RUN, &devpriv->state_bits) && (count--));

	usleep_range(750, 1000);
	devpriv->ao_cmd_canceled = true;
	s->async->inttrig = NULL;
	dev_info(dev->class_dev, "ao cancel end\n");
	return 0;
}

/*
 * DO SPI Q84 transaction
 */
static void digitalWriteOPi(struct comedi_device *dev,
	uint32_t pin,
	uint32_t *value)
{
	struct daqbmc_private *devpriv = dev->private;
	uint32_t val_value;
	struct bmc_packet_type *packet = kzalloc(SPI_BUFF_SIZE_NOHUNK, GFP_KERNEL | GFP_NOWAIT | GFP_ATOMIC);

	if (!packet) {
		return;
	}

	val_value = value[0];

	/* use single transfer for all bytes of the complete SPI transaction */
	packet->bmc_byte_t[BMC_CMD] = CMD_PORT_GO;
	packet->bmc_byte_t[BMC_D0] = (uint8_t) (val_value & 0xff);
	packet->bmc_byte_t[BMC_D1] = (uint8_t) ((val_value >> 8)&0xff);
	packet->bmc_byte_t[BMC_D2] = (uint8_t) ((val_value >> 16)&0xff);
	packet->bmc_byte_t[BMC_D3] = BMC_D3;
	packet->bmc_byte_t[BMC_D4] = BMC_D4;
	packet->bmc_byte_t[BMC_EXT] = BMC_EXT;
	packet->bmc_byte_t[BMC_CKSUM] = CHECKBYTE;
	packet->bmc_byte_t[BMC_DUMMY] = CHECKBYTE;
	bmc_spi_exchange(dev, packet);

	devpriv->do_count++;
	kfree(packet);
}

/*
 * DI SPI Q84 transaction
 */
static int digitalReadOPi(struct comedi_device *dev,
	uint32_t * data)
{
	struct daqbmc_private *devpriv = dev->private;

	uint32_t val_value;
	struct bmc_packet_type *packet = kzalloc(SPI_BUFF_SIZE_NOHUNK, GFP_KERNEL | GFP_NOWAIT | GFP_ATOMIC);
	if (!packet) {
		return 0;
	}

	/* use single transfer for all bytes of the complete SPI transaction */
	packet->bmc_byte_t[BMC_CMD] = CMD_PORT_GET;
	packet->bmc_byte_t[BMC_D0] = BMC_D0;
	packet->bmc_byte_t[BMC_D1] = BMC_D1;
	packet->bmc_byte_t[BMC_D2] = BMC_D2;
	packet->bmc_byte_t[BMC_D3] = BMC_D3;
	packet->bmc_byte_t[BMC_D4] = BMC_D4;
	packet->bmc_byte_t[BMC_EXT] = BMC_EXT;
	packet->bmc_byte_t[BMC_CKSUM] = CHECKBYTE;
	packet->bmc_byte_t[BMC_DUMMY] = CHECKBYTE;
	bmc_spi_exchange(dev, packet);

	val_value = packet->bmc_byte_r[BMC_D1];
	val_value += (packet->bmc_byte_r[BMC_D2] << 8);
	val_value += (packet->bmc_byte_r[BMC_DUMMY] << 16);
	val_value = val_value >> 1; // shift out the tic12400 parity bit from results

	devpriv->di_count++;
	kfree(packet);

	return val_value;
}

/*
 * Handle Digital output requests
 */
static int daqbmc_do_insn_bits(struct comedi_device *dev,
	struct comedi_subdevice *s,
	struct comedi_insn *insn,
	uint32_t * data)
{
	struct daqbmc_private *devpriv = dev->private;
	uint32_t pinOPi;

	if (unlikely(!devpriv)) {
		return -EFAULT;
	}
	devpriv->digitalWrite = digitalWriteOPi;

	/* s->state contains the GPIO bits */
	if (comedi_dio_update_state(s, data)) {
		pinOPi = s->state;
		devpriv->digitalWrite(dev, pinOPi, data);
	}

	data[1] = s->state;
	do_count++;
	return insn->n;
}

/*
 * Serial TX/RX SPI Q84 transaction, 24-bit data
 * This is memory mapped to the memory sub-device
 */
static void serialWriteOPi(struct comedi_device *dev,
	uint32_t chan,
	uint32_t *value)
{
	uint32_t val_value;
	struct bmc_packet_type *packet = kzalloc(SPI_BUFF_SIZE_NOHUNK, GFP_KERNEL | GFP_NOWAIT | GFP_ATOMIC);

	if (!packet) {
		return;
	}

	val_value = value[0];
	if (chan < MEM_BLOCKS) {
		// send TX packet
		/* use single transfer for all bytes of the complete SPI transaction */
		packet->bmc_byte_t[BMC_CMD] = CMD_CHAR_GO;
		packet->bmc_byte_t[BMC_D0] = (uint8_t) (val_value & 0xff); // serial data
		packet->bmc_byte_t[BMC_D1] = (uint8_t) chan; // serial channel
		packet->bmc_byte_t[BMC_D2] = (uint8_t) ((val_value >> 16)&0xff); // serial options
		packet->bmc_byte_t[BMC_D3] = BMC_D3;
		packet->bmc_byte_t[BMC_D4] = BMC_D4;
		packet->bmc_byte_t[BMC_EXT] = BMC_EXT;
		packet->bmc_byte_t[BMC_CKSUM] = CHECKBYTE;
		packet->bmc_byte_t[BMC_DUMMY] = CHECKBYTE;
		bmc_spi_exchange(dev, packet);
	}

	kfree(packet);
}

/*
 * returns data from the BMC data buffer
 */
static int32_t daqbmc_sio_get_sample(struct comedi_device *dev,
	struct comedi_subdevice *s)
{
	uint32_t chan = 4;
	int32_t val = 0;

	struct bmc_packet_type *packet = kzalloc(SPI_BUFF_SIZE_NOHUNK, GFP_KERNEL | GFP_NOWAIT | GFP_ATOMIC);
	if (!packet) {
		return 0;
	}

	// always check for received data
	packet->bmc_byte_t[BMC_CMD] = CMD_CHAR_GET;
	packet->bmc_byte_t[BMC_D0] = BMC_D0;
	packet->bmc_byte_t[BMC_D1] = (uint8_t) chan;
	packet->bmc_byte_t[BMC_D2] = BMC_D2;
	packet->bmc_byte_t[BMC_D3] = BMC_D3;
	packet->bmc_byte_t[BMC_D4] = BMC_D4;
	packet->bmc_byte_t[BMC_EXT] = BMC_EXT;
	packet->bmc_byte_t[BMC_CKSUM] = CHECKBYTE;
	packet->bmc_byte_t[BMC_DUMMY] = CHECKBYTE;
	bmc_spi_exchange(dev, packet);

	val = packet->bmc_byte_r[BMC_DUMMY]; // serial data
	val += (packet->bmc_byte_r[BMC_D2] << 8); // serial channel
	val += (packet->bmc_byte_r[BMC_D1] << 16); // serial data is fresh

	kfree(packet);
	return val;
}

/*
 * Handle memory-mapped serial port requests
 */
static int daqbmc_sio_insn_write(struct comedi_device *dev,
	struct comedi_subdevice *s,
	struct comedi_insn *insn,
	uint32_t * data)
{
	struct daqbmc_private *devpriv = dev->private;
	unsigned int chan = CR_CHAN(insn->chanspec);
	uint32_t val = 0;

	if (unlikely(!devpriv)) {
		return -EFAULT;
	}

	if (insn->n > 0) {
		val = data[insn->n - 1];

		serialWriteOPi(dev, chan, &val);
		data[1] = val;
		serial_count++;
	}
	return insn->n;
}

static int daqbmc_sio_insn_read(struct comedi_device *dev,
	struct comedi_subdevice *s,
	struct comedi_insn *insn,
	uint32_t * data)
{
	struct daqbmc_private *devpriv = dev->private;
	int32_t n;

	if (unlikely(!devpriv)) {
		return -EFAULT;
	}

	/* convert n samples */
	for (n = 0; n < insn->n; n++) {
		data[n] = daqbmc_sio_get_sample(dev, s);
	}

	return insn->n;
}

/*
 * Handle Digital input requests
 */
static int daqbmc_di_insn_bits(struct comedi_device *dev,
	struct comedi_subdevice *s,
	struct comedi_insn *insn,
	uint32_t * data)
{
	struct daqbmc_private *devpriv = dev->private;
	uint32_t pinOPi;

	if (unlikely(!devpriv)) {
		return -EFAULT;
	}
	devpriv->digitalRead = digitalReadOPi;

	pinOPi = digitalReadOPi(dev, data);

	data[1] = pinOPi;
	di_count++;
	return 2;
}

/*
 * Talk to the ADC via the SPI Q84
 */
static int32_t daqbmc_ai_rinsn(struct comedi_device *dev,
	struct comedi_subdevice *s,
	struct comedi_insn *insn,
	uint32_t * data)
{
	struct daqbmc_private *devpriv = dev->private;
	int32_t ret = -EBUSY;
	int32_t n;

	if (unlikely(!devpriv)) {
		return -EFAULT;
	}

	mutex_lock(&devpriv->cmd_lock);
	if (unlikely(test_bit(AI_CMD_RUNNING, &devpriv->state_bits))) {
		goto ai_read_exit;
	}

	devpriv->ai_hunk = false;

	devpriv->ai_chan = CR_CHAN(insn->chanspec);
	devpriv->ai_range = CR_RANGE(insn->chanspec);

	/* convert n samples */
	for (n = 0; n < insn->n; n++) {
		data[n] = daqbmc_ai_get_sample(dev, s);
	}
	ai_count = devpriv->ai_count;
	ret = 0;
ai_read_exit:
	mutex_unlock(&devpriv->cmd_lock);
	smp_mb__after_atomic();
	return ret ? ret : insn->n;
}

/*
 * write to the DAC via SPI Q84 and read the last value back DON'T LOCK
 */
static int32_t daqbmc_ao_winsn(struct comedi_device *dev,
	struct comedi_subdevice *s,
	struct comedi_insn *insn,
	uint32_t * data)
{
	struct daqbmc_private *devpriv = dev->private;
	uint32_t chan = CR_CHAN(insn->chanspec);
	uint32_t n, val;

	daqbmc_ao_set_chan_range(dev, chan, false);
	for (n = 0; n < insn->n; n++) {
		val = data[n];
		daqbmc_ao_put_sample(dev, s, val);
	}
	ao_count = devpriv->ao_count;
	return insn->n;
}

/*
 * make AO thread for the spi i/o stream
 */
static int32_t daqbmc_create_thread(struct comedi_device *dev,
	struct daqbmc_private * devpriv)
{
	const char hunk_thread_name[] = "daqbmch", thread_name[] = "daqbmc";
	const char *name_ptr;

	if (devpriv->use_hunking) {
		name_ptr = hunk_thread_name;
	} else {
		name_ptr = thread_name;
	}

	devpriv->ao_spi->daqbmc_task =
		kthread_create_on_node(&daqbmc_ao_thread_function,
		(void *) dev,
		cpu_to_node(devpriv->ao_node),
		"%s_d/%d", name_ptr,
		devpriv->ao_node);
	if (!IS_ERR(devpriv->ao_spi->daqbmc_task)) {
		kthread_bind(devpriv->ao_spi->daqbmc_task, devpriv->ao_node);
		wake_up_process(devpriv->ao_spi->daqbmc_task);
	} else {
		return PTR_ERR(devpriv->ao_spi->daqbmc_task);
	}
	return 0;
}

/*
 * returns slave device configuration data
 * used to set the Analog only mode of the board automatically
 * a return value of 0xff means no board detected
 */
static int32_t daqbmc_bmc_get_config(struct comedi_device *dev)
{
	struct daqbmc_private *devpriv = dev->private;
	int32_t val = 0;
	uint32_t chan = 1;

	struct bmc_packet_type *packet = kzalloc(SPI_BUFF_SIZE_NOHUNK, GFP_KERNEL | GFP_NOWAIT | GFP_ATOMIC);
	if (!packet) {
		return CHECKBYTE;
	}

	mutex_lock(&devpriv->drvdata_lock);

	packet->bmc_byte_t[BMC_CMD] = CMD_ADC_GO + chan;
	packet->bmc_byte_t[BMC_D0] = BMC_D0;
	packet->bmc_byte_t[BMC_D1] = BMC_D1;
	packet->bmc_byte_t[BMC_D2] = BMC_D2;
	packet->bmc_byte_t[BMC_D3] = CMD_ADC_GO + chan;
	packet->bmc_byte_t[BMC_D4] = CMD_ADC_GO + chan;
	packet->bmc_byte_t[BMC_EXT] = CMD_ADC_GO + chan;
	packet->bmc_byte_t[BMC_CKSUM] = CHECKBYTE;
	packet->bmc_byte_t[BMC_DUMMY] = CHECKBYTE;
	bmc_spi_exchange(dev, packet);
	val = (packet->bmc_byte_r[BMC_DUMMY]);
	/* use single transfer for all bytes of the complete SPI transaction */
	packet->bmc_byte_t[BMC_CMD] = CMD_DUMMY_CFG + 4;
	packet->bmc_byte_t[BMC_D0] = BMC_D0;
	packet->bmc_byte_t[BMC_D1] = BMC_D1;
	packet->bmc_byte_t[BMC_D2] = BMC_D2;
	packet->bmc_byte_t[BMC_D3] = BMC_D3;
	packet->bmc_byte_t[BMC_D4] = BMC_D4;
	packet->bmc_byte_t[BMC_EXT] = CMD_DUMMY_CFG;
	packet->bmc_byte_t[BMC_CKSUM] = CHECKBYTE;
	packet->bmc_byte_t[BMC_DUMMY] = CHECKBYTE;
	bmc_spi_exchange(dev, packet);
	devpriv->scalar4_ptr = (float*) &packet->bmc_byte_r[BMC_D0];
	memcpy(&devpriv->scalar4, devpriv->scalar4_ptr, 4); // make a copy for use later

	packet->bmc_byte_t[BMC_CMD] = CMD_DUMMY_CFG + 5;
	packet->bmc_byte_t[BMC_D0] = BMC_D0;
	packet->bmc_byte_t[BMC_D1] = BMC_D1;
	packet->bmc_byte_t[BMC_D2] = BMC_D2;
	packet->bmc_byte_t[BMC_D3] = BMC_D3;
	packet->bmc_byte_t[BMC_D4] = BMC_D4;
	packet->bmc_byte_t[BMC_EXT] = CMD_DUMMY_CFG;
	packet->bmc_byte_t[BMC_CKSUM] = CHECKBYTE;
	packet->bmc_byte_t[BMC_DUMMY] = CHECKBYTE;
	bmc_spi_exchange(dev, packet);
	devpriv->scalar5_ptr = (float*) &packet->bmc_byte_r[BMC_D0];
	memcpy(&devpriv->scalar5, devpriv->scalar5_ptr, 4);

	devpriv->ai_count++;
	dev_info(dev->class_dev, "Calibration Scalars read from board\n");
	mutex_unlock(&devpriv->drvdata_lock);
	clear_bit(SPI_AI_RUN, &devpriv->state_bits);
	smp_mb__after_atomic();

	kfree(packet);
	return val;
}

/*
 * setup driver resources from spi bus probes from board devices for sub-device configurations
 */
static int32_t daqbmc_auto_attach(struct comedi_device *dev,
	unsigned long unused_context)
{
	const struct daqbmc_board *thisboard = &daqbmc_boards[bmc_type & 0x01];
	struct comedi_subdevice *s;
	int32_t ret;
	unsigned long spi_device_missing = 0;
	int32_t num_do_chan = NUM_DO_CHAN;
	int32_t num_di_chan = NUM_DI_CHAN;
	struct daqbmc_private *devpriv;
	struct comedi_spibmc *pdata;

	/*
	 * auto free on exit of comedi module
	 */
	devpriv = comedi_alloc_devpriv(dev, sizeof(*devpriv));
	if (!devpriv) {
		ret = -ENOMEM;
		goto daqbmc_kfree_exit;
	}

	dev_info(dev->class_dev, "bmc protocol %s\n", bmc_version);

	devpriv->checkmark = CHECKMARK;
	devpriv->dev = dev;

	/* set hardware defaults table */
	dev->board_ptr = thisboard;

	devpriv->cpu_nodes = num_online_cpus();
	if (devpriv->cpu_nodes >= SMP_CORES) {
		dev_info(dev->class_dev, "%d cpu(s) online for threads\n",
			devpriv->cpu_nodes);
		devpriv->ai_node = thisboard->ai_node;
		devpriv->ao_node = thisboard->ao_node;
		devpriv->smp = true;
	} else {
		use_hunking = false;
	}
	if (daqbmc_conf == 0 || daqbmc_conf == 1) { /* single transfers, ADC is in continuous conversion mode */
		use_hunking = false;
	}
	devpriv->use_hunking = use_hunking; /* defaults to false */

	if (speed_test) {
		dev_info(dev->class_dev, "samples per second speed test mode\n");
	}

	/*
	 * loop the spi device queue for needed devices
	 */
	if (list_empty(&device_list)) {
		ret = -ENODEV;
		goto daqbmc_kfree_exit;
	}

	/*
	 * set the wanted device bits
	 */
	set_bit(CSnA, &spi_device_missing);

	list_for_each_entry(pdata, &device_list, device_entry)
	{
		/*
		 * use smaller SPI data buffers if we don't hunk
		 */
		pdata->tx_buff = kzalloc(SPI_BUFF_SIZE_NOHUNK, GFP_KERNEL);
		if (!pdata->tx_buff) {
			ret = -ENOMEM;
			goto daqbmc_kfree_exit;
		}
		pdata->rx_buff = kzalloc(SPI_BUFF_SIZE_NOHUNK, GFP_KERNEL);
		if (!pdata->rx_buff) {
			ret = -ENOMEM;
			goto daqbmc_kfree_tx_exit;
		}

		/*
		 * we have a valid device pointer, see which one and
		 * probe/init hardware for special cases that may need
		 * many SPI transfers
		 */
		if (pdata->slave.spi->chip_select == thisboard->ai_cs) {
			devpriv->ai_spi = &pdata->slave;
			devpriv->ao_spi = &pdata->slave;
			pdata->one_t.tx_buf = pdata->tx_buff;
			pdata->one_t.rx_buf = pdata->rx_buff;
			if (daqbmc_conf == PICSL12 || daqbmc_conf == PICSL12_AO) {
				mutex_lock(&devpriv->drvdata_lock);
				dev_info(dev->class_dev, "BMCBoard using chip select : 0x%2x\n",
					thisboard->ai_cs);
				mutex_unlock(&devpriv->drvdata_lock);
				smp_mb__after_atomic();

			}
			clear_bit(CSnA, &spi_device_missing);
		} else {
		}
	}

	/*
	 * check for possible bad spibmc table entry (dupe)
	 * or missing special case device
	 */
	if (spi_device_missing) {
		dev_info(dev->class_dev, "spi_device_missing\n");
		ret = -ENODEV;
		goto daqbmc_kfree_rx_exit;
	}

	/* multi user locking */
	mutex_init(&devpriv->cmd_lock);
	mutex_init(&devpriv->drvdata_lock);

	/* Board  operation data */
	dev->board_name = thisboard->name;
	devpriv->ai_cmd_delay_usecs = PIC18_CMDD_57Q84;
	devpriv->ai_conv_delay_usecs = PIC18_CONVD_57Q84;
	devpriv->ai_neverending = true;
	devpriv->ai_mix = false;
	devpriv->ai_conv_delay_10nsecs = CONV_SPEED;
	devpriv->timing_lockout = false;

	/*
	 * Use OPi board type Zero 3
	 * need to get SoC board info for Zero 3
	 */
	devpriv->board_rev = piBoardRev(dev); /* what board are we running on? */
	if (devpriv->board_rev < 3) {
		dev_err(dev->class_dev, "invalid OPi board revision! %u\n",
			devpriv->board_rev);
		ret = -EINVAL;
		goto daqbmc_kfree_rx_exit;
	}

	dev_info(dev->class_dev,
		"%s board detection started\n",
		thisboard->name);
	devpriv->num_subdev = 0;
	if (daqbmc_spi_probe(dev, devpriv->ai_spi)) {
		devpriv->num_subdev += daqbmc_devices[PICSL12].n_subdev;
	} else {
		dev_err(dev->class_dev, "board revision detection failed!\n");
		ret = -EINVAL;
		goto daqbmc_kfree_rx_exit;
	}

	/*
	 * all Comedi buffers default to 32 bits
	 * always add AI and AO channels
	 */
	ret = comedi_alloc_subdevices(dev, devpriv->num_subdev);
	if (ret) {
		dev_err(dev->class_dev, "alloc subdevice(s) failed!\n");
		goto daqbmc_kfree_rx_exit;
	}

	if (devpriv->num_subdev > 0) { /* setup comedi for on-board devices */
		/* daq_bmc ai */
		if (devpriv->use_hunking) {
			dev_info(dev->class_dev,
				"hunk ai transfers enabled, length: %i\n",
				hunk_len);
		}
		s = &dev->subdevices[SUBDEV_AI]; // AI setup
		s->private = devpriv->ai_spi;
		s->type = COMEDI_SUBD_AI;
		/* default setups, we support single-ended (ground)  */
		s->n_chan = devpriv->ai_spi->chan;
		s->len_chanlist = devpriv->ai_spi->chan;
		s->maxdata = (1 << (thisboard->n_aichan_bits - devpriv->ai_spi->device_spi->n_chan_bits)) - 1;
		if (devpriv->ai_spi->range) {
			s->range_table = &daqbmc_ai_range2_048;
		} else {
			s->range_table = &daqbmc_ai_range3_300;
		}
		s->insn_read = daqbmc_ai_rinsn;
		s->subdev_flags = devpriv->ai_spi->device_spi->ai_subdev_flags;

		if (devpriv->ai_spi->device_type == PICSL12 || devpriv->ai_spi->device_type == PICSL12_AO) { // no async
			s->maxdata = (1 << devpriv->ai_spi->device_spi->n_chan_bits) - 1;
			s->range_table = &daqbmc_ai_rangeq84;
			s->n_chan = devpriv->ai_spi->chan;
			s->len_chanlist = devpriv->ai_spi->chan;
			s->subdev_flags = devpriv->ai_spi->device_spi->ai_subdev_flags;
		}

		if (lsamp_size != SDF_LSAMPL) {
			lsamp_size = 0;
			dev_info(dev->class_dev, "16 bit and less device buffers set to 16 bits\n");
		}
		dev->read_subdev = s;

		/* daq-bmc ao */
		s = &dev->subdevices[SUBDEV_AO];
		s->private = devpriv->ao_spi;
		s->type = COMEDI_SUBD_AO;
		/* we support single-ended (ground)  */
		s->n_chan = thisboard->n_aochan;
		s->len_chanlist = thisboard->n_aochan;
		/* analog resolution depends on the DAC chip 8,10,12 bits */
		s->maxdata = (1 << thisboard->n_aochan_bits) - 1;
		s->range_table = &daqbmc_ao_range;
		s->insn_write = daqbmc_ao_winsn;
		s->insn_read = comedi_readback_insn_read;

		if (devpriv->smp) {
			s->subdev_flags = devpriv->ao_spi->device_spi->ao_subdev_flags;
			s->do_cmdtest = daqbmc_ao_cmdtest;
			s->do_cmd = daqbmc_ao_cmd;
			s->cancel = daqbmc_ao_cancel;
		} else {
			s->subdev_flags = devpriv->ao_spi->device_spi->ao_subdev_flags - SDF_CMD_WRITE;
		}
		dev->write_subdev = s;

		ret = comedi_alloc_subdev_readback(s);
		if (ret) {
			dev_err(dev->class_dev,
				"alloc AO subdevice readback failed!\n");
			goto daqbmc_kfree_rx_exit;
		}
	}

	/*
	 * Probe the BMCboard existence and for configuration data
	 */
	ret = daqbmc_bmc_get_config(dev);

	if (ret == 0xff) { // no SPI comms with daq_bmc board
		dev_err(dev->class_dev,
			"BMCBoard not detected 0X%X, unloading driver \n", ret);
		ret = -EINVAL;
		goto daqbmc_kfree_rx_exit;
	}

	dev_info(dev->class_dev,
		"Analog Out channels %d, Analog In channels %d : Q84 config code 0x%x\n",
		thisboard->n_aochan, devpriv->ai_spi->chan, ret);

	/*
	 * check Q84 board returned config codes for digital channels
	 */
	if (ret != 0) { // sub-device codes from the Q84
		di_conf = false;
		do_conf = false;
		daqbmc_conf = PICSL12_AO;
		dev->n_subdevices = daqbmc_devices[daqbmc_conf].n_subdev; // only show the analog devices
		dev_info(dev->class_dev,
			"BMCBoard Digital DI DO Disabled, controller device %s \n", daqbmc_devices[daqbmc_conf].name);
	}

	if (do_conf) { // add the extra sub-devices
		s = &dev->subdevices[SUBDEV_MEM];
		s->type = COMEDI_SUBD_MEMORY;
		s->subdev_flags = SDF_READABLE | SDF_WRITABLE;
		s->n_chan = MEM_BLOCKS;
		s->maxdata = 0xffff;
		s->insn_write = daqbmc_sio_insn_write;
		s->insn_read = daqbmc_sio_insn_read;
		dev_info(dev->class_dev,
			"DISPLAY, RS232/TTL and device serial TX/RX channels %d\n", MEM_BLOCKS);

		s = &dev->subdevices[SUBDEV_DO];
		s->type = COMEDI_SUBD_DO;
		s->subdev_flags = SDF_WRITABLE;
		s->n_chan = num_do_chan;
		s->len_chanlist = num_do_chan;
		s->range_table = &range_digital;
		s->maxdata = 1;
		s->insn_bits = daqbmc_do_insn_bits;
		s->io_bits = 0x00ffffff;
		dev_info(dev->class_dev,
			"Digital Out channels %d\n", num_do_chan);
	}

	if (di_conf) {
		s = &dev->subdevices[2];
		s->type = COMEDI_SUBD_DI;
		s->subdev_flags = SDF_READABLE;
		s->n_chan = num_di_chan;
		s->len_chanlist = num_di_chan;
		s->range_table = &range_digital;
		s->maxdata = 1;
		s->insn_bits = daqbmc_di_insn_bits;
		dev_info(dev->class_dev,
			"Digital In channels %d\n", num_di_chan);
	}

	/*
	 * setup the timer to call my_timer_ai_callback
	 */
	timer_setup(&devpriv->ai_spi->my_timer, my_timer_ai_callback, 0);

	/*
	 * setup kthreads on other cores if possible
	 */
	if (devpriv->smp) {
		ret = daqbmc_create_thread(dev, devpriv);
		if (ret) {
			dev_err(dev->class_dev, "cpu thread creation failed\n");
			goto daqbmc_kfree_rx_exit;
		}
	}
	return 0; // complete without errors

	/*
	 * memory cleanups
	 */
daqbmc_kfree_rx_exit:
	kfree(pdata->rx_buff);
daqbmc_kfree_tx_exit:
	kfree(pdata->tx_buff);
daqbmc_kfree_exit:
	return ret;
}

static void daqbmc_detach(struct comedi_device * dev)
{
	struct daqbmc_private *devpriv = dev->private;

	dev->n_subdevices = SUBDEV; // set to correct number of sub-devices allocated
	/* wakeup and kill the threads */
	if (devpriv->smp) {
		if (devpriv->ao_spi->daqbmc_task) {
			set_bit(AO_CMD_RUNNING, &devpriv->state_bits);
			wake_up_interruptible(&daqbmc_ao_thread_wq);
			kthread_stop(devpriv->ao_spi->daqbmc_task);
		}
		devpriv->ao_spi->daqbmc_task = NULL;
	}

	del_timer_sync(&devpriv->ai_spi->my_timer);
	dev_info(dev->class_dev, "daq_bmc detached\n");
}

static struct comedi_driver daqbmc_driver = {
	.driver_name = "daq_bmc",
	.module = THIS_MODULE,
	.auto_attach = daqbmc_auto_attach,
	.detach = daqbmc_detach,
	.board_name = &daqbmc_boards[BMCBOARD].name,
	.num_names = ARRAY_SIZE(daqbmc_boards),
	.offset = sizeof(struct daqbmc_board),
};

/*
 * called for each listed spibmc SPI device
 * SO THIS RUNS FIRST, setup basic spi comm parameters here
 */
static int32_t spibmc_spi_probe(struct spi_device * spi)
{
	struct comedi_spibmc *pdata;
	int32_t ret;

	pdata = kzalloc(sizeof(struct comedi_spibmc), GFP_KERNEL | GFP_DMA);
	if (!pdata)
		return -ENOMEM;

	/*
	 * default SPI delays of zero
	 */
	pdata->delay = CS_CHANGE_DELAY_USECS0;
	pdata->cs_delay = CS_CHANGE_DELAY_USECS0;
	pdata->word_delay = CS_CHANGE_DELAY_USECS0;

	spi->dev.platform_data = pdata;
	reinit_completion(&done);
	pdata->ping_pong = false;
	pdata->upper_lower = 0;
	pdata->tx_buff = kzalloc(SPI_BUFF_SIZE_NOHUNK, GFP_KERNEL);
	if (!pdata->tx_buff) {
		ret = -ENOMEM;
		goto kfree_exit;
	}
	pdata->rx_buff = kzalloc(SPI_BUFF_SIZE_NOHUNK, GFP_KERNEL);
	if (!pdata->rx_buff) {
		ret = -ENOMEM;
		goto kfree_tx_exit;
	}

	dev_info(&spi->dev, "spi link %s\n", spibmc_version);
	/*
	 * Do only one chip select for the BMCboard
	 */
	dev_info(&spi->dev,
		"BMCboard default: do_conf=%d, di_conf=%d, daqbmc_conf=%d\n",
		do_conf, di_conf, daqbmc_conf);

	if (spi->chip_select == CSnA) {
		/*
		 * get a copy of the slave device 0 to share with Comedi
		 * we need a device to talk to the Q84
		 *
		 * create entry into the Comedi device list
		 */
		INIT_LIST_HEAD(&pdata->device_entry);
		pdata->slave.spi = spi;
		/*
		 * put entry into the Comedi device list
		 */
		list_add_tail(&pdata->device_entry, &device_list);
		spi->mode = daqbmc_devices[daqbmc_conf].spi_mode;
		spi->max_speed_hz = daqbmc_devices[daqbmc_conf].max_speed_hz;
		spi->word_delay = CS_CHANGE_DELAY_USECS0;
		spi->cs_setup = CS_CHANGE_DELAY_USECS0;
		spi->cs_hold = CS_CHANGE_DELAY_USECS0;
		spi->cs_inactive = CS_CHANGE_DELAY_USECS0;
	}

	spi->bits_per_word = daqbmc_devices[daqbmc_conf].spi_bpw;
	spi_setup(spi);

	/* setup Comedi part of driver */
	if (spi->chip_select == CSnA) {
		ret = comedi_driver_register(&daqbmc_driver);
		if (ret < 0) {
			goto kfree_rx_exit;
		}

		if (bmc_autoload) {
			ret = comedi_auto_config(&spi->master->dev,
				&daqbmc_driver, 0);
		}

		if (ret < 0) {
			goto kfree_rx_exit;
		}
	}
	return 0;

kfree_rx_exit:
	kfree(pdata->rx_buff);
kfree_tx_exit:
	kfree(pdata->tx_buff);
kfree_exit:
	kfree(pdata);
	return ret;
}

static int spibmc_spi_remove(struct spi_device * spi)
{
	struct comedi_spibmc *pdata = spi->dev.platform_data;

	dev_info(&spi->dev, "spibmc device link removed \n");
	if (!list_empty(&device_list)) {
		list_del(&pdata->device_entry);
	}

	if (pdata->rx_buff) {
		kfree(pdata->rx_buff);
	}
	if (pdata->tx_buff) {
		kfree(pdata->tx_buff);
	}

	if (pdata) {
		kfree(pdata);
	}
	dev_info(&spi->dev, "spibmc driver released\n");
	return 0;
}

static const struct spi_device_id spibmc_spi_ids[] = {
	{ .name = "spi-bmc"},
	{ .name = "dh2228fv"},
	{ .name = "ltc2488"},
	{ .name = "sx1301"},
	{ .name = "bk4"},
	{ .name = "dhcom-board"},
	{ .name = "m53cpld"},
	{ .name = "spi-petra"},
	{ .name = "spi-authenta"},
	{},
};
MODULE_DEVICE_TABLE(spi, spibmc_spi_ids);

/*
 * spibmc should never be referenced in DT without a specific compatible string,
 * it is a Linux implementation thing rather than a description of the hardware.
 */
static int spibmc_of_check(struct device *dev)
{
	if (device_property_match_string(dev, "compatible", "spi-bmc") < 0) {
		return 0;
	}

	dev_err(dev, "spibmc listed directly in DT is not supported\n");
	dev_info(dev, "Use a compatible alias string like spi-bmc in DT\n");
	return -EINVAL;
}

static const struct of_device_id spibmc_dt_ids[] = {
	{ .compatible = "orangepi,spi-bmc", .data = &spibmc_of_check},
	{ .compatible = "rohm,dh2228fv", .data = &spibmc_of_check},
	{ .compatible = "lineartechnology,ltc2488", .data = &spibmc_of_check},
	{ .compatible = "semtech,sx1301", .data = &spibmc_of_check},
	{ .compatible = "lwn,bk4", .data = &spibmc_of_check},
	{ .compatible = "dh,dhcom-board", .data = &spibmc_of_check},
	{ .compatible = "menlo,m53cpld", .data = &spibmc_of_check},
	{ .compatible = "cisco,spi-petra", .data = &spibmc_of_check},
	{ .compatible = "micron,spi-authenta", .data = &spibmc_of_check},
	{},
};
MODULE_DEVICE_TABLE(of, spibmc_dt_ids);

/* Dummy SPI devices not to be used in production systems */
static int spibmc_acpi_check(struct device *dev)
{
	dev_warn(dev, "do not use this driver in production systems!\n");
	return 0;
}

static const struct acpi_device_id spibmc_acpi_ids[] = {
	/*
	 * The ACPI SPT000* devices are only meant for development and
	 * testing. Systems used in production should have a proper ACPI
	 * description of the connected peripheral and they should also use
	 * a proper driver instead of poking directly to the SPI bus.
	 */
	{ "SPT0001", (kernel_ulong_t) & spibmc_acpi_check},
	{ "SPT0002", (kernel_ulong_t) & spibmc_acpi_check},
	{ "SPT0003", (kernel_ulong_t) & spibmc_acpi_check},
	{},
};
MODULE_DEVICE_TABLE(acpi, spibmc_acpi_ids);

static struct spi_driver spibmc_spi_driver = {
	.driver =
	{
		.name = "spibmc",
		.owner = THIS_MODULE,
		.of_match_table = spibmc_dt_ids,
	},
	.probe = spibmc_spi_probe,
	.remove = (void *) spibmc_spi_remove,
	.id_table = spibmc_spi_ids,
};

/*
 * use table data to setup the SPI hardware for the selected board
 */
static int32_t daqbmc_spi_setup(struct spi_param_type * spi)
{
	spi->spi->max_speed_hz = spi->device_spi->max_speed_hz;
	spi->spi->mode = spi->device_spi->spi_mode;
	return spi_setup(spi->spi);
}

/*
 * setup driver defaults using config variables and modules conf files
 */
static int32_t daqbmc_spi_probe(struct comedi_device * dev,
	struct spi_param_type * spi_adc)
{
	const struct daqbmc_board *thisboard = dev->board_ptr;

	if (!spi_adc->spi) {
		dev_err(dev->class_dev, "no ADC spi channel detected\n");
		spi_adc->chan = 0;
		return 0;
	}

	switch (daqbmc_conf) {
	case 0:
		spi_adc->device_type = PICSL12;
		break;
	case 1:
		spi_adc->device_type = PICSL12_AO;
		break;
	default:
		spi_adc->device_type = PICSL12_AO;
	}
	spi_adc->device_spi = &daqbmc_devices[spi_adc->device_type];

	/* default setup */
	spi_adc->pic18 = CONF_Q84; /* Q84 mode 12 bits */
	spi_adc->chan = thisboard->n_aichan;

	/*
	 * SPI link data hardware setup
	 */
	daqbmc_spi_setup(spi_adc);
	dev_info(dev->class_dev, "PIC18F57Q84 DAQ device, probing SPI slave mode\n");
	daqbmc_spi_setup(spi_adc);
	spi_adc->device_type = daqbmc_conf;
	spi_adc->chan = spi_adc->device_spi->n_chan;
	spi_adc->range = 0; // 4.096 default
	spi_adc->bits = spi_adc->device_spi->n_chan_bits;

	if ((daqbmc_conf == PICSL12_AO) || (do_conf == 0) || (di_conf == 0)) { // no DI DO board version
		do_conf = 0;
		di_conf = 0;
	}

	dev_info(dev->class_dev,
		"BMCboard SPI setup: spi cs %d: %d Hz: mode 0x%x: "
		"probing for controller device %s\n",
		spi_adc->spi->chip_select,
		spi_adc->spi->max_speed_hz,
		spi_adc->spi->mode,
		spi_adc->device_spi->name);

	return spi_adc->chan;
}

/*
 * Linux Module setups
 */
static int32_t __init daqbmc_init(void)
{
	return spi_register_driver(&spibmc_spi_driver);
}
module_init(daqbmc_init);

static void __exit daqbmc_exit(void)
{
	struct comedi_spibmc *pdata;
	static struct spi_param_type *slave_spi;

	/*
	 * find the needed spi device for module shutdown
	 */
	list_for_each_entry(pdata, &device_list, device_entry)
	{
		slave_spi = &pdata->slave;
	}

	comedi_auto_unconfig(&slave_spi->spi->master->dev);
	comedi_driver_unregister(&daqbmc_driver);
	spi_unregister_driver(&spibmc_spi_driver);
}
module_exit(daqbmc_exit);

MODULE_AUTHOR("NSASPOOK <nsaspooksma2@gmail.com");
MODULE_DESCRIPTION("OPi DI/DO/AI/AO SPI Driver");
MODULE_VERSION("6.1.31");
MODULE_LICENSE("GPL");
MODULE_ALIAS("spi:spibmc");

