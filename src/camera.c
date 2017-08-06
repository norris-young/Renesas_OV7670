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
static int  vsync_num = 0;

#if RGB565

static uint8_t img[CAM_WIDTH * CAM_HEIGHT * 2];

#endif

#if YUV_UYVY

static uint8_t img[CAM_WIDTH * CAM_HEIGHT];

#endif

#if SHOW_FPS

static unsigned int frame_num = 0;

#endif

#if !CAM_TEST

static const int collect_w_step = OV_WIDTH / CAM_WIDTH;
static const int collect_h_step = OV_HEIGHT / CAM_HEIGHT;
static int collect_w_step_index = 0;
static int collect_h_step_index = 0;

#endif

/* public variables. */
volatile bool send_end = true;
unsigned char start_c[2] = {0x01, 0xfe};
unsigned char end_c[2] = {0xfe, 0x01};

/* private functions declarations. */
static void reset_read_FIFO(void);

#if CAM_TEST

static void cam_test(void);

#else

static void get_data(int i, int j);
static void get_none(void);

#endif

#if SEND_IMG

static void send_img(void);

#endif

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
    R_SCI5_Start();
    R_ICU_IRQ5_Start();
    return 0;
}

int read_img_from_FIFO(void)
{
    if (vsync_num > 0) {
        OE = SET_BIT_LOW;
        reset_read_FIFO();
#if CAM_TEST
        cam_test();
#else
        for (int j = 0; j < OV_WIDTH; j++) {
            for (int i = OV_HEIGHT - 1; i >= 0; i--) {
                if (collect_w_step_index == 0 && collect_h_step_index == 0) {
                    get_data(i, j);
                } else {
                    get_none();
                }
                collect_h_step_index++;
                if (collect_h_step_index == collect_h_step) collect_h_step_index = 0;
            }
            collect_w_step_index++;
            if (collect_w_step_index == collect_w_step) collect_w_step_index = 0;
        }
    #if SEND_IMG
            send_img();
    #endif
#endif
        OE = SET_BIT_HIGH;
#if SHOW_FPS
        frame_num++;
#endif
        /* read over, start next image. */
        vsync_num = 0;
        return 1;
    }
    return 0;
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
    if(vsync_num == 0) {
        WRST = SET_BIT_LOW;
        WRST = SET_BIT_HIGH;
        WEN = SET_BIT_HIGH;
        if (first_VSYNC) {
            first_VSYNC = false;
        } else {
            vsync_num++;
        }
    } else {
        WEN = SET_BIT_LOW;
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


#if CAM_TEST
    #if RGB565

    static void cam_test(void)
    {
        uint16_t tx_buf;
        while(!send_end);
        send_end = false;
        R_SCI5_Serial_Send(start_c, sizeof(start_c));
        for (int i = 0; i < OV_WIDTH * OV_HEIGHT; i++) {
            while(!send_end);
            RCK_L;
            tx_buf = FIFO_DATA;
            RCK_H;
            tx_buf <<= 8;
            RCK_L;
            tx_buf |= FIFO_DATA;
            RCK_H;
            send_end = false;
            R_SCI5_Serial_Send((uint8_t *)&tx_buf, sizeof(tx_buf));
        }
        while(!send_end);
        send_end = false;
        R_SCI5_Serial_Send(end_c, sizeof(end_c));
    }

    #endif /* endif RGB565 */

    #if YUV_UYVY /* only send Y signal. */

    static void cam_test(void)
    {
        uint8_t tx_buf;
        while(!send_end);
        send_end = false;
        R_SCI5_Serial_Send(start_c, sizeof(start_c));
        for (int i = 0; i < OV_WIDTH * OV_HEIGHT; i++) {
            while(!send_end);
            RCK_L;
            RCK_H;
            RCK_L;
            tx_buf = FIFO_DATA;
            RCK_H;
            send_end = false;
            R_SCI5_Serial_Send((void *)&tx_buf, sizeof(tx_buf));
        }
        while(!send_end);
        send_end = false;
        R_SCI5_Serial_Send(end_c, sizeof(end_c));
    }

    #endif
#else
    static void get_data(int i, int j)
    {
    #if RGB565
        RCK_L;
        img[(i/collect_h_step * CAM_WIDTH + j/collect_w_step) * 2 + 1] = FIFO_DATA;
        RCK_H;
        RCK_L;
        img[(i/collect_h_step * CAM_WIDTH + j/collect_w_step) * 2] = FIFO_DATA;
        RCK_H;
    #endif

    #if YUV_UYVY
        RCK_L;
        RCK_H;
        RCK_L;
        img[i/collect_h_step * CAM_WIDTH + j/collect_w_step] = FIFO_DATA;
        RCK_H;
    #endif
    }

    static void get_none(void)
    {
        RCK_L;
        RCK_H;
        RCK_L;
        RCK_H;
    }
#endif

#if SEND_IMG
    #if RGB565

    static void send_img(void)
    {
        while(!send_end);
        send_end = false;
        R_SCI5_Serial_Send(start_c, sizeof(start_c));
        while(!send_end);
        send_end = false;
        R_SCI5_Serial_Send(img, CAM_WIDTH * CAM_HEIGHT * 2);
        while(!send_end);
        send_end = false;
        R_SCI5_Serial_Send(end_c, sizeof(end_c));
    }

    #endif

    #if YUV_UYVY

    static void send_img(void)
    {
        while(!send_end);
        send_end = false;
        R_SCI5_Serial_Send(start_c, sizeof(start_c));
        while(!send_end);
        send_end = false;
        R_SCI5_Serial_Send(img, CAM_WIDTH * CAM_HEIGHT);
        while(!send_end);
        send_end = false;
        R_SCI5_Serial_Send(end_c, sizeof(end_c));
    }

    #endif
#endif
