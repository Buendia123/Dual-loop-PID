/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    i2c.c
  * @brief   This file provides code for the configuration
  *          of the I2C instances.
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
/* Includes ------------------------------------------------------------------*/
#include "drv_key.h"
volatile ButtonState buttonState = IDLE;
volatile uint8_t pressCount = 0;
volatile uint16_t pressDuration = 0;
volatile uint16_t lastPressTime = 0;
volatile uint16_t lastIdleTime = 0;

#define LONG_PRESS_THRESHOLD 3000
/* USER CODE END 0 */
void CheckButtonPress(void)
{
    switch (buttonState) {
        case IDLE:
            if (HAL_GPIO_ReadPin(GPIOA, GPIO_KEY) == GPIO_PIN_RESET) {
                // 检测到按键按下
                buttonState = PRESSED;
                lastPressTime = xTaskGetTickCount(); // 记录按下开始时间
                pressDuration = 0;             // 重置按下持续时间
            }else {
            	pressDuration = xTaskGetTickCount();
            	 if (pressDuration - lastIdleTime >= 10000) {
					lastIdleTime = pressDuration;
					pressDuration = 0;
					pressCount = 0;
					buttonResult = 0;
					}
			}
            break;

        case PRESSED:
            if (HAL_GPIO_ReadPin(GPIOA, GPIO_KEY) == GPIO_PIN_SET) {
                // 检测到按键释放
                buttonState = RELEASED;
                pressCount++; // 增加按下次数
            } else {
                // 检测按键按下持续时间
                pressDuration = xTaskGetTickCount() - lastPressTime;
                if (pressDuration >= LONG_PRESS_THRESHOLD) {
                    buttonState = LONG_PRESS;
                }
            }
            break;

        case RELEASED:
            if (HAL_GPIO_ReadPin(GPIOA, GPIO_KEY) == GPIO_PIN_RESET) {
                // 再次检测到按键按下
                buttonState = PRESSED;
                lastPressTime = HAL_GetTick(); // 记录按下开始时间
                pressDuration = 0;             // 重置按下持续时间
            } else {
                    if (pressCount == 1) {
                        buttonResult = 1; // 按下1次
                    } else if (pressCount == 2) {
                        buttonResult = 2; // 按下2次
                    } else {
                        buttonResult = 0; // 按下多次或长按
                        pressCount = 0;
                    }
                    lastPressTime = 0;
                    buttonState = IDLE;
            }
            break;

        case LONG_PRESS:
			buttonResult = 0;
			buttonState = IDLE;
			pressCount = 0;
            break;
    }
}

/* USER CODE BEGIN 1 */

/* USER CODE END 1 */
