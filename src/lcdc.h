/*
 * lcdc.h
 *
 *  Created on: 13 maj 2014
 *      Author: andreas
 */

#ifndef LCDC_H_
#define LCDC_H_

#include "types.h"

#define LCDC_T 10

typedef enum {LCDC_CURSOR_HIDDEN, LCDC_CURSOR_VISIBLE, LCDC_CURSOR_BLINK} tCursorVisibility;
typedef enum {LCDC_LEFT, LCDC_CENTER, LCDC_RIGHT} tAlignment;
typedef enum {LCDC_LINE1, LCDC_LINE2} tLine;

void Lcdc_init(void);
void Lcdc_10ms(void);
void Lcdc_setBacklight(const tB status_B);
void Lcdc_clear(void);
void Lcdc_writeCharacter(const tU08 c_U08);
void Lcdc_writeString(const char *string_U08);
void Lcdc_moveCursor(const tU08 line_U08, const tU08 segment_U08);
void Lcdc_writeStringAligned(const char *string, const tLine line_E, const tAlignment alignment_E);
void Lcdc_writeStringAtPos(const char *string, const tU08 line_U08, const tU08 segment_U08);
void Lcdc_setCursorVisibility(const tCursorVisibility value_E);

#endif /* LCDC_H_ */
