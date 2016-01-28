/*
 * at25xxx.h
 *
 *  Created on: 22 okt 2015
 *      Author: andreas
 */

#ifndef AT25XXX_H_
#define AT25XXX_H_

#if(DEVICE == AT25256)
#define MAX_ADDRESS AT25256_MAX_ADDRESS
#elif(DEVICE == AT25128)
#define MAX_ADDRESS
#endif

#define AT25256_MAX_ADDRESS 32767
#define AT25128_MAX_ADDRESS 16383

#endif /* AT25XXX_H_ */
