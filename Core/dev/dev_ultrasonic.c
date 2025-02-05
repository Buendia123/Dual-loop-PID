/*
 * dev_ultrasonic.c
 *
 *  Created on: Jan 19, 2025
 *      Author: Lenovo
 */

#include "dev_ultrasonic.h"

volatile uint8_t TIM1CH1_CAPTURE_STA;
volatile uint16_t TIM1CH1_CAPTURE_VAL;
static uint8_t  tim1ch1_capture_ovf  = 0;
void TIM1_CC_IRQHandler(void)
{
	 if (!(TIM1CH1_CAPTURE_STA & 0x80))
	{
		if (__HAL_TIM_GET_FLAG(&htim1, TIM_FLAG_CC1))
		{
			if (TIM1CH1_CAPTURE_STA & 0x40)  // falling capture
			{
				TIM1CH1_CAPTURE_STA |= 0x80;  // 捕获逻辑高，获取距离值
				TIM1CH1_CAPTURE_VAL = __HAL_TIM_GET_COMPARE(&htim1, TIM_CHANNEL_1);
				__HAL_TIM_DISABLE_IT(&htim1, TIM_IT_UPDATE);

				// 将捕获值存入环形队列，处理溢出次数
				if (capture_queue.count < QUEUE_SIZE)
				{
					capture_queue.tail = (capture_queue.tail + 1) % QUEUE_SIZE;
					capture_queue.capture_val[capture_queue.tail] = TIM1CH1_CAPTURE_VAL;
					capture_queue.overflow_count[capture_queue.tail] = tim1ch1_capture_ovf;
					capture_queue.count++;
				}
				else
				{
					capture_queue.head = (capture_queue.head + 1) % QUEUE_SIZE;
					capture_queue.capture_val[capture_queue.tail] = TIM1CH1_CAPTURE_VAL;
					capture_queue.overflow_count[capture_queue.tail] = TIM1CH1_CAPTURE_STA & 0x3F;
					capture_queue.tail = (capture_queue.tail + 1) % QUEUE_SIZE;
				}

				__HAL_TIM_SET_CAPTUREPOLARITY(&htim1, TIM_CHANNEL_1, TIM_INPUTCHANNELPOLARITY_RISING);
			}
			else  // rising capture
			{
				TIM1CH1_CAPTURE_STA = 0;
				TIM1CH1_CAPTURE_VAL = 0;
				tim1ch1_capture_ovf = 0;
				TIM1CH1_CAPTURE_STA |= 0x40;  // 捕获上升沿标志
				__HAL_TIM_SET_COUNTER(&htim1, 0);  // 清零计数器
				__HAL_TIM_ENABLE_IT(&htim1, TIM_IT_UPDATE);
				__HAL_TIM_SET_CAPTUREPOLARITY(&htim1, TIM_CHANNEL_1, TIM_INPUTCHANNELPOLARITY_FALLING);
			}
		}

	}
	__HAL_TIM_CLEAR_FLAG(&htim1,TIM_FLAG_CC1);  // 清除中断标志

}


void TIM1_UP_IRQHandler(void)
{

	if (TIM1CH1_CAPTURE_STA & 0x40)
	{
		if (__HAL_TIM_GET_FLAG(&htim1, TIM_FLAG_UPDATE) != RESET) {
			 if (tim1ch1_capture_ovf >= 63)
			{
				TIM1CH1_CAPTURE_VAL = 1000;
				TIM1CH1_CAPTURE_STA |= 0x80;
				__HAL_TIM_DISABLE_IT(&htim1, TIM_IT_UPDATE);
			}
			else
			{
				tim1ch1_capture_ovf++;
			}
	  __HAL_TIM_CLEAR_FLAG(&htim1, TIM_FLAG_UPDATE);
		}
	}
}

void Get_Distance(void)
{
    int distance = 0;
    HAL_GPIO_WritePin(GPIOA, GPIO_PIN_11, GPIO_PIN_SET);  // 触发超声波信号
    HAL_Delay(15);
    HAL_GPIO_WritePin(GPIOA, GPIO_PIN_11, GPIO_PIN_RESET);
    if (TIM1CH1_CAPTURE_STA&0x80)
    {
		if (capture_queue.count > 0)  // 确保队列中有数据
		{
			// 获取队列中的最新数据
			uint16_t capture_val = capture_queue.capture_val[capture_queue.head];
			uint8_t overflow_count = capture_queue.overflow_count[capture_queue.head];

			// 计算完整的计数值，包括溢出
			distance = (overflow_count-1)* 1000 + capture_val;

			// 计算距离
			Dis_Buf[BufHead] = (distance * 174) >> 10 ;
			BufHead = BufHead > 10? 0 : (BufHead + 1) % QUEUE_SIZE;

			// 更新队列头指针，准备读取下一条数据
			capture_queue.head = (capture_queue.head + 1) % QUEUE_SIZE;
			capture_queue.count--;
		}
		TIM1CH1_CAPTURE_STA = 0;
    }
}
