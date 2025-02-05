/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    usart.c
  * @brief   This file provides code for the configuration
  *          of the USART instances.
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
#include "drv_usart.h"

/* USER CODE BEGIN 0 */
QueueHandle_t app_queue;
/* USER CODE END 0 */

UART_HandleTypeDef huart3;

/* USART1 init function */

/* USART3 init function */
static uint8_t usart_rec[64];
void MX_USART3_UART_Init(void)
{

  /* USER CODE BEGIN USART3_Init 0 */

  /* USER CODE END USART3_Init 0 */

  /* USER CODE BEGIN USART3_Init 1 */

  /* USER CODE END USART3_Init 1 */
  huart3.Instance = USART3;
  huart3.Init.BaudRate = 9600;
  huart3.Init.WordLength = UART_WORDLENGTH_8B;
  huart3.Init.StopBits = UART_STOPBITS_1;
  huart3.Init.Parity = UART_PARITY_NONE;
  huart3.Init.Mode = UART_MODE_TX_RX;
  huart3.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart3.Init.OverSampling = UART_OVERSAMPLING_16;
  if (HAL_UART_Init(&huart3) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART3_Init 2 */
	__HAL_UART_ENABLE_IT(&huart3, UART_IT_RXNE);
//	__HAL_UART_ENABLE_IT(&huart3, UART_IT_PE);

	// 配置 NVIC 中断优先级和使能中断
	HAL_NVIC_SetPriority(USART3_IRQn, 5, 2);
	HAL_NVIC_EnableIRQ(USART3_IRQn);
	HAL_UART_Receive_IT(&huart3, usart_rec, 1);
  /* USER CODE END USART3_Init 2 */

}

void HAL_UART_MspInit(UART_HandleTypeDef* uartHandle)
{

  GPIO_InitTypeDef GPIO_InitStruct = {0};
  if(uartHandle->Instance==USART3)
  {
  /* USER CODE BEGIN USART3_MspInit 0 */

  /* USER CODE END USART3_MspInit 0 */
    /* USART3 clock enable */
    __HAL_RCC_USART3_CLK_ENABLE();

    __HAL_RCC_GPIOB_CLK_ENABLE();
    /**USART3 GPIO Configuration
    PB10     ------> USART3_TX
    PB11     ------> USART3_RX
    */
    GPIO_InitStruct.Pin = GPIO_PIN_10;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

    GPIO_InitStruct.Pin = GPIO_PIN_11;
    GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

    /* USART3 interrupt Init */

  /* USER CODE BEGIN USART3_MspInit 1 */

  /* USER CODE END USART3_MspInit 1 */
  }
}


void HAL_UART_MspDeInit(UART_HandleTypeDef* uartHandle)
{

  if(uartHandle->Instance==USART3)
  {
  /* USER CODE BEGIN USART3_MspDeInit 0 */

  /* USER CODE END USART3_MspDeInit 0 */
    /* Peripheral clock disable */
    __HAL_RCC_USART3_CLK_DISABLE();

    /**USART3 GPIO Configuration
    PB10     ------> USART3_TX
    PB11     ------> USART3_RX
    */
    HAL_GPIO_DeInit(GPIOB, GPIO_PIN_10|GPIO_PIN_11);

  /* USER CODE BEGIN USART3_MspDeInit 1 */

  /* USER CODE END USART3_MspDeInit 1 */
  }
}

/* USER CODE BEGIN 1 */
void queue_init(void) {

    app_queue = xQueueCreate(FRAME_QUEUE_LENGTH, FRAME_QUEUE_ITEM_SIZE);
    if (app_queue == NULL) {
        Error_Handler();
    }
}

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
    if (huart->Instance == USART3)
    {

    	BaseType_t xHigherPriorityTaskWoken = pdFALSE;
        static uint8_t rx_buffer[64];    // 静态缓冲区（64字节）
        static uint16_t rx_index = 0;    // 缓冲区索引
        uint8_t received_data;

        if (huart->Instance == USART3) {
            received_data = huart->Instance->DR;  // 读取数据

            // 1. 处理接收到的字节
            if (rx_index < sizeof(rx_buffer)) {
                rx_buffer[rx_index++] = received_data;  // 存入缓冲区

                // 2. 检测结束符 0x0A 0x0D（注意顺序）
                if (rx_index >= 2 &&
                    rx_buffer[rx_index-2] == 0x0A &&
                    rx_buffer[rx_index-1] == 0x0D) {

                    // 动态分配内存保存完整帧（包含结束符）
                	FrameData_t frame_data;
					frame_data.length = rx_index;  // 设置数据长度
					memset(frame_data.data, 0, MAX_FRAME_SIZE);

					memcpy(frame_data.data, rx_buffer, rx_index);
					// 发送帧指针到队列（FromISR API）
					if (xQueueSendFromISR(app_queue, &frame_data, &xHigherPriorityTaskWoken) != pdPASS) {
						vPortFree(app_queue);  // 队列满，释放内存
					}
                    rx_index = 0;  // 重置缓冲区索引
                }
            } else {
                // 缓冲区溢出，重置索引
                rx_index = 0;
            }

            // 重新启用接收中断（指向静态变量）
//            HAL_UART_Receive_IT(&huart3, &received_data, 1);
            HAL_UART_Receive_IT(&huart3, usart_rec, 1);
        }
        portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
    }
}


/* USER CODE END 1 */
