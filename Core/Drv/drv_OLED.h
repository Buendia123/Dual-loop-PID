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

#ifndef __DRV_OLED_H__
#define __DRV_OLED_H__

#include "drv_spimb.h"

#define OLED_CMD  0
#define OLED_DATA 1
void DEV_OLED_Init(void);
void OLED_ShowChar(uint8_t x,uint8_t y,uint8_t chr,uint8_t size1,uint8_t mode);
void OLED_ShowString(uint8_t x,uint8_t y,const char *chr,uint8_t size1,uint8_t mode);
void OLED_Refresh(void);
void OLED_Clear(void);
void OLED_DisPlay_On(void);
void OLED_ShowFloat(uint8_t x, uint8_t y, float num, uint8_t integer_len, uint8_t decimal_len, uint8_t size1, uint8_t mode);
void OLED_Clear_part(void);
#endif /* __DRV_SPIM0_H */

