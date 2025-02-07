/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    tim.c
  * @brief   This file provides code for the configuration
  *          of the TIM instances.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2025 STMicroelectronics.
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
#include "drv_tim.h"
#include "stm32f1xx_hal_tim.h"
/* USER CODE BEGIN 0 */
#define ENCODER_TIM_PERIOD (uint16_t)(65535)
/* USER CODE END 0 */

TIM_HandleTypeDef htim1;
TIM_HandleTypeDef htim2;
TIM_HandleTypeDef htim3;
TIM_HandleTypeDef htim4;

/* TIM1 init function */
void MX_TIM1_Init(void)
{

  /* USER CODE BEGIN TIM1_Init 0 */

  /* USER CODE END TIM1_Init 0 */

  TIM_MasterConfigTypeDef sMasterConfig = {0};
  TIM_IC_InitTypeDef sConfigIC = {0};

  /* USER CODE BEGIN TIM1_Init 1 */

  /* USER CODE END TIM1_Init 1 */
  htim1.Instance = TIM1;
  htim1.Init.Prescaler = 71;
  htim1.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim1.Init.Period = 1000;
  htim1.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim1.Init.RepetitionCounter = 0;
  htim1.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if (HAL_TIM_IC_Init(&htim1) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim1, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
  sConfigIC.ICPolarity = TIM_INPUTCHANNELPOLARITY_RISING;
  sConfigIC.ICSelection = TIM_ICSELECTION_DIRECTTI;
  sConfigIC.ICPrescaler = TIM_ICPSC_DIV1;
  sConfigIC.ICFilter = 0;
  if (HAL_TIM_IC_ConfigChannel(&htim1, &sConfigIC, TIM_CHANNEL_1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM1_Init 2 */
  HAL_NVIC_SetPriority(TIM1_CC_IRQn, 1, 3);
  HAL_NVIC_EnableIRQ(TIM1_CC_IRQn);

  HAL_NVIC_SetPriority(TIM1_UP_IRQn, 1, 2);
    HAL_NVIC_EnableIRQ(TIM1_UP_IRQn);

  __HAL_TIM_ENABLE_IT(&htim1, TIM_IT_CC1);
//  __HAL_TIM_ENABLE_IT(&htim1, TIM_IT_UPDATE);

  HAL_TIM_Base_Start(&htim1);
  HAL_TIM_IC_Start_IT(&htim1, TIM_CHANNEL_1);
  /* USER CODE END TIM1_Init 2 */

}
/* TIM2 init function */
void MX_TIM2_Init(void)
{
    // 配置TIM2为编码器模式
    htim2.Instance = TIM2;                          // 使用TIM2
    htim2.Init.Prescaler = 0;                       // 预分频器为0
    htim2.Init.CounterMode = TIM_COUNTERMODE_UP;    // 向上计数模式
    htim2.Init.Period = ENCODER_TIM_PERIOD;         // 自动重装载值
    htim2.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1; // 时钟分频
    htim2.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE; // 不预装载

    // 配置编码器模式
    TIM_Encoder_InitTypeDef sEncoderConfig = {0};
    sEncoderConfig.EncoderMode = TIM_ENCODERMODE_TI12;

    // 配置输入通道1 (TI1)
    sEncoderConfig.IC1Polarity  = TIM_ICPOLARITY_RISING; // 采集上升沿
    sEncoderConfig.IC1Selection = TIM_ICSELECTION_DIRECTTI; // 直接映射到 TI1
    sEncoderConfig.IC1Prescaler = TIM_ICPSC_DIV1; // 不分频
    sEncoderConfig.IC1Filter    = 0;   // 无滤波

    // 配置输入通道2 (TI2)
    sEncoderConfig.IC2Polarity  = TIM_ICPOLARITY_RISING;
    sEncoderConfig.IC2Selection = TIM_ICSELECTION_DIRECTTI;
    sEncoderConfig.IC2Prescaler = TIM_ICPSC_DIV1;
    sEncoderConfig.IC2Filter    = 0;                 // 输入滤波器

    // 初始化TIM2编码器模式
    HAL_TIM_Encoder_Init(&htim2, &sEncoderConfig);

    // 4. 使能TIM2更新中断和触发中断
    HAL_TIM_Base_Start_IT(&htim2);  // 启动定时器并使能更新中断
    HAL_TIM_Encoder_Start(&htim2, TIM_CHANNEL_ALL); // 启动编码器模式

    // 5. 配置NVIC（嵌套向量中断控制器）
    HAL_NVIC_SetPriority(TIM2_IRQn, 1, 3);  // 设置优先级
    HAL_NVIC_EnableIRQ(TIM2_IRQn);         // 使能TIM2中断
}

/* TIM3 init function */
void MX_TIM3_Init(void)
{

  /* USER CODE BEGIN TIM3_Init 0 */

  /* USER CODE END TIM3_Init 0 */

  TIM_MasterConfigTypeDef sMasterConfig = {0};
  TIM_OC_InitTypeDef sConfigOC = {0};

  /* USER CODE BEGIN TIM3_Init 1 */

  /* USER CODE END TIM3_Init 1 */
  htim3.Instance = TIM3;
  htim3.Init.Prescaler = 0;
  htim3.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim3.Init.Period = 7199;
  htim3.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim3.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if (HAL_TIM_PWM_Init(&htim3) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_TIM_OC_Init(&htim3) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim3, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
  sConfigOC.OCMode = TIM_OCMODE_PWM1;
  sConfigOC.Pulse = 0;
  sConfigOC.OCPolarity = TIM_OCPOLARITY_HIGH;
  sConfigOC.OCFastMode = TIM_OCFAST_DISABLE;
	if (HAL_TIM_PWM_ConfigChannel(&htim3, &sConfigOC, TIM_CHANNEL_1) != HAL_OK)
	{
	Error_Handler();
	}
	if (HAL_TIM_PWM_ConfigChannel(&htim3, &sConfigOC, TIM_CHANNEL_2) != HAL_OK)
	{
	Error_Handler();
	}
	if (HAL_TIM_PWM_ConfigChannel(&htim3, &sConfigOC, TIM_CHANNEL_3) != HAL_OK)
	{
	Error_Handler();
	}
	if (HAL_TIM_PWM_ConfigChannel(&htim3, &sConfigOC, TIM_CHANNEL_4) != HAL_OK)
	{
	Error_Handler();
	}

	/* USER CODE BEGIN TIM3_Init 2 */
	// 5. 启动PWM输出
	HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_1);
	HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_2);
	HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_3);
	HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_4);
  /* USER CODE BEGIN TIM3_Init 2 */

  /* USER CODE END TIM3_Init 2 */

}

/* TIM4 init function */
void MX_TIM4_Init(void)
{

  /* USER CODE BEGIN TIM4_Init 0 */

  /* USER CODE END TIM4_Init 0 */

  /* USER CODE BEGIN TIM4_Init 1 */

  /* USER CODE END TIM4_Init 1 */
  htim4.Instance = TIM4;
  htim4.Init.Prescaler = 0;
  htim4.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim4.Init.Period = 65535;
  htim4.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim4.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;

	TIM_Encoder_InitTypeDef sEncoderConfig = {0};
	sEncoderConfig.EncoderMode = TIM_ENCODERMODE_TI12;

	// 配置输入通道1 (TI1)
	sEncoderConfig.IC1Polarity  = TIM_ICPOLARITY_RISING;
	sEncoderConfig.IC1Selection = TIM_ICSELECTION_DIRECTTI;
	sEncoderConfig.IC1Prescaler = TIM_ICPSC_DIV1;
	sEncoderConfig.IC1Filter    = 0;

	// 配置输入通道2 (TI2)
	sEncoderConfig.IC2Polarity  = TIM_ICPOLARITY_RISING;
	sEncoderConfig.IC2Selection = TIM_ICSELECTION_DIRECTTI;
	sEncoderConfig.IC2Prescaler = TIM_ICPSC_DIV1;
	sEncoderConfig.IC2Filter    = 0;

	// 初始化TIM2编码器模式
	HAL_TIM_Encoder_Init(&htim4, &sEncoderConfig);

	// 4. 使能TIM2更新中断和触发中断
	HAL_TIM_Base_Start_IT(&htim4);  // 启动定时器并使能更新中断
	HAL_TIM_Encoder_Start(&htim4, TIM_CHANNEL_ALL); // 启动编码器模式

	// 5. 配置NVIC（嵌套向量中断控制器）
	HAL_NVIC_SetPriority(TIM4_IRQn, 1, 3);  // 设置优先级
	HAL_NVIC_EnableIRQ(TIM4_IRQn);         // 使能TIM2中断
  /* USER CODE END TIM4_Init 2 */

}



void HAL_TIM_IC_MspDeInit(TIM_HandleTypeDef* tim_icHandle)
{

  if(tim_icHandle->Instance==TIM1)
  {
  /* USER CODE BEGIN TIM1_MspDeInit 0 */

  /* USER CODE END TIM1_MspDeInit 0 */
    /* Peripheral clock disable */
    __HAL_RCC_TIM1_CLK_DISABLE();

    /**TIM1 GPIO Configuration
    PA8     ------> TIM1_CH1
    */
    HAL_GPIO_DeInit(GPIOA, GPIO_PIN_8);

    /* TIM1 interrupt Deinit */
    HAL_NVIC_DisableIRQ(TIM1_CC_IRQn);
  /* USER CODE BEGIN TIM1_MspDeInit 1 */

  /* USER CODE END TIM1_MspDeInit 1 */
  }
  else if(tim_icHandle->Instance==TIM2)
  {
  /* USER CODE BEGIN TIM2_MspDeInit 0 */

  /* USER CODE END TIM2_MspDeInit 0 */
    /* Peripheral clock disable */
    __HAL_RCC_TIM2_CLK_DISABLE();

    /**TIM2 GPIO Configuration
    PA0-WKUP     ------> TIM2_CH1
    PA1     ------> TIM2_CH2
    */
    HAL_GPIO_DeInit(GPIOA, GPIO_PIN_0|GPIO_PIN_1);

    /* TIM2 interrupt Deinit */
    HAL_NVIC_DisableIRQ(TIM2_IRQn);
  /* USER CODE BEGIN TIM2_MspDeInit 1 */

  /* USER CODE END TIM2_MspDeInit 1 */
  }
  else if(tim_icHandle->Instance==TIM4)
  {
  /* USER CODE BEGIN TIM4_MspDeInit 0 */

  /* USER CODE END TIM4_MspDeInit 0 */
    /* Peripheral clock disable */
    __HAL_RCC_TIM4_CLK_DISABLE();

    /**TIM4 GPIO Configuration
    PB6     ------> TIM4_CH1
    PB7     ------> TIM4_CH2
    */
    HAL_GPIO_DeInit(GPIOB, GPIO_PIN_6|GPIO_PIN_7);

  /* USER CODE BEGIN TIM4_MspDeInit 1 */

  /* USER CODE END TIM4_MspDeInit 1 */
  }
}

void HAL_TIM_PWM_MspDeInit(TIM_HandleTypeDef* tim_pwmHandle)
{

  if(tim_pwmHandle->Instance==TIM3)
  {
  /* USER CODE BEGIN TIM3_MspDeInit 0 */

  /* USER CODE END TIM3_MspDeInit 0 */
    /* Peripheral clock disable */
    __HAL_RCC_TIM3_CLK_DISABLE();
  /* USER CODE BEGIN TIM3_MspDeInit 1 */

  /* USER CODE END TIM3_MspDeInit 1 */
  }
}

/* USER CODE BEGIN 1 */


/* USER CODE END 1 */
