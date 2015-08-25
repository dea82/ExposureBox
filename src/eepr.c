/******************************************************************************
 * Copyright (C) 2014 Andreas, Sweden
 * <andreas.laghamn@gmail.com>
 * http://laghamn.dyndns.org
 *****************************************************************************
 *
 * File    : eepr.c
 * Date    : 10 maj 2014
 * 
 * Summary : TODO
 *
 *****************************************************************************/
#include "eepr.h"
#include "gpio.h"
#include "conf.h"
#include <avr/io.h>
#include "spii.h"
#include "seri.h"
#include <stdio.h>

static void chipSelect(csState_t state);
static tU08 readStatusRegister_U08(void);
static tB ready_B(void);
static tB write_B(tU16 address_U16, const void *data_ptr, tU08 bytes_U08);
static void read(tU08 *data_ptr, tU16 address_U16, tU08 bytes_U08);

/**
 * Initializes the EEPR module.
 */
void Eepr_init(void)
{
    /** Initialize CS pin for EEPROM memory */
    gpio_conf(EEPROM_CS_CFG, OUTPUT, SPII_CS_HIGH);
}

/**
 * Sets the Chip Select pin state.
 * @param state
 */
static void chipSelect(csState_t state)
{
    switch (state)
    {
    case SPII_CS_LOW:
        gpio_clr(EEPROM_CS_CFG);
        break;
    case SPII_CS_HIGH:
        gpio_set(EEPROM_CS_CFG);
        break;
    }
}

tB Eepr_write_B(tU08 *data_paU08, tU16 address_U16, tU08 size_U08)
{
    char buffer[20] = "";

    tB ret_B = write_B(address_U16, data_paU08, size_U08);

    for (tU08 i_U08 = 0; i_U08 < size_U08; i_U08++)
    {
        sprintf(buffer,"Data: %i\n", data_paU08[i_U08]);
        Seri_writeString(buffer);
        ret_B &= (data_paU08[i_U08] == Eepr_readByte_U08(address_U16 + i_U08));
        sprintf(buffer,"Read: %i\n", Eepr_readByte_U08(address_U16 + i_U08));
        Seri_writeString(buffer);
    }
    return ret_B;
}

/*! \brief Writes a single bit in the eeprom at the given address.
 *
 * @param address_U16
 * @param bit_U08
 * @param data_B
 * @return
 */
tB Eepr_writeBit_B(tU16 address_U16, tU08 bit_U08, tB data_B)
{
    tU08 data_U08 = Eepr_readByte_U08(address_U16);
    if (data_B == TRUE)
    {
        data_U08 |= (1 << bit_U08);
    }
    else
    {
        data_U08 &= ~(1 << bit_U08);
    }

    return Eepr_writeByte_B(address_U16, data_U08);
}

tB Eepr_writeByte_B(tU16 address_U16, tU08 data_U08)
{
    tB ret_B = FALSE;

    write_B(address_U16, &data_U08, BYTES_IN_BYTE);

    if (Eepr_readByte_U08(address_U16) == data_U08)
    {
        ret_B = TRUE;
    }
    return ret_B;
}

tB Eepr_writeInt_B(tU16 address_U16, tU16 data_U16)
{
    tB ret_B = FALSE;

    write_B(address_U16, &data_U16, BYTES_IN_INT);

    if (Eepr_readInt_U16(address_U16) == data_U16)
    {
        ret_B = TRUE;
    }
    return ret_B;
}

tB Eepr_writeLong_B(tU16 address_U16, tU32 data_U32)
{
    tB ret_B = FALSE;

    write_B(address_U16, &data_U32, BYTES_IN_LONG);

    if (Eepr_readLong_U32(address_U16) == data_U32)
    {
        ret_B = TRUE;
    }
    return ret_B;
}

static tB write_B(tU16 address_U16, const void *data_ptr, tU08 bytes_U08)
{
    tB ret_B = FALSE;

    /* Make sure the EEPROM is ready */
    while (ready_B() == FALSE)
        ;

    /* Enable write of EEPROM */
    if (Eepr_writeEnable_B() == TRUE)
    {
        chipSelect(SPII_CS_LOW);
        /* Write command */
        Spii_write(WRITE);
        /* Send address low/high nibble */
        Spii_write(HI_BYTE(address_U16));
        Spii_write(LO_BYTE(address_U16));
        /* Data to be written */
        while (bytes_U08--)
        {
            Spii_write(*(tU08*) (data_ptr++));
        }

        chipSelect(SPII_CS_HIGH);
        ret_B = TRUE;
    }
    else
    {
        /* Unable to enable write of EEPROM memory */
        ret_B = FALSE;
    }
    return ret_B;
}

void Eepr_read(void *data_ptr, const tU16 address_U16, tU08 bytes_U08)
{
    while (ready_B() == FALSE)
        ;

    chipSelect(SPII_CS_LOW);
    /* Send read command */
    Spii_write(READ);
    /* Send high and low nibble of address */
    Spii_write(HI_BYTE(address_U16));
    Spii_write(LO_BYTE(address_U16));
    /* Get the data at the address */
    while (bytes_U08--)
    {
        *(tU08*) data_ptr++ = Spii_getByte_U08();
    }
    chipSelect(SPII_CS_HIGH);
}

static void read(tU08 *data_ptr, const tU16 address_U16, tU08 bytes_U08)
{
    while (ready_B() == FALSE)
        ;
    chipSelect(SPII_CS_LOW);
    /* Send read command */
    Spii_write(READ);
    /* Send high and low nibble of address */
    Spii_write(HI_BYTE(address_U16));
    Spii_write(LO_BYTE(address_U16));
    /* Get the data at the address */
    while (bytes_U08--)
    {
        *data_ptr++ = Spii_getByte_U08();
    }

    chipSelect(SPII_CS_HIGH);
}

tU08 Eepr_readByte_U08(tU16 address_U16)
{
    tU08 data_U08;
    read(&data_U08, address_U16, BYTES_IN_BYTE);
    return data_U08;
}

tU16 Eepr_readInt_U16(tU16 address_U16)
{
    tU16 data_U16;
    read((tU08*) &data_U16, address_U16, BYTES_IN_INT);
    return data_U16;
}

tU32 Eepr_readLong_U32(tU16 address_U16)
{
    tU32 data_U32;
    read((tU08*) &data_U32, address_U16, BYTES_IN_LONG);
    return data_U32;
}

tB Eepr_writeEnable_B(void)
{
    chipSelect(SPII_CS_LOW);
    /* Enable write of EEPROM memory */
    Spii_write(WREN);
    chipSelect(SPII_CS_HIGH);
    /* Return write enable status */
    return (BIT_IS_SET(readStatusRegister_U08(), WEN));
}

tB Eepr_updateByte_B(tU16 address_U16, tU08 data_U08)
{
    tB ret_B = FALSE;

    if (Eepr_readByte_U08(address_U16) == data_U08)
    {
        ret_B = TRUE;
    }
    else
    {
        write_B(address_U16, &data_U08, BYTES_IN_BYTE);

        if (Eepr_readByte_U08(address_U16) == data_U08)
        {
            ret_B = TRUE;
        }
    }
    return ret_B;
}

tB Eepr_updateInt_B(tU16 address_U16, tU16 data_U16)
{
    tB ret_B = FALSE;

    if (Eepr_readInt_U16(address_U16) == data_U16)
    {
        ret_B = TRUE;
    }
    else
    {
        write_B(address_U16, &data_U16, BYTES_IN_INT);

        if (Eepr_readInt_U16(address_U16) == data_U16)
        {
            ret_B = TRUE;
        }
    }
    return ret_B;
}

tB Eepr_updateLong_B(tU16 address_U16, tU32 data_U32)
{
    tB ret_B = FALSE;

    if (Eepr_readLong_U32(address_U16) == data_U32)
    {
        ret_B = TRUE;
    }
    else
    {
        write_B(address_U16, &data_U32, BYTES_IN_LONG);

        if (Eepr_readLong_U32(address_U16) == data_U32)
        {
            ret_B = TRUE;
        }
    }
    return ret_B;
}

static tB ready_B(void)
{
    return (!(BIT_IS_SET(readStatusRegister_U08(), RDY_)));
}

static tU08 readStatusRegister_U08(void)
{
    tU08 statusRegister_U08;
    chipSelect(SPII_CS_LOW);
    /* Read status register */
    Spii_write(RDSR);
    /* Shift in response */
    statusRegister_U08 = Spii_getByte_U08();
    chipSelect(SPII_CS_HIGH);
    return statusRegister_U08;
}
