/*
 * soft_SCCB.c
 *
 *  Created on: 2017年8月3日
 *      Author: Cotyledon
 */

#include <platform.h>

#include "soft_SCCB.h"

static uint8_t softSCCB_Start(void);
static void softSCCB_Stop(void);
static void softSCCB_Ack(void);
static void softSCCB_NoAck(void);
static uint8_t softSCCB_WaitAck(void);
static void softSCCB_SendByte(uint8_t);
static uint8_t softSCCB_ReceiveByte(void);
static void softSCCB_Delay(uint32_t);

/*
 * softSCCB_WriteReg
 * 写SCCB设备寄存器
 *
 * 参数:
 *    reg_addr--寄存器地址
 *    data--待写数据
 *
 * 输出:
 *    1-成功
 *    0-失败
 */
uint8_t softSCCB_WriteReg(uint8_t reg_addr , uint8_t data)
{
  if(!softSCCB_Start())
  {
    return 0;
  }
  softSCCB_SendByte( SCCB_DEV_ADR );
  if( !softSCCB_WaitAck() )
  {
    softSCCB_Stop();
    return 0;
  }
  softSCCB_SendByte(reg_addr);
  softSCCB_WaitAck();
  softSCCB_SendByte(data);
  softSCCB_WaitAck();
  softSCCB_Stop();
  return 1;
}




/******************************************************************************************************************
* 函数名：SCCB_ReadByte
* 描述  ：读取一串数据
* 输入  ：- data: 存放读出数据   - length: 待读出长度 - reg_addr: 待读出地址        - DeviceAddress: 器件类型
* 输出  ：返回为:=1成功读入,=0失败
* 注意  ：无
*******************************************************************************************************************/
/*
 * softSCCB_ReadReg
 * 读SCCB设备寄存器
 *
 * 参数:
 *    reg_addr--寄存器地址
 *    *data--待存读出数据地址
 *    length--读取长度
 *
 * 输出:
 *    1-成功
 *    0-失败
 */
uint8_t softSCCB_ReadReg(uint8_t reg_addr, uint8_t* data, uint8_t length)
{
  if(!softSCCB_Start())
  {
    return 0;
  }
  softSCCB_SendByte( SCCB_DEV_ADR );
  if( !softSCCB_WaitAck() )
  {
    softSCCB_Stop();
    return 0;
  }
  softSCCB_SendByte( reg_addr );
  softSCCB_WaitAck();
  softSCCB_Stop();

  if(!softSCCB_Start())
  {
    return 0;
  }
  softSCCB_SendByte( SCCB_DEV_ADR + 1 );
  if(!softSCCB_WaitAck())
  {
    softSCCB_Stop();
    return 0;
  }
  while(length)
  {
    *data = softSCCB_ReceiveByte();
    if(length == 1)
    {
      softSCCB_NoAck();
    }
    else
    {
      softSCCB_Ack();
    }
    data++;
    length--;
  }
  softSCCB_Stop();
  return 1;
}

/*
 * softSCCB_Start
 * SCCB起始信号，内部调用
 */
static uint8_t softSCCB_Start(void)
{
  SCCB_SDA_O(1);
  SCCB_SCL(1);
  SCCB_DELAY();

  SCCB_SDA_IN();
  if(!SCCB_SDA_I)
  {
    SCCB_SDA_OUT();
    return 0;
  }
  SCCB_SDA_OUT();
  SCCB_SDA_O(0);

  SCCB_DELAY();

  SCCB_SDA_IN();
  if(SCCB_SDA_I)
  {
    SCCB_SDA_OUT();
    return 0;
  }
  SCCB_SDA_OUT();
  SCCB_SDA_O(0);

  return 1;
}

/*
 * softSCCB_Stop
 * SCCB停止信号，内部调用
 */
static void softSCCB_Stop(void)
{
  SCCB_SCL(0);
  SCCB_SDA_O(0);
  SCCB_DELAY();

  SCCB_SCL(1);
  SCCB_SDA_O(1);
  SCCB_DELAY();
}

/*
 * softSCCB_Stop
 * SCCB应答信号，内部调用
 */
static void softSCCB_Ack(void)
{
  SCCB_SCL(0);
  SCCB_DELAY();

  SCCB_SDA_O(0);
  SCCB_DELAY();

  SCCB_SCL(1);
  SCCB_DELAY();

  SCCB_SCL(0);
  SCCB_DELAY();
}

/*
 * softSCCB_NoAck
 * SCCB无应答信号，内部调用
 */
static void softSCCB_NoAck(void)
{
  SCCB_SCL(0);
  SCCB_DELAY();
  SCCB_SDA_O(1);
  SCCB_DELAY();
  SCCB_SCL(1);
  SCCB_DELAY();
  SCCB_SCL(0);
  SCCB_DELAY();
}

/*
 * softSCCB_WaitAck
 * SCCB等待应答信号，内部调用
 */
static uint8_t softSCCB_WaitAck(void)
{
  SCCB_SCL(0);
  SCCB_DELAY();
  SCCB_SDA_O(1);
  SCCB_DELAY();

  SCCB_SCL(1);

  SCCB_SDA_IN();
  SCCB_DELAY();

  if(SCCB_SDA_I)
  {
    SCCB_SDA_OUT();
    SCCB_SCL(0);
    return 0;
  }
  SCCB_SDA_OUT();
  SCCB_SCL(0);
  return 1;
}

/*
 * softSCCB_SendByte
 * SCCB发送数据，内部调用
 */
static void softSCCB_SendByte(uint8_t data)
{
  uint8_t i=8;
  while(i--)
  {
    SCCB_SCL(0);
    SCCB_DELAY();
    if(data&0x80)
    {
      SCCB_SDA_O(1);
    }
    else
    {
      SCCB_SDA_O(0);
    }
    data<<=1;
    SCCB_DELAY();
    SCCB_SCL(1);
    SCCB_DELAY();
  }
  SCCB_SCL(0);
}

/*
 * softSCCB_SendByte
 * SCCB接收数据，内部调用
 */
static uint8_t softSCCB_ReceiveByte(void)
{
  uint8_t i=8;
  uint8_t ReceiveByte=0;

  SCCB_SDA_O(1);
  SCCB_DELAY();

  SCCB_SDA_IN();

  while(i--)
  {
    ReceiveByte<<=1;
    SCCB_SCL(0);
    SCCB_DELAY();

    SCCB_SCL(1);
    SCCB_DELAY();

    if(SCCB_SDA_I)
    {
      ReceiveByte|=0x01;
    }

  }
  SCCB_SDA_OUT();
  SCCB_SCL(0);

  return ReceiveByte;
}

/*
 * softSCCB_SendByte
 * SCCB延时函数，内部调用
 */
static void softSCCB_Delay(uint32_t delay)
{
    R_BSP_SoftwareDelay(delay, BSP_DELAY_MICROSECS);
}
