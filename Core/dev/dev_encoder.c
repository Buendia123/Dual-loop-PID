/*
 * dev_encoder.c
 *
 *  Created on: Jan 19, 2025
 *      Author: Lenovo
 */

#include"dev_encoder.h"

void Set_PWM_RAW(TIM_HandleTypeDef *htim, uint32_t channel, uint32_t pulse)
{
    if (pulse > htim->Init.Period)
    {
        pulse = htim->Init.Period; // 确保不会超过最大值
    }

    // 直接设置 CCRx 值
    __HAL_TIM_SET_COMPARE(htim, channel, pulse);
}

void Set_Pwm(int motor_A,int motor_B)
{
	if(motor_A<0){
		SET_ENB_PWM_B(7199);
		SET_ENB_PWM_A(7199 + motor_A);
	}
	else{
		SET_ENB_PWM_A(7199);
		SET_ENB_PWM_B(7199 - motor_A);//PWM赋值
	}
	if(motor_B<0)
	{
		SET_ENA_PWM_B(7199);
		SET_ENA_PWM_A(7199 + motor_B);
	}
	else
	{
		SET_ENA_PWM_A(7199);
		SET_ENA_PWM_B(7199 - motor_B);//PWM赋值
	}
}

int Encoder_Read(uint8_t TIMX)
{
   int Encoder_TIM;
   switch(TIMX)
	 {
	   case 2:
	   {
		   Encoder_TIM = (int16_t)__HAL_TIM_GET_COUNTER(&htim2);
		   __HAL_TIM_SET_COUNTER(&htim2, 0);
		   break;
	   }
		 case 4:{
			   Encoder_TIM = (int16_t)__HAL_TIM_GET_COUNTER(&htim4);
			   __HAL_TIM_SET_COUNTER(&htim4, 0);
//			 Encoder_TIM= (short)TIM4 -> CNT;
//			 TIM4 -> CNT=0;
			 break;
		 }
		 default:{
			 Encoder_TIM=0;
			 break;
		 }
	 }
		return Encoder_TIM;
}
