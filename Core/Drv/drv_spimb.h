/******************************************************************************
 *
 * COPYRIGHT:
 *   Copyright      Volex.      All rights reserved.
 *
 *
 * DESCRIPTION:
 *   SPI Master driver related.
 * HISTORY:
 *   @2022.09.01        AAA          Create
 *
 *****************************************************************************/

#ifndef __DRV_SPIMB_H__
#define __DRV_SPIMB_H__
#define DRV_SPIM0_SUPPORT 1
#include "main.h"
#define DRV_SPIM_SDA_GET() 				HAL_GPIO_ReadPin(SPIM_MOSI_PORT,OLED_SPI_MOSI_Pin);

#define DRV_SPIM_SDA_SET(_v)               \
	do                                      \
	{                                       \
		HAL_GPIO_WritePin(SPIM_MOSI_PORT,OLED_SPI_MOSI_Pin,_v);        \
	} while (0)

#define DRV_SPIM_SCK_SET(_v)               \
	do                                      \
	{                                       \
		HAL_GPIO_WritePin(SPIM_CLK_PORT,OLED_SPI_CLK_Pin,_v);        \
	} while (0)

#define DRV_SPIM_RST_SET(_v)               \
	do                                      \
	{                                       \
		HAL_GPIO_WritePin(SPIM_REST_PORT,OLED_SPI_REST_Pin,_v);        \
	} while (0)

#define DRV_SPIM_DC_SET(_v)               \
	do                                      \
	{                                       \
		HAL_GPIO_WritePin(SPIM_DC_PORT,OLED_SPI_DC_Pin,_v);        \
	} while (0)


#if DRV_SPIM0_SUPPORT

/******************************************************************************
 * FUNCTION NAME:
 *   DRV_SPIM0_ExchangeByte
 * DESCRIPTION:
 *   SPI Master exchange byte data.
 * PARAMETERS:
 *   vData : Data to be exchanged.
 * RETURN:
 *   Exchanged data.
 * NOTES:
 *   N/A
 * HISTORY:
 *   @2022.09.01       AAA         Create
 *****************************************************************************/
uint8_t drv_oled_spim_stream(uint8_t pTxData, uint8_t CmdFlag);


#endif


#endif /* __DRV_SPIM0_H */

