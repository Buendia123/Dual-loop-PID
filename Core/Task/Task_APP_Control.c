/*
 * Task_APP_Control.c
 *
 *  Created on: Jan 15, 2025
 *      Author: Lenovo
 */

#include "Task_APP_Control.h"
#include <stdio.h>
#include "semphr.h"


SemaphoreHandle_t xBinarySemaphore = NULL;
extern QueueHandle_t app_queue;
char tempStr[2];
static void App_Handle(uint8_t *data, uint16_t length);

void InitBinarySemaphore(void)
{
    // 创建二值信号量
    xBinarySemaphore = xSemaphoreCreateBinary();
    if (xBinarySemaphore == NULL)
    {
        // 创建失败，通常是内存不足等原因
        Error_Handler();
    }
}


void vAPPTask(void *pvParameters) {
    FrameData_t recvFrame;
    BaseType_t err;
    InitBinarySemaphore();
    for (;;) {
        // 从队列中接收结构体
        err = xQueueReceive(app_queue, &recvFrame, portMAX_DELAY);
        if (err != pdTRUE) {
            continue;
        }
        App_Handle(recvFrame.data, recvFrame.length);

    }
}

static float Parse_Float(uint8_t *data, uint8_t digits) {
    float value = 0.0;
    for (uint8_t i = 0; i < digits; i++) {
        value = value * 10 + (data[i] - '0');  // 将字符转换为数字
    }
    return value;
}

static void App_Handle(uint8_t *data, uint16_t length)
{
	BaseType_t xHigherPriorityTaskWoken = pdFALSE;
	 for (int i = 0; i < length; i++) {
	 sprintf(tempStr, "%02X", data[i]);
	 uint8_t x_pos = i * 16;

	 OLED_ShowString(x_pos, 50, (uint8_t*)tempStr, 8, 1);
	 }
	 OLED_Refresh();

	 xSemaphoreGiveFromISR(xBinarySemaphore, &xHigherPriorityTaskWoken);

	if (data[0] == 0x79)
	{
		if(data[1] == 0x64)//oritate
		{
			data_type = dir;
			if(data[2] == 0xFF && data[3] == 0x0F)
			{
				state = front;
			}
			if(data[2] == 0x5A && data[3] == 0x00)
			{
				state = rear;
			}
			if(data[2] == 0xB4 && data[3] == 0x00)
			{
				state = left;
			}
			if(data[2] == 0x01 && data[3] == 0x00)
			{
				state = right;
			}
			if(data[2] == 0xDF && data[3] == 0x00)
			{
				state = lefront;
			}
			if(data[2] == 0xFF && data[3] == 0x3B)
			{
				state = rifront;
			}
			if(data[2] == 0x86 && data[3] == 0x00)
			{
				state = lerear;
			}
			if(data[2] == 0x2B && data[3] == 0x00)
			{
				state = rirear;
			}
			if(data[2] == 0xFF && data[3] == 0xFF)
			{
				state = stop;
			}

		}
		if(data[1] == 0x62)
		{
			data_type = type_key;
			switch(data[2])
			{
			case 0x01:
				app_key = key1;
				break;
			case 0x02:
				app_key = key2;
				break;
			case 0x03:
				app_key = key3;
				control_speed = 40;
				break;
			case 0x04:
				app_key = key4;
				control_speed = 30;
				break;
			case 0x5:
				app_key = key5;
				control_speed = 20;
				break;
			case 0x06:
				app_key = key6;
				break;
			case 0x07:
				app_key = key7;
				break;
			case 0x08:
				app_key = key8;
				break;
			case 0x09:
				app_key = key9;
				break;
			}
		}
	}
	if(data[0] == 0xAA && data[1] == 0xAF)
	{
		data_type = parameter;
		uint8_t balance_kp_digits = data[2];
		uint8_t balance_ki_digits = data[3];
		uint8_t balance_kd_digits = data[4];
		uint8_t velocity_kp_digits = data[5];
		uint8_t velocity_ki_digits = data[6];
		app_para.balance_kp = Parse_Float(data + 7, balance_kp_digits);
		app_para.balance_ki = Parse_Float(data + 7 + balance_kp_digits, balance_ki_digits);
		app_para.balance_kd = Parse_Float(data + 7 + balance_kp_digits + balance_ki_digits, balance_kd_digits);
		app_para.velocity_kp = Parse_Float(data + 7 + balance_kp_digits + balance_ki_digits + balance_kd_digits, velocity_kp_digits);
		app_para.velocity_ki = Parse_Float(data + 7 + balance_kp_digits + balance_ki_digits + balance_kd_digits + velocity_kp_digits, velocity_ki_digits);
	}

}
