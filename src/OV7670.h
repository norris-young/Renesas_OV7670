/*
 * OV7670.H
 *
 *  Created on: 2017年8月3日
 *      Author: Cotyledon
 */

#ifndef OV7670_H_
#define OV7670_H_

#define OV_WIDTH   240
#define OV_HEIGHT  320

#define RCK     PORT3.PODR.BIT.B0
#define WEN     PORT2.PODR.BIT.B3
#define OE      PORTA.PODR.BIT.B3
#define RRST    PORT9.PODR.BIT.B4
#define WRST    PORT9.PODR.BIT.B2
#define VSYNC   PORTB.PIDR.BIT.B6

#define D7      PORTB.PIDR.BIT.B5
#define D6      PORT3.PIDR.BIT.B1
#define D5      PORTB.PIDR.BIT.B4
#define D4      PORT3.PIDR.BIT.B2
#define D3      PORTB.PIDR.BIT.B3
#define D2      PORT7.PIDR.BIT.B0
#define D1      PORTB.PIDR.BIT.B1
#define D0      PORT7.PIDR.BIT.B2

#define OV7670_ID_1     0x73
#define OV7670_ID_2     0x76

#endif /* OV7670_H_ */
