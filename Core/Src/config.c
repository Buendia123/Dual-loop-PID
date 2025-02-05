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

#include "config.h"
#include <string.h>

struct acceleor acc = {
	0.0,
	0.0,
	0.0
};
struct gyroscoper gyo = {
	0.0,
	0.0,
	0.0
};

struct pose_angle ang = {
	&acc,
	&gyo
};
struct Encoder motorval=
{
	0,
	0
};
CircularQueue capture_queue = {{0}, {0}, 0, 0, 0};
uint16_t Dis_Buf[QUEUE_SIZE] = {0};
uint8_t BufHead;
uint16_t ultra_distance = 0;
uint8_t buttonResult = 0;
float voltage = 0.0;
PIDPara app_para = {0.0};
ControlKey app_key = key0;
ControlState state = stop;
blue_data_type data_type;
float ZeroDiff = 0;
float ZeroPoint = 1;
int control_speed = 15;
int Motor_A, Motor_B = 0;
int encodeA_PWMA, encodeA_PWMB,encodeB_PWMA,encodeB_PWMB;
BOOL rec_flg=0;
