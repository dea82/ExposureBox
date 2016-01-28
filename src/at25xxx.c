/*
 * at25xxx.c
 *
 *  Created on: 22 okt 2015
 *      Author: andreas
 */

#include "at25xxx.h"

static uint8_t ready_U08(void);

uint8_t write_U08(uint8_t *source_U08, uint16_t address_U16, uint8_t bytes_U08)
{
    uint8_t ret_U08 = 1;

    if (address_U16 + bytes_U08 <= MAX_ADDRESS)
    {
        /* Write memory page (max 64 bytes) */
        while (bytes_U08 > 0)
        {
            /* TODO: Chip select */
            if (ready_U08)
            {

            }
            else
            {
                /* Failed to communicate or busy too long. */
                return 0;
            }
        }
    }
    else
    {
        /* Memory address out of range. */
        return 0;
    }

    return ret_U08;
}

static uint8_t ready_U08(void)
{
    uint8_t ret_U08 = 1;
    return ret_U08;
}

static uint8_t chipSelectH(uint8_t )
