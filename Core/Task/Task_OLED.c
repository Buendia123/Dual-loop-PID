
/* Includes ------------------------------------------------------------------*/
#include "FreeRTOS.h"
#include "task_OLED.h"


void vOLEDTask(void *pvParameters)
{

	for(;;)
	{
		float dis = Dis_Buf[BufHead]/1000.0;
		if(ang.acc->Roll <0)
		{
		OLED_ShowString(00,0,"R:-",8,1);
		OLED_ShowFloat(22,0,-ang.acc->Roll ,3,1,8,1);
		}
		else{
		OLED_ShowString(00,0,"R:+",8,1);
		OLED_ShowFloat(22,0,+ang.acc->Roll ,3,1,8,1);
		}

		if(ang.acc->Pitch<0)
		{
			OLED_ShowString(00,10,"P:-",8,1);
			OLED_ShowFloat(22,10,-ang.acc->Pitch,3,1,8,1);
		}
		else
		{
			OLED_ShowString(00,10,"P:+",8,1);
			OLED_ShowFloat(22,10,+ang.acc->Pitch,3,1,8,1);
		}

		if(ang.acc->Yaw<0)
		{
		OLED_ShowString(00,20,"Y:-",8,1);
		OLED_ShowFloat(22,20,-ang.acc->Yaw,3,1,8,1);
		}
		else{
			OLED_ShowString(00,20,"Y:+",8,1);
			OLED_ShowFloat(22,20,+ang.acc->Yaw,3,1,8,1);
		}

		OLED_ShowString(00,30,"V:",8,1);
		OLED_ShowFloat(12,30,voltage,1,1,8,1);
		OLED_ShowNum(35,30,buttonResult,1,8,1);
		OLED_ShowString(00,40,"U:",8,1),OLED_ShowFloat(12,40,dis,2,3,8,1);

		OLED_Refresh();
		vTaskDelay(100);
//		UBaseType_t uxHighWaterMark = uxTaskGetStackHighWaterMark(NULL);
//				printf("High Water Mark: %lu\n", uxHighWaterMark);
	}
}
//extern SemaphoreHandle_t xBinarySemaphore;
//void vOLEDTask(void *pvParameters)
//{
//	for(;;)
//	{
//		if (xSemaphoreTake(xBinarySemaphore, portMAX_DELAY) == pdTRUE)
//		{
//			OLED_Clear_part();
//			switch(data_type)
//			{
//				case dir:
//					OLED_ShowString(00,0,"direction:",8,1);
//					switch(state)
//					{
//						case front:
//							OLED_ShowString(00,10,"front",8,1);
//							break;
//						case rear:
//							OLED_ShowString(00,10,"rear",8,1);
//							break;
//						case left:
//							OLED_ShowString(00,10,"left",8,1);
//							break;
//						case right:
//							OLED_ShowString(00,10,"right",8,1);
//							break;
//						case lefront:
//							OLED_ShowString(00,10,"left front",8,1);
//							break;
//						case rifront:
//							OLED_ShowString(00,10,"right front",8,1);
//							break;
//						case lerear:
//							OLED_ShowString(00,10,"left rear",8,1);
//							break;
//						case rirear:
//							OLED_ShowString(00,10,"right rear",8,1);
//							break;
//						case stop:
//							OLED_ShowString(00,10,"stop",8,1);
//							break;
//
//					}
//					break;
//
//				case parameter:
//					OLED_ShowString(00,0,"BP:",8,1);
//					OLED_ShowFloat(20,0,app_para.balance_kp,3,0,8,1);
//
//					OLED_ShowString(00,10,"BI:",8,1);
//					OLED_ShowFloat(20,10,app_para.balance_ki,3,0,8,1);
//
//					OLED_ShowString(00,20,"BD:",8,1);
//					OLED_ShowFloat(20,20,app_para.balance_kd,3,0,8,1);
//
//					OLED_ShowString(00,30,"VP:",8,1);
//					OLED_ShowFloat(20,30,app_para.velocity_kp,3,0,8,1);
//
//					OLED_ShowString(00,40,"VI:",8,1);
//					OLED_ShowFloat(20,40,app_para.velocity_ki,3,0,8,1);
//					break;
//
//				case type_key:
//					OLED_ShowString(00,0,"Key:",8,1);
//					OLED_ShowFloat(00,10,app_key,1,0,8,1);
//					break;
//			}
//			OLED_Refresh();
//		}
//		vTaskDelay(100);
//
//	}
//
//}


