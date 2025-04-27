/*
 * the main secs/gem protocol functions
 */
#include "gemsecs.h"

/*
 * Checksum for message and header block after length byte
 */
uint16_t block_checksum(uint8_t *byte_block, const uint16_t byte_count) {
    uint16_t sum = 0, i;

    for (i = 0; i < byte_count; i++) {
        sum += byte_block[i];
    }


    return sum;
}

/*
 * Checksum for data stream
 */
uint16_t run_checksum(const uint8_t byte_block, const bool clear) {
    static uint16_t sum = 0;

    if (clear)
        sum = 0;

    sum += byte_block;
    return sum;
}

/*
 * logger helper
 */
static void ee_logger(const uint8_t stream, const uint8_t function, const uint16_t dtime, uint8_t *msg_data) {
    uint16_t i = 0;

    do {
        DATAEE_WriteByte(i, msg_data[254 + 2 - i]);
    } while (++i <= 255);

    StartTimer(TMR_INFO, dtime);
}