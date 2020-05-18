/*
 * GPSTask.h
 *
 *  Created on: 18 mai 2020
 *      Author: Simon GALAND
 */

#ifndef INC_GPSTASK_H_
#define INC_GPSTASK_H_
#include "stm32f1xx_hal.h"
#include "cmsis_os.h"
#include "string.h"
#include <stdio.h>
#include "Constants.h"

#define IDLE_TIME 25 //time between 2 frames in ticks number

//prototype
void GPSTask(void* arguments);

void GPSTask();




#endif /* INC_GPSTASK_H_ */
