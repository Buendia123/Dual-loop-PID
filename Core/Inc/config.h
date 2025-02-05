/******************************************************************************
 *
 * COPYRIGHT:
 *   Copyright (c) 2016      Volex Inc.      All rights reserved.
 *
 *   This is unpublished proprietary source code of Volex Inc.
 *   The copyright notice above does not evidence any actual or intended
 *   publication of such source code.
 *
 * DESCRIPTION:
 *   USB packet definition.
 * HISTORY:
 *   2024.03.16      Fremon.Du         Create
 *
 *****************************************************************************/

#ifndef __CONFIG_H__
#define __CONFIG_H__

#include "drv_gpio.h"

#include <string.h>
#include <stdint.h>

#define MAX_FRAME_SIZE 64
typedef struct {
    uint8_t data[MAX_FRAME_SIZE];  // 静态缓冲区
    uint16_t length;               // 数据长度
} FrameData_t;


typedef unsigned char           BOOL;
#define TRUE                    (BOOL)1
#define FALSE                   (BOOL)0
#define HIGH                    (BOOL)1
#define LOW                     (BOOL)0
#define QUEUE_SIZE 				10
#define FRAME_QUEUE_LENGTH 		4
#define FRAME_QUEUE_ITEM_SIZE 	sizeof(FrameData_t)


struct acceleor {
	float Roll;
	float Pitch;
	float Yaw;
};

struct gyroscoper {
	float gyro_Roll;
	float gyro_Pitch;
	float gyro_Yaw;
};

struct Encoder {
    int encodeA;
	int encodeB;
};

struct pose_angle {
    struct acceleor *acc;
    struct gyroscoper *gyro;
};


typedef enum {
    IDLE =  0x00U,
    PRESSED,
    RELEASED,
    LONG_PRESS
} ButtonState;


typedef struct {
    uint16_t capture_val[QUEUE_SIZE];
    uint8_t overflow_count[QUEUE_SIZE];
    uint8_t head;
    uint8_t tail;
    uint8_t count;
} CircularQueue;

typedef enum {
    front =  0x00U,
    rear,
    left,
    right,
	lefront,
	rifront,
	lerear,
	rirear,
	stop
} ControlState;

typedef enum {
    key1 =  0x00U,
	key2,
	key3,
	key4,
	key5,
	key6,
	key7,
	key8,
	key9,
	key0
} ControlKey;

typedef enum {
    dir =  0x00U,
    type_key,
    parameter
} blue_data_type;

typedef struct{
	float balance_kp;
	float balance_ki;
	float balance_kd;
	float velocity_kp;
	float velocity_ki;
}PIDPara;

extern CircularQueue capture_queue;
extern uint16_t Dis_Buf[10];
extern uint8_t BufHead;

extern struct acceleor acc;
extern struct gyroscoper gyo;
extern struct pose_angle ang;
extern struct Encoder motorval;
extern uint16_t ultra_distance;
extern uint8_t buttonResult;
extern float voltage;
extern blue_data_type data_type;
extern PIDPara app_para;
extern ControlKey app_key;
extern ControlState state;
extern float ZeroDiff;
extern float ZeroPoint;
extern int control_speed;
extern int Motor_A, Motor_B;
extern int encodeA_PWMA, encodeA_PWMB,encodeB_PWMA,encodeB_PWMB;
extern BOOL rec_flg;
#endif /* __PKT_DEFINE_H */
