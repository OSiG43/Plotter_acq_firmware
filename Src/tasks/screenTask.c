/*
 * screenTask.c
 *
 *  Created on: 14 juin 2020
 *      Author: Simon GALAND
 */
#include "screenTask.h"

static osThreadId_t screenTaskHandle;
static const osThreadAttr_t screenTask_attributes = {
		.name = "PilotTask",
		.priority = (osPriority_t) SCREEN_TASK_PRIORITIE,
		.stack_size = SCREE_TASK_STACK_SIZE};

void initScreenTask(){
	screenTaskHandle = osThreadNew(screenTask, NULL, &screenTask_attributes);
}

void screenTask(){
	osDelay(1000);
	SSD1306_Init();
	for(;;){

		osDelay(SCREE_TASK_IDLE_TIME);
	}
}
