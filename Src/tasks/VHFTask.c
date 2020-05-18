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
static uint8_t vhfMsg[PARSER_MESSAGE_SIZE];
static osThreadId_t uartParserTaskHandle;


void initVHFTask(){
	HAL_UART_Receive_DMA(&huart2, vhfBuffer, UART_DMA_BUFFER_SIZE);
	uartParserTaskHandle = osThreadNew(VHFTask, vhfBuffer, &uartParserTask_attributes2);

}

void VHFTask(void* arguments)
{
	size_t dma_head = 0, dma_tail = 0;
	size_t cur_msg_sz = 0;
	uint8_t found = 0;

	const uint8_t* threadStartMsg="VHF Task Start\r\n";
	osMessageQueuePut(mainNmeaQueueHandle, threadStartMsg,0,0); //put thread start msg in queue to be print in main thread

	for(;;)
	{

		do
		{
			__disable_irq();
			dma_tail = UART_DMA_BUFFER_SIZE - huart2.hdmarx->Instance->CNDTR;
			__enable_irq();

			if(dma_tail!=dma_head)
			{
				memset(vhfMsg,0,PARSER_MESSAGE_SIZE);
				cur_msg_sz=0;
				if(dma_head < dma_tail)
				{
					for(register size_t i=dma_head; i<dma_tail; i++)
					{
						found = (found == 0 && vhfBuffer[i] == '\r') ? 1
								: (found == 1 && vhfBuffer[i] == '\n') ? 2
										: 0;
						vhfMsg[cur_msg_sz++]= vhfBuffer[i];
					}

				}
				else
				{
					for(register size_t i=dma_head; i<UART_DMA_BUFFER_SIZE; i++)
					{
						found = (found == 0 && vhfBuffer[i] == '\r') ? 1
								: (found == 1 && vhfBuffer[i] == '\n') ? 2
										: 0;
						vhfMsg[cur_msg_sz++]= vhfBuffer[i];
					}

					for(register size_t i=0; i<dma_tail; i++)
					{
						found = (found == 0 && vhfBuffer[i] == '\r') ? 1
								: (found == 1 && vhfBuffer[i] == '\n') ? 2
										: 0;
						vhfMsg[cur_msg_sz++]= vhfBuffer[i];
					}

				}

				dma_head=dma_tail;
				osMessageQueuePut(mainNmeaQueueHandle, &vhfMsg,0,0);

			}
		}while(dma_head!=(UART_DMA_BUFFER_SIZE- huart2.hdmarx->Instance->CNDTR));
		osDelay(IDLE_TIME); // this should be the minimum time difference between each frame
	}
}
