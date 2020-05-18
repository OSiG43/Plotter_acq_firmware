/*
 * PiloteTask.c
 *
 *  Created on: 18 mai 2020
 *      Author: Simon GALAND
 */

#include <PilotTask.h>
#include "shared.h"
#include "cmsis_os.h"
#include "queue.h"


static uint8_t pilotBuffer[UART_DMA_BUFFER_SIZE];
static uint8_t pilotMsg[PARSER_MESSAGE_SIZE];
static osThreadId_t uartParserTaskHandle;


void initPilotTask(){
	HAL_UART_Receive_DMA(&huart3, pilotBuffer, UART_DMA_BUFFER_SIZE);
	uartParserTaskHandle = osThreadNew(PilotTask, pilotBuffer, &uartParserTask_attributes2);
}

void PilotTask(void* arguments)
{
	size_t dma_head = 0, dma_tail = 0;
	size_t cur_msg_sz = 0;
	uint8_t found = 0;

	const uint8_t* threadStartMsg="Pilot Task Start\r\n";
	osMessageQueuePut(mainNmeaQueueHandle, threadStartMsg,0,0); //put thread start msg in queue to be print in main thread

	for(;;)
	{
		do
		{
			__disable_irq();
			dma_tail = UART_DMA_BUFFER_SIZE - huart3.hdmarx->Instance->CNDTR;
			__enable_irq();
			//printf("tail:%d | head:%d\r\n",dma_tail,dma_head);
			if(dma_tail!=dma_head)
			{
				memset(pilotMsg,0,PARSER_MESSAGE_SIZE);
				cur_msg_sz=0;
				if(dma_head < dma_tail)
				{
					for(register size_t i=dma_head; i<dma_tail; i++)
					{
						found = (found == 0 && pilotBuffer[i] == '\r') ? 1
								: (found == 1 && pilotBuffer[i] == '\n') ? 2
										: 0;
						pilotMsg[cur_msg_sz++]= pilotBuffer[i];
					}

				}
				else
				{
					for(register size_t i=dma_head; i<UART_DMA_BUFFER_SIZE; i++)
					{
						found = (found == 0 && pilotBuffer[i] == '\r') ? 1
								: (found == 1 && pilotBuffer[i] == '\n') ? 2
										: 0;
						pilotMsg[cur_msg_sz++]= pilotBuffer[i];
					}

					for(register size_t i=0; i<dma_tail; i++)
					{
						found = (found == 0 && pilotBuffer[i] == '\r') ? 1
								: (found == 1 && pilotBuffer[i] == '\n') ? 2
										: 0;
						pilotMsg[cur_msg_sz++]= pilotBuffer[i];
					}

				}

				dma_head=dma_tail;
				osMessageQueuePut(mainNmeaQueueHandle, &pilotMsg,0,0);

			}
		}while(dma_head!=(UART_DMA_BUFFER_SIZE- huart3.hdmarx->Instance->CNDTR));
		osDelay(IDLE_TIME); // this should be the minimum time difference between each frame
	}
}
