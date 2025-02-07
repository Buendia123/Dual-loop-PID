/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    gpio.h
  * @brief   This file contains all the function prototypes for
  *          the gpio.c file
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
#ifndef __GPIO_H__
#define __GPIO_H__

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* USER CODE BEGIN Private defines */
#define Encoder_TIM2_CH1 		GPIO_PIN_0
#define Encoder_TIM2_CH2 		GPIO_PIN_1
#define Encoder_TIM4_CH1 		GPIO_PIN_6
#define Encoder_TIM4_CH2 		GPIO_PIN_7
#define PWM_TIM3_CH1 			GPIO_PIN_6
#define PWM_TIM3_CH2 			GPIO_PIN_7
#define PWM_TIM3_CH3	 		GPIO_PIN_0
#define PWM_TIM3_CH4 			GPIO_PIN_1

#define Ultra_Echo 				GPIO_PIN_8
#define Ultra_Trig	 			GPIO_PIN_11

#define USART1_TX 		 		GPIO_PIN_9
#define USART1_RX 	     		GPIO_PIN_10
#define USART3_TX 		 		GPIO_PIN_10
#define USART3_RX 	     		GPIO_PIN_11
#define GPIO_KEY 	     		GPIO_PIN_5

#define GPIO_LED 	     		GPIO_PIN_4

#define OLED_DC 	     		GPIO_PIN_15
#define OLED_RST 		 		GPIO_PIN_13
#define OLED_SDA 	    		 GPIO_PIN_14
#define OLED_SCL 	     		GPIO_PIN_15

#define PIN_KEY 	     		GPIO_PIN_4
#define MPU_SCL 	     		GPIO_PIN_8
#define MPU_SDA 		 		GPIO_PIN_9


#define MPU_GPIO 		 		GPIOB

#define SPIM_DC_PORT			GPIOA
#define SPIM_CLK_PORT			GPIOC
#define SPIM_MOSI_PORT			GPIOC
#define SPIM_REST_PORT			GPIOC

#define SPIM_DC_PORT			GPIOA
#define SPIM_CLK_PORT			GPIOC
#define SPIM_MOSI_PORT			GPIOC
#define SPIM_REST_PORT			GPIOC

#define OLED_SPI_DC_Pin			GPIO_PIN_15
#define OLED_SPI_CLK_Pin		GPIO_PIN_15
#define OLED_SPI_MOSI_Pin		GPIO_PIN_14
#define OLED_SPI_REST_Pin		GPIO_PIN_13
/* USER CODE END Private defines */

void MX_GPIO_Init(void);

/* USER CODE BEGIN Prototypes */

/* USER CODE END Prototypes */

#ifdef __cplusplus
}
#endif
#endif /*__ GPIO_H__ */

