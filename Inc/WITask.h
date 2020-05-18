/*
 * WITask.h
 *
 *  Created on: 18 mai 2020
 *      Author: Simon GALAND
 */

#ifndef INC_WITASK_H_
#define INC_WITASK_H_

#include "stm32f1xx_hal.h"
#include "cmsis_os.h"
#include "string.h"
#include <stdio.h>
#include "Constants.h"

#define IDLE_TIME 25 //time between 2 frames in ticks number
#define UART huart4 //Wind instruments is on uart 4

//prototype
void WITask(void* arguments);

void initWITask();




#endif /* INC_WITASK_H_ */
