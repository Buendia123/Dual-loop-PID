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
#ifndef __DRV_DELAY_H__
#define __DRV_DELAY_H__

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "stm32f1xx_hal.h"

#define SYSTEM_SUPPORT_RTOS 1

#define DRV_DELAY_125ns() 		__NOP(); __NOP(); __NOP(); __NOP(); __NOP();\
								__NOP(); __NOP(); __NOP(); __NOP(); __NOP();\
								__NOP(); __NOP(); __NOP(); __NOP(); __NOP();\
								__NOP(); __NOP(); __NOP(); __NOP(); __NOP();\
								__NOP(); __NOP(); __NOP(); __NOP(); __NOP();\
								__NOP(); __NOP(); __NOP(); __NOP(); __NOP();\
								__NOP(); __NOP(); __NOP(); __NOP(); __NOP();\
								__NOP(); __NOP(); __NOP(); __NOP(); __NOP();\
								__NOP(); __NOP(); __NOP(); __NOP(); __NOP();



#define DRV_DELAY_2us() for(int i = 0; i < 28; i++) { __NOP(); }




#define DRV_DELAY_625ns()			DRV_DELAY_125ns();DRV_DELAY_125ns();DRV_DELAY_125ns()

#ifdef __cplusplus
}
#endif

#endif /* __I2C_H__ */

