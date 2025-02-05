/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    i2c.h
  * @brief   This file contains all the function prototypes for
  *          the i2c.c file
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2024 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __I2CM_H__
#define __I2CM_H__

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "config.h"


typedef enum
{
  I2C_OK       =  0x00U,
  I2C_TimeOut  =  0x01U,
  I2C_Busy     =  0x02U,
  I2C_NACK     =  0x03U
} I2State;
typedef unsigned char           BOOL;
#define TRUE                    (BOOL)1
#define FALSE                   (BOOL)0
#define HIGH                    (BOOL)1
#define LOW                     (BOOL)0

 /******************************************************************************
  * FUNCTION NAME:
  *   DRV_I2CM_Stream
  * DESCRIPTION:
  *   I2C master send write/read combined stream.
  * PARAMETERS:
  *   vI2cAddr  : I2C slave chip address.
  *   vWriteLen : Write length.
  *   aWriteBuf : Write buffer.
  *   vReadLen  : Read length.
  *   aReadBuf  : Read buffer.
  * RETURN:
  *   TRUE   : Send stream successfully.
  *   FALSE  : Send stream failed.
  * NOTES:
  *   N/A
  * HISTORY:
  *   @2021.6.1        Fremon          Create
  *****************************************************************************/
BOOL DRV_I2CM_Stream(uint8_t vI2cAddr,uint8_t vWriteLen, uint8_t *aWriteBuf, uint8_t vReadLen, uint8_t *aReadBuf);

/******************************************************************************
 * FUNCTION NAME:
 *   DRV_I2CM_Probe
 * DESCRIPTION:
 *   I2C master probe chip.
 * PARAMETERS:
 *   vI2cAddr : I2C chip address.
 * RETURN:
 *   TRUE   : Probe successfully.
 *   FALSE  : Probe failed.
 * NOTES:
 *   N/A
 * HISTORY:
 *   @2021.6.1        Fremon          Create
 *****************************************************************************/
BOOL DRV_I2CM_Probe(uint8_t vI2cAddr);

/******************************************************************************
 * FUNCTION NAME:
 *   DRV_I2CM_Init
 * DESCRIPTION:
 *   I2C master driver init.
 * PARAMETERS:
 *   N/A
 * RETURN:
 *   N/A
 * NOTES:
 *   N/A
 * HISTORY:
 *   @2021.6.1        Fremon          Create
 *****************************************************************************/
void DRV_I2CM_Init(void);
int i2cWrite(uint8_t addr, uint8_t reg, uint8_t len, uint8_t *data);
int i2cRead(uint8_t addr, uint8_t reg, uint8_t len, uint8_t *data);
BOOL IICwriteBits(uint8_t I2cAdd, uint8_t OffSet, uint8_t bitstart, uint8_t len, uint8_t Data);
BOOL IICreadBytes(uint8_t i2c_addr, uint8_t offset, uint8_t read_len, uint8_t* read_buf);
#ifdef __cplusplus
}
#endif

#endif /* __I2C_H__ */

