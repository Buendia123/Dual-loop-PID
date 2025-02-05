/*
 * Task_Algorithm.h
 *
 *  Created on: Jan 15, 2025
 *      Author: Lenovo
 */

#ifndef TASK_TASK_ALGORITHM_H_
#define TASK_TASK_ALGORITHM_H_
#include "main.h"

typedef struct {
    float velocity;
    float bias;
    float Last_bias;
} VelocityControlState;


void vControlTask(void *pvParameters);

#endif /* TASK_TASK_ALGORITHM_H_ */
