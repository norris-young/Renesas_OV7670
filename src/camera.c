/*
 * camera.c
 *
 *  Created on: 2017年8月3日
 *      Author: Cotyledon
 */

#include "platform.h"
#include "r_cg_icu.h"
#include "r_cg_sci.h"

#include "OV7670.h"
#include "OV7670cfg.h"
#include "soft_SCCB.h"
#include "camera.h"

/* private variables. */
static bool first_VSYNC = true;
static bool start_get_image = false;
static bool new_frame_FIFO_ready = false;

static uint8_t img[CAM_WIDTH * CAM_HEIGHT];
static const int collect_w_step = OV_WIDTH / CAM_WIDTH;
static const int collect_h_step = OV_HEIGHT / CAM_HEIGHT;

static const int test_step = 1;

/* public variables. */
volatile bool send_end = true;
unsigned char start_c[2] = {0x01, 0xfe};
unsigned char end_c[2] = {0xfe, 0x01};

/* private functions declarations. */
static void reset_read_FIFO(void);
static void get_data(int i, int j);
static void get_none(void);
static void cam_test(void);

/* public functions definitions. */
int cam_init(void)
{
    uint8_t device_id;

    /* restore. */
    if (0 == softSCCB_WriteReg(0x12, 0x80)) return 1;
    R_BSP_SoftwareDelay(50, BSP_DELAY_MILLISECS);

    /* read product ID. */
    softSCCB_ReadReg(0x0b, &device_id, 1);
    if (device_id != OV7670_ID_1) return 2;
    softSCCB_ReadReg(0x0a, &device_id, 1);
    if (device_id != OV7670_ID_2) return 2;

    /* initialize. */
    for (int i = 0; i < sizeof(ov7670_init_reg_tbl)/sizeof(ov7670_init_reg_tbl[0]); i++) {
        if (0 == softSCCB_WriteReg(ov7670_init_reg_tbl[i][0], ov7670_init_reg_tbl[i][1]))
            return 3;
    }

    /* start get image. */
    start_get_image = true;
    R_SCI5_Start();
    R_ICU_IRQ5_Start();
    return 0;
}

int read_img_from_FIFO(void)
{
    if (!new_frame_FIFO_ready) {
        return 0;
    } else {
        OE = SET_BIT_LOW;
        reset_read_FIFO();
#if CAM_TEST
        cam_test();
#else
        for (int j = 0; j < OV_WIDTH; j++) {
            for (int i = OV_HEIGHT - 1; i >= 0; i--) {
                if ((i % collect_h_step) == 0 && (j % collect_w_step) == 0) {
                    get_data(i, j);
                } else {
                    get_none();
                }
            }
        }
#endif
        OE = SET_BIT_HIGH;

        /* read over, start next image. */
        new_frame_FIFO_ready = false;
        first_VSYNC = true;
        start_get_image = true;
        return 1;
    }
}

void get_img(unsigned char **image, int *width, int *height)
{
    *image  = img;
    *width  = CAM_WIDTH;
    *height = CAM_HEIGHT;
}

void std_tx_callback(void)
{
    send_end = true;
}

void IRQ5_IntHandler(void)
{
    if (first_VSYNC && start_get_image) {
        WRST = SET_BIT_LOW;
        WRST = SET_BIT_HIGH;
        WEN = SET_BIT_HIGH;
        first_VSYNC = false;
        start_get_image = false;
    } else {
        WEN = SET_BIT_LOW;
        new_frame_FIFO_ready = true;
    }
}

/* private functions definitions. */
static void reset_read_FIFO(void)
{
    RRST = SET_BIT_LOW;
    RCK_L;
    RCK_H;
    RCK_L;
    RRST = SET_BIT_HIGH;
    RCK_H;
}

static void get_data(int i, int j)
{
    RCK_L;
//    img[(i/collect_h_step * CAM_WIDTH + j/collect_w_step) * 2 + 1] = FIFO_DATA;
    img[i/collect_h_step * CAM_WIDTH + j/collect_w_step] = FIFO_DATA;
    RCK_H;
    RCK_L;
//    img[(i/collect_h_step * CAM_WIDTH + j/collect_w_step) * 2] = FIFO_DATA;

    RCK_H;
}

static void get_none(void)
{
    RCK_L;
    RCK_H;
    RCK_L;
    RCK_H;
}

static void cam_test(void)
{
    uint16_t tx_buf;
    while(!send_end);
    send_end = false;
    R_SCI5_Serial_Send(start_c, sizeof(start_c));
    for (int i = 0; i < OV_WIDTH * OV_HEIGHT; i++) {
        RCK_L;
        tx_buf = FIFO_DATA;
        RCK_H;
        tx_buf <<= 8;
        RCK_L;
        tx_buf |= FIFO_DATA;
        RCK_H;
        if (((i / OV_WIDTH) % test_step) == 0 && ((i % OV_WIDTH) % test_step) == 0) {
            while(!send_end);
            send_end = false;
            R_SCI5_Serial_Send((void *)&tx_buf, sizeof(tx_buf));
        }
    }
    while(!send_end);
    send_end = false;
    R_SCI5_Serial_Send(end_c, sizeof(end_c));
}
