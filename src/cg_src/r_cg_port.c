/***********************************************************************************************************************
* DISCLAIMER
* This software is supplied by Renesas Electronics Corporation and is only intended for use with Renesas products.
* No other uses are authorized. This software is owned by Renesas Electronics Corporation and is protected under all
* applicable laws, including copyright laws. 
* THIS SOFTWARE IS PROVIDED "AS IS" AND RENESAS MAKES NO WARRANTIESREGARDING THIS SOFTWARE, WHETHER EXPRESS, IMPLIED
* OR STATUTORY, INCLUDING BUT NOT LIMITED TO WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
* NON-INFRINGEMENT.  ALL SUCH WARRANTIES ARE EXPRESSLY DISCLAIMED.TO THE MAXIMUM EXTENT PERMITTED NOT PROHIBITED BY
* LAW, NEITHER RENESAS ELECTRONICS CORPORATION NOR ANY OF ITS AFFILIATED COMPANIES SHALL BE LIABLE FOR ANY DIRECT,
* INDIRECT, SPECIAL, INCIDENTAL OR CONSEQUENTIAL DAMAGES FOR ANY REASON RELATED TO THIS SOFTWARE, EVEN IF RENESAS OR
* ITS AFFILIATES HAVE BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGES.
* Renesas reserves the right, without notice, to make changes to this software and to discontinue the availability 
* of this software. By using this software, you agree to the additional terms and conditions found by accessing the 
* following link:
* http://www.renesas.com/disclaimer
*
* Copyright (C) 2015, 2016 Renesas Electronics Corporation. All rights reserved.
***********************************************************************************************************************/

/***********************************************************************************************************************
* File Name    : r_cg_port.c
* Version      : Code Generator for RX23T V1.00.04.02 [29 Nov 2016]
* Device(s)    : R5F523T5AxFM
* Tool-Chain   : CCRX
* Description  : This file implements device driver for Port module.
* Creation Date: 17.8.5
***********************************************************************************************************************/

/***********************************************************************************************************************
Pragma directive
***********************************************************************************************************************/
/* Start user code for pragma. Do not edit comment generated here */
/* End user code. Do not edit comment generated here */

/***********************************************************************************************************************
Includes
***********************************************************************************************************************/
#include "r_cg_macrodriver.h"
#include "r_cg_port.h"
/* Start user code for include. Do not edit comment generated here */
/* End user code. Do not edit comment generated here */
#include "r_cg_userdefine.h"

/***********************************************************************************************************************
Global variables and functions
***********************************************************************************************************************/
/* Start user code for global. Do not edit comment generated here */
/* End user code. Do not edit comment generated here */

/***********************************************************************************************************************
* Function Name: R_PORT_Create
* Description  : This function initializes the Port I/O.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
void R_PORT_Create(void)
{
    PORT9.PODR.BYTE = _04_Pm2_OUTPUT_1 | _10_Pm4_OUTPUT_1;
    PORT3.PCR.BYTE = _02_Pm1_PULLUP_ON | _04_Pm2_PULLUP_ON;
    PORT7.PCR.BYTE = _01_Pm0_PULLUP_ON | _04_Pm2_PULLUP_ON;
    PORTB.PCR.BYTE = _02_Pm1_PULLUP_ON | _08_Pm3_PULLUP_ON | _10_Pm4_PULLUP_ON | _20_Pm5_PULLUP_ON;
    PORT2.DSCR.BYTE |= _00_Pm3_HIDRV_OFF;
    PORT3.DSCR.BYTE |= _00_Pm0_HIDRV_OFF | _00_Pm1_HIDRV_OFF | _00_Pm2_HIDRV_OFF;
    PORT7.DSCR.BYTE |= _00_Pm0_HIDRV_OFF | _00_Pm2_HIDRV_OFF | _10_Pm4_HIDRV_ON | _40_Pm6_HIDRV_ON;
    PORT9.DSCR.BYTE |= _00_Pm2_HIDRV_OFF | _00_Pm4_HIDRV_OFF;
    PORTA.DSCR.BYTE |= _00_Pm3_HIDRV_OFF;
    PORTB.DSCR.BYTE |= _00_Pm1_HIDRV_OFF | _00_Pm3_HIDRV_OFF | _00_Pm4_HIDRV_OFF | _00_Pm5_HIDRV_OFF;
    PORT2.PDR.BYTE = _08_Pm3_MODE_OUTPUT;
    PORT3.PDR.BYTE = _01_Pm0_MODE_OUTPUT | _00_Pm1_MODE_INPUT | _00_Pm2_MODE_INPUT;
    PORT7.PDR.BYTE = _00_Pm0_MODE_INPUT | _00_Pm2_MODE_INPUT | _10_Pm4_MODE_OUTPUT | _40_Pm6_MODE_OUTPUT;
    PORT9.PDR.BYTE = _04_Pm2_MODE_OUTPUT | _10_Pm4_MODE_OUTPUT;
    PORTA.PDR.BYTE = _08_Pm3_MODE_OUTPUT;
    PORTB.PDR.BYTE = _00_Pm1_MODE_INPUT | _00_Pm3_MODE_INPUT | _00_Pm4_MODE_INPUT | _00_Pm5_MODE_INPUT;
}

/* Start user code for adding. Do not edit comment generated here */
void U_PORT_SDA_OUT(void)
{
    PORT7.PDR.BIT.B4 = SET_BIT_HIGH;
}

void U_PORT_SDA_IN(void)
{
    PORT7.PCR.BIT.B4 = SET_BIT_HIGH;
    PORT7.PDR.BIT.B4 = SET_BIT_LOW;
}
/* End user code. Do not edit comment generated here */
