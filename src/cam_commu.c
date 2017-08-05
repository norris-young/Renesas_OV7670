/*
 * cam_commu.c
 *
 *  Created on: 2017年8月5日
 *      Author: Cotyledon
 */

#include "platform.h"
#include "r_cg_sci.h"

#include "camera.h"
#include "cam_commu.h"

/* public variables. */
static uint8_t mid[3] = {
                           CAM_COMMU_STX,
                           CAM_WIDTH / 2,
                           CAM_HEIGHT / 2,
                          };
static volatile bool commu_tx_end = true;

void cam_commu_init(void)
{
    R_SCI1_Start();
}

void send_mid(unsigned char *img, int width, int height)
{
    uint32_t tmp_x = 0, tmp_y = 0, black_cnt = 0;
    for (int j = 1; j < CAM_HEIGHT; j++)
        for (int i = 1; i < CAM_WIDTH; i++)
            if (img[j*CAM_WIDTH + i] < BLACK_DEFINE) {
                tmp_x += i;
                tmp_y += j;
                black_cnt++;
            }

    mid[1] = (uint8_t)(tmp_x / black_cnt);
    mid[2] = (uint8_t)(tmp_y / black_cnt);

    while(!commu_tx_end);
    commu_tx_end = false;
    R_SCI1_Serial_Send(mid, sizeof(mid));
}

void cam_commu_tx_callback(void)
{
    commu_tx_end = true;
}

