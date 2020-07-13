/*
 * screenTask.h
 *
 *  Created on: 14 juin 2020
 *      Author: Simon GALAND
 */

#ifndef INC_SCREENTASK_H_
#define INC_SCREENTASK_H_

#include "stm32f1xx_hal.h"
#include "cmsis_os.h"
#include "Constants.h"
#include "shared.h"
#include "ssd1306.h"
#include "fonts.h"
#include "test.h"

#define SCREEN_TASK_PRIORITIE osPriorityLow
#define SCREE_TASK_STACK_SIZE 128*4
#define SCREE_TASK_IDLE_TIME 50

void initScreenTask();

void screenTask();



#endif /* INC_SCREENTASK_H_ */
