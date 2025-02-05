/*
 * Task_Read.c
 *
 *  Created on: Jan 15, 2025
 *      Author: Lenovo
 */
#include "Task_Read.h"

extern void Read_DMP(struct pose_angle* angle);

void vReadTask(void *pvParameters)
{

	while (1)
	{
		Encoder_Read(2);
		Encoder_Read(4);
		Read_DMP(&ang);
		CheckButtonPress();
		Get_Distance();
//		UBaseType_t uxHighWaterMark = uxTaskGetStackHighWaterMark(NULL);
//		printf("High Water Mark: %lu\n", uxHighWaterMark);

		vTaskDelay(20);
	}


}
