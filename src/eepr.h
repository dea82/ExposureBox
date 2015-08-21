/*
 * eepr.h
 *
 *  Created on: 11 maj 2014
 *      Author: andreas
 */

#ifndef EEPR_H_
#define EEPR_H_

#include <avr/io.h>
#include "types.h"

#define AT25128_MEM_SIZE_BYTE 0x4000
#define AT25256_MEM_SIZE_BYTE 0x8000

/* Instructions for AT25128A_256A                              */
#define WREN  0x06 /* Write enable                             */
#define WRDI  0x04 /* Write disable                            */
#define RDSR  0x05 /* Read status register                     */
#define WRSR  0x01 /* Write status register                    */
#define READ  0x03 /* Read data from memory array              */
#define WRITE 0x02 /* Write Data from memory array             */
#define DUMMY 0xFF /* Dummy instruction for shifing in result. */
/* Status register                                             */
#define RDY_  0x0  /* Ready                                    */
#define WEN   0x1  /* Write enabled                            */
#define BP0   0x2  /* Block write protext bits                 */
#define BP1   0x3  /* Block write protext bits                 */
#define WPEN  0x7  /* Write protect enable                     */

void Eepr_init(void);
tB Eepr_writeEnable_B(void);
tB Eepr_write_B(tU08 *data_paU08, tU16 address_U16, tU08 size_U08);
tB Eepr_writeBit_B(tU16 address_U16, tU08 bit_U08, tB data_B);
tB Eepr_writeByte_B(tU16 address_U16, tU08 data_U08);
tB Eepr_writeInt_B(tU16 address_U16, tU16 data_U16);
tB Eepr_writeLong_B(tU16 address_U16, tU32 data_U32);

tU08 Eepr_readByte_U08(tU16 address_U16);
tU16 Eepr_readInt_U16(tU16 address_U16);
tU32 Eepr_readLong_U32(tU16 address_U16);
void Eepr_read(void *data_ptr, const tU16 address_U16, tU08 bytes_U08);

#endif /* EEPR_H_ */
