/*
 * WITask.c
 *
 *  Created on: 18 mai 2020
 *      Author: Simon GALAND
 */

#include "WITask.h"
#include "shared.h"
#include "cmsis_os.h"
#include "queue.h"


static uint8_t wiBuffer[UART_DMA_BUFFER_SIZE];
static osThreadId_t uartParserTaskHandle;
static const osThreadAttr_t uartParserTask_attributes2 = {
		.name = "WITask",
		.priority = (osPriority_t) DEFAULT_UART_TASK_PRIORITY,
		.stack_size = DEFAULT_UART_TASK_STACK_SIZE};


void initWITask(){
	HAL_UART_Receive_DMA(&huart4, wiBuffer, UART_DMA_BUFFER_SIZE);
	uartParserTaskHandle = osThreadNew(WITask, wiBuffer, &uartParserTask_attributes2);
}

void WITask(void* arguments)
{
	size_t dma_head = 0, dma_tail = 0;
	size_t cur_msg_sz = 0;
	uint8_t found = 0;
	NMEA_PAQUET wiPaquet;
	memset(wiPaquet.msg,0,PARSER_MESSAGE_SIZE);

	NMEA_PAQUET threadStartPaquet;
	strcpy(threadStartPaquet.msg,"VHF Task Start\r\n");
	osMessageQueuePut(mainNmeaQueueHandle, &threadStartPaquet,0,0); //put thread start msg in queue to be print in main thread

	for(;;)
	{

		do
		{
			__disable_irq();
			dma_tail = UART_DMA_BUFFER_SIZE - huart4.hdmarx->Instance->CNDTR;
			__enable_irq();

			if(dma_tail!=dma_head)
			{

				if(dma_head < dma_tail)
				{
					for(register size_t i=dma_head; i<dma_tail; i++)
					{
						found = (found == 0 && wiBuffer[i] == '\r') ? 1
								: (found == 1 && wiBuffer[i] == '\n') ? 2
										: 0;
						wiPaquet.msg[cur_msg_sz++]= wiBuffer[i];
						if(found==2)
						{
							sendNparsePaquet(wiPaquet, WI);
							memset(wiPaquet.msg,0,PARSER_MESSAGE_SIZE);
							cur_msg_sz=0;
						}
					}

				}
				else
				{
					for(register size_t i=dma_head; i<UART_DMA_BUFFER_SIZE; i++)
					{
						found = (found == 0 && wiBuffer[i] == '\r') ? 1
								: (found == 1 && wiBuffer[i] == '\n') ? 2
										: 0;
						wiPaquet.msg[cur_msg_sz++]= wiBuffer[i];
						if(found==2)
						{
							sendNparsePaquet(wiPaquet, WI);
							memset(wiPaquet.msg,0,PARSER_MESSAGE_SIZE);
							cur_msg_sz=0;
						}
					}

					for(register size_t i=0; i<dma_tail; i++)
					{
						found = (found == 0 && wiBuffer[i] == '\r') ? 1
								: (found == 1 && wiBuffer[i] == '\n') ? 2
										: 0;
						wiPaquet.msg[cur_msg_sz++]= wiBuffer[i];
						if(found==2)
						{
							sendNparsePaquet(wiPaquet, WI);
							memset(wiPaquet.msg,0,PARSER_MESSAGE_SIZE);
							cur_msg_sz=0;
						}
					}

				}

				dma_head=dma_tail;

			}
		}while(dma_head!=(UART_DMA_BUFFER_SIZE- huart4.hdmarx->Instance->CNDTR));
		osDelay(IDLE_TIME); // this should be the minimum time difference between each frame
	}
}

