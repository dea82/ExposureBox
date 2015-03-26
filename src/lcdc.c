/******************************************************************************
 * Copyright (C) 2014 Andreas, Sweden
 * <andreas.laghamn@gmail.com>
 * http://laghamn.dyndns.org
 *****************************************************************************
 *
 * File    : lcdc.c
 * Date    : 13 maj 2014
 * 
 * Summary : TODO
 *
 *****************************************************************************/
#include "conf.h"
#include "gpio.h"
#include "lcdc.h"
#include "types.h"
#include <util/delay.h>
#include <string.h>

#define LCD_POWER_UP_TIME_15_MS    15 /* [ms] */
#define LCD_POWER_UP_TIME_5_MS      5 /* [ms] */
#define LCD_POWER_UP_TIME_1_MS      1 /* [ms] */
#define INSTRUCTION_TIME_50_US     50 /* [us] */
#define INSTRUCTION_TIME_1800_US 1800 /* [us] */

#define LCDC_CMD_SCREEN_CLEAR 0x01
#define LCDC_CMD_HIDE_CURSOR 0x0C
#define LCDC_CMD_SHOW_CURSOR 0x0F

#define LCDC_MAX_LINE     1
#define LCDC_MAX_SEGMENT 19

#define HI_NIBBLE(b) (((b) >> 4) & 0x0F)
#define LO_NIBBLE(b) ((b) & 0x0F)

static void setupIO(void);
static void inititalize(void);
static void busyWait(void);
static void writeCommand(const tU08 command_U08);
static void writeNibble(const uint8_t nibble);
static void strobe(void);

void Lcdc_init(void)
{
    setupIO();
    inititalize();
}

void Lcdc_10ms(void)
{
}

static void setupIO(void)
{
    gpio_conf(LCD_BACKLIGHT_CFG, OUTPUT, 1);
    gpio_conf(LCD_RS_CFG, OUTPUT, 0);
    gpio_conf(LCD_RW_CFG, OUTPUT, 0);
    gpio_conf(LCD_ENABLE_CFG, OUTPUT, 0);
    gpio_conf(LCD_D_CFG, OUTPUT, 0);
}

static void inititalize(void)
{
    _delay_ms(LCD_POWER_UP_TIME_15_MS);
    writeNibble(0x3);
    _delay_ms(LCD_POWER_UP_TIME_5_MS);
    writeNibble(0x3);
    _delay_ms(LCD_POWER_UP_TIME_5_MS);
    writeNibble(0x3);
    _delay_ms(INSTRUCTION_TIME_50_US);
    writeNibble(0x2);
    _delay_ms(INSTRUCTION_TIME_50_US);
    /* From now on 4-bit mode */
    writeCommand(0x28);
    writeCommand(0x0C);
    writeCommand(0x06);
    Lcdc_clear();
}

static void busyWait(void)
{
    tB busyFlag_B;

    gpio_conf(LCD_D_CFG, INPUT, PULLUP);
    gpio_clr(LCD_RS_CFG);
    gpio_set(LCD_RW_CFG);
    do
    {
        _delay_us(1);
        gpio_set(LCD_ENABLE_CFG);
        _delay_us(1);
        busyFlag_B = gpio_tst(LCD_D7_CFG);
        gpio_clr(LCD_ENABLE_CFG);
        _delay_us(1);
        gpio_set(LCD_ENABLE_CFG);
        _delay_us(1);
        gpio_clr(LCD_ENABLE_CFG);

    } while (busyFlag_B);
    gpio_clr(LCD_RW_CFG);
    gpio_conf(LCD_D_CFG, OUTPUT, 0);
}

static void writeCommand(const tU08 command_U08)
{
    gpio_clr(LCD_RS_CFG);
    writeNibble(HI_NIBBLE(command_U08));
    writeNibble(LO_NIBBLE(command_U08));
    busyWait();
}

static void writeNibble(const tU08 nibble_U08)
{
    gpio_wr(PORT, LCD_D_CFG, nibble_U08);
    strobe();
}

static void strobe(void)
{
    gpio_set(LCD_ENABLE_CFG);
    _delay_us(1);
    gpio_clr(LCD_ENABLE_CFG);
    _delay_us(1);
}

void Lcdc_setBacklight(const tB value_B)
{
    if (value_B)
    {
        gpio_set(LCD_BACKLIGHT_CFG);
    }
    else
    {
        gpio_clr(LCD_BACKLIGHT_CFG);
    }
}

void Lcdc_setCursorVisibility(const tB value_B)
{
    if (value_B)
    {
        writeCommand(LCDC_CMD_SHOW_CURSOR);
    }
    else
    {
        writeCommand(LCDC_CMD_HIDE_CURSOR);
    }
}

void Lcdc_clear(void)
{
    writeCommand(LCDC_CMD_SCREEN_CLEAR);
}

void Lcdc_writeCharacter(const tU08 c_U08)
{
    gpio_set(LCD_RS_CFG);
    writeNibble(HI_NIBBLE(c_U08));
    writeNibble(LO_NIBBLE(c_U08));

}

void Lcdc_writeString(const char *string_U08)
{
    while (*string_U08)
    {
        Lcdc_writeCharacter(*string_U08++);
        _delay_us(40);
    }
}

void Lcdc_moveCursor(const tU08 line_U08, const tU08 segment_U08)
{
    tU08 cmd_U08;
    if (line_U08 < LCDC_MAX_LINE)
    {
        cmd_U08 = 0x80;
    }
    else
    {
        cmd_U08 = 0xC0;
    }
    cmd_U08 +=
            (segment_U08 > LCDC_MAX_SEGMENT) ? LCDC_MAX_SEGMENT : segment_U08;
    writeCommand(cmd_U08);
}

void Lcdc_writeStringAligned(const char *string, const tLine line_E,
        const tAlignment alignment_E)
{
    tU08 length_U08 = strlen(string);
    tU08 offsetPos_U08 = 0;
    tU08 line_U08 = 0;

    switch (alignment_E)
    {
    case LCDC_LEFT:
        offsetPos_U08 = 0;
        break;
    case LCDC_CENTER:
        offsetPos_U08 = ((LCDC_MAX_SEGMENT + 1) - length_U08) / 2;
        break;
    case LCDC_RIGHT:
        offsetPos_U08 = ((LCDC_MAX_SEGMENT + 1) - length_U08);
        break;
    default:
        offsetPos_U08 = 0;
        break;
    }

    switch (line_E)
    {
    case LCDC_LINE1:
        line_U08 = 0;
        break;
    case LCDC_LINE2:
        line_U08 = 1;
        break;
    default:
        line_U08 = 0;
        break;
    }

    Lcdc_writeStringAtPos(string, line_U08, offsetPos_U08);
}

void Lcdc_writeStringAtPos(const char *string, const tU08 line_U08,
        const tU08 segment_U08)
{
    Lcdc_moveCursor(line_U08, segment_U08);
    Lcdc_writeString(string);
}
