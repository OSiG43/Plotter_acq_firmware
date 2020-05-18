/*
 * VHFTask.h
 *
 *  Created on: 13 mai 2020
 *      Author: Lenovo X1
 */

#ifndef TASKS_VHFTASK_H_
#define TASKS_VHFTASK_H_
#include "stm32f1xx_hal.h"
#include "cmsis_os.h"
#include "string.h"
#include <stdio.h>
#include "Constants.h"





//variables

//prototype
void VHFTask(void* arguments);


void initVHFTask();



#endif /* TASKS_VHFTASK_H_ */
