/*
 * soft_SCCB.h
 *
 *  Created on: 2017年8月3日
 *      Author: Cotyledon
 */

#ifndef TOOLS_SOFT_SCCB_H_
#define TOOLS_SOFT_SCCB_H_

#define SCCB_DEV_ADR    0x42

#define SCCB_SCL(x)         PORT7.PODR.BIT.B6 = (x)
#define SCCB_SDA_O(x)       PORT7.PODR.BIT.B4 = (x)
#define SCCB_SDA_I          PORT7.PIDR.BIT.B4

#define SCCB_SDA_OUT()      U_PORT_SDA_OUT()
#define SCCB_SDA_IN()       U_PORT_SDA_IN()

#define SCCB_DELAY()    softSCCB_Delay(25)      /* units:us */

extern uint8_t softSCCB_WriteReg(uint8_t, uint8_t);
extern uint8_t softSCCB_ReadReg(uint8_t, uint8_t*, uint8_t);

#endif /* TOOLS_SOFT_SCCB_H_ */
