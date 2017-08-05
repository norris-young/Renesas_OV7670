/*
 * camera.h
 *
 *  Created on: 2017年8月3日
 *      Author: Cotyledon
 */

#ifndef CAMERA_H_
#define CAMERA_H_

#define CAM_WIDTH   40
#define CAM_HEIGHT  80

#define RCK_L       RCK = SET_BIT_LOW
#define RCK_H       RCK = SET_BIT_HIGH
#define FIFO_DATA   (D7<<7|D6<<6|D5<<5|D4<<4|D3<<3|D2<<2|D1<<1|D0)

extern int cam_init(void);
extern int read_img_from_FIFO(void);
extern void get_img(unsigned char **image, int *width, int *height);

#endif /* CAMERA_H_ */
