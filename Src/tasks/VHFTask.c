/*
 * VHFTask.c
 *
 *  Created on: 13 mai 2020
 *      Author: Simon GALAND
 */

#include "VHFTask.h"
#include "shared.h"
#include "cmsis_os.h"
#include "queue.h"


static uint8_t vhfBuffer[UART_DMA_BUFFER_SIZE];
static osThreadId_t uartParserTaskHandle;
static const osThreadAttr_t uartParserTask_attributes2 = {
		.name = "VHFTask",
		.priority = (osPriority_t) DEFAULT_UART_TASK_PRIORITY,
		.stack_size = DEFAULT_UART_TASK_STACK_SIZE};

void initVHFTask(){
	HAL_UART_Receive_DMA(&huart2, vhfBuffer, UART_DMA_BUFFER_SIZE);
	uartParserTaskHandle = osThreadNew(VHFTask, vhfBuffer, &uartParserTask_attributes2);

}

void VHFTask(void* arguments)
{
	size_t dma_head = 0, dma_tail = 0;
	size_t cur_msg_sz = 0;
	uint8_t found = 0;
	NMEA_PAQUET vhfPaquet;
	memset(vhfPaquet.msg,0,PARSER_MESSAGE_SIZE);

	NMEA_PAQUET threadStartPaquet;
	strcpy(threadStartPaquet.msg,"VHF Task Start\r\n");
	osMessageQueuePut(mainNmeaQueueHandle, &threadStartPaquet,0,0); //put thread start msg in queue to be print in main thread

	for(;;)
	{


		do
		{
			__disable_irq();
			dma_tail = UART_DMA_BUFFER_SIZE - huart2.hdmarx->Instance->CNDTR;
			__enable_irq();

			if(dma_tail!=dma_head)
			{

				if(dma_head < dma_tail)
				{
					for(register size_t i=dma_head; i<dma_tail; i++)
					{
						found = (found == 0 && vhfBuffer[i] == '\r') ? 1
								: (found == 1 && vhfBuffer[i] == '\n') ? 2
										: 0;
						vhfPaquet.msg[cur_msg_sz++]= vhfBuffer[i];
						if(found==2)
						{
							osMessageQueuePut(mainNmeaQueueHandle, &vhfPaquet,0,0);
							memset(vhfPaquet.msg,0,PARSER_MESSAGE_SIZE);
							cur_msg_sz=0;
						}
					}



				}
				else
				{
					for(register size_t i=dma_head; i<UART_DMA_BUFFER_SIZE; i++)
					{
						found = (found == 0 && vhfBuffer[i] == '\r') ? 1
								: (found == 1 && vhfBuffer[i] == '\n') ? 2
										: 0;
						vhfPaquet.msg[cur_msg_sz++]= vhfBuffer[i];
						if(found==2)
						{
							osMessageQueuePut(mainNmeaQueueHandle, &vhfPaquet,0,0);
							memset(vhfPaquet.msg,0,PARSER_MESSAGE_SIZE);
							cur_msg_sz=0;
						}
					}

					for(register size_t i=0; i<dma_tail; i++)
					{
						found = (found == 0 && vhfBuffer[i] == '\r') ? 1
								: (found == 1 && vhfBuffer[i] == '\n') ? 2
										: 0;
						vhfPaquet.msg[cur_msg_sz++]= vhfBuffer[i];
						if(found==2)
						{
							osMessageQueuePut(mainNmeaQueueHandle, &vhfPaquet,0,0);
							memset(vhfPaquet.msg,0,PARSER_MESSAGE_SIZE);
							cur_msg_sz=0;
						}
					}

				}

				dma_head=dma_tail;

			}
		}while(dma_head!=(UART_DMA_BUFFER_SIZE- huart2.hdmarx->Instance->CNDTR));
		osDelay(IDLE_TIME); // this should be the minimum time difference between each frame
	}
}
