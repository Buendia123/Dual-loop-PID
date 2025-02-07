///*
// * dev_encoder.h
// *
// *  Created on: Jan 19, 2025
// *      Author: Lenovo
// */
//
#ifndef DEV_DEV_ENCODER_H_
#define DEV_DEV_ENCODER_H_

#include "drv_tim.h"

#define SET_ENA_PWM_A(duty)  Set_PWM_RAW(&htim3, TIM_CHANNEL_1, duty)
#define SET_ENA_PWM_B(duty)  Set_PWM_RAW(&htim3, TIM_CHANNEL_2, duty)
#define SET_ENB_PWM_A(duty)  Set_PWM_RAW(&htim3, TIM_CHANNEL_3, duty)
#define SET_ENB_PWM_B(duty)  Set_PWM_RAW(&htim3, TIM_CHANNEL_4, duty)

void Set_Pwm(int motor_A,int motor_B);

int Encoder_Read(uint8_t TIMX);
#endif /* DEV_DEV_ENCODER_H_ */
