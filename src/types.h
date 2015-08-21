
/*
 * types.h
 *
 *  Created on: 15 maj 2014
 *      Author: andreas
 */

#ifndef TYPES_H_
#define TYPES_H_

#include <limits.h>

typedef unsigned char tB;

typedef enum
{
    U08_E,
    U16_E,
    U32_E
} tType_E;

#define  FALSE      0
#define  TRUE       !FALSE

#define ON  TRUE
#define OFF FALSE

typedef unsigned char tU08;
#define MAX_U08 UCHAR_MAX
typedef char tS08;

typedef unsigned int tU16;
#define MAX_U16 UINT_MAX
typedef int tS16;

typedef unsigned long tU32;

#define HI_BYTE(w) (((w) >> 8) & 0x00FF)
#define LO_BYTE(w) ((w) & 0x00FF)

#define BIT_IS_SET(v, b) ((v >> b) & 0x1)

#define BYTES_IN_BYTE 1
#define BYTES_IN_INT  2
#define BYTES_IN_LONG 4

#define ABS(a)     (((a) < 0) ? -(a) : (a))
#define INC_U08(a) ((a) == MAX_U08 ? MAX_U08 : (a)++)
#define INC_U16(a) ((a) == MAX_U16 ? MAX_U16 : (a)++)

#define COUNT(a) (sizeof(a)/(sizeof(a[0])))

#endif /* TYPES_H_ */
