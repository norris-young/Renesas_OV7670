/*
 * cam_commu.h
 *
 *  Created on: 2017年8月5日
 *      Author: Cotyledon
 */

#ifndef CAM_COMMU_H_
#define CAM_COMMU_H_

#define CAM_COMMU_STX       0xFE
#define BLACK_DEFINE        0x5A

extern void cam_commu_init(void);
extern void send_mid(unsigned char *img, int width, int height);


#endif /* CAM_COMMU_H_ */
