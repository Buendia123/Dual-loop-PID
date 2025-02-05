/*
 * Task_Battery.c
 *
 *  Created on: Jan 15, 2025
 *      Author: Lenovo
 */

#include "Task_Battery.h"

void Task_BAT_Flash(void *argument)
{
	uint16_t volt_mV = 0;
	TickType_t blinkDelay = 500;

	for(;;)
	{

		volt_mV = HAL_ADC_CalValtage();
		voltage = volt_mV/1000.0;

		if(volt_mV > 6000 && volt_mV < 7500)
		{
			blinkDelay = 100;
		}
		else
		{
			blinkDelay = 1000;
		}

		HAL_GPIO_TogglePin(GPIOA, GPIO_LED);


		vTaskDelay(blinkDelay); // RTOS 延时
	}
}
