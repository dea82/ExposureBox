
/*
 * types.h
 *
 *  Created on: 15 maj 2014
 *      Author: andreas
 */

#ifndef TYPES_H_
#define TYPES_H_

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

#define NULL 0

typedef unsigned char tU08;
typedef char tS08;

typedef unsigned int tU16;
typedef int tS16;

typedef unsigned long tU32;

#define HI_BYTE(w) (((w) >> 8) & 0x00FF)
#define LO_BYTE(w) ((w) & 0x00FF)

#define BIT_IS_SET(v, b) ((v >> b) & 0x1)

#define BYTES_IN_BYTE 1
#define BYTES_IN_INT  2
#define BYTES_IN_LONG 4

#define COUNT(a) (sizeof(a)/(sizeof(a[0])))

#endif /* TYPES_H_ */
