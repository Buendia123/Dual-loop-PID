/*
 * dev_encoder.c
 *
 *  Created on: Jan 19, 2025
 *      Author: Lenovo
 */

#include"dev_encoder.h"

void Encoder_Read(uint8_t TIMX)
{
	switch(TIMX)
	{
		case 2:
			motorval.encodeA = -(int16_t)__HAL_TIM_GET_COUNTER(&htim2);
			__HAL_TIM_SET_COUNTER(&htim2, 0);
			break;
		case 4:
			motorval.encodeB = +(int16_t)__HAL_TIM_GET_COUNTER(&htim4);
			__HAL_TIM_SET_COUNTER(&htim4, 0);
			break;
		default:
			break;
	}
}

void TIM2_IRQHandler(void)
{
	if(TIM2->SR&TIM_SR_UIF)
	{
	}
	TIM2->SR&=~TIM_SR_UIF;
}

void TIM4_IRQHandler(void)
{
	if(TIM4->SR&TIM_SR_UIF)
	{
	}
	TIM4->SR&=~TIM_SR_UIF;
}
