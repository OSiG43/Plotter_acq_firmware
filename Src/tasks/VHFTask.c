/*
 * VHFTask.c
 *
 *  Created on: 13 mai 2020
 *      Author: Lenovo X1
 */

#include "VHFTask.h"
#include "shared.h"
#include "cmsis_os.h"
#include "queue.h"


static uint8_t buffer[UART_DMA_BUFFER_SIZE];
static uint8_t msg[PARSER_MESSAGE_SIZE];
static osThreadId_t uartParserTaskHandle;

const osThreadAttr_t uartParserTask_attributes2 = {
		.name = "UARTParserTask",
		.priority = (osPriority_t) osPriorityNormal,
		.stack_size = 128*4};

void initVHFTask(){
	HAL_UART_Receive_DMA(&huart2, buffer, UART_DMA_BUFFER_SIZE);
	uartParserTaskHandle = osThreadNew(VHFTask, buffer, &uartParserTask_attributes2);

}

void VHFTask(void* arguments)
{
	size_t dma_head = 0, dma_tail = 0;
	size_t cur_msg_sz = 0;
	uint8_t found = 0;


	for(;;)
	{

		do
		{
			__disable_irq();
			dma_tail = UART_DMA_BUFFER_SIZE - huart2.hdmarx->Instance->CNDTR;
			__enable_irq();

			if(dma_tail!=dma_head)
			{
				memset(msg,0,PARSER_MESSAGE_SIZE);
				cur_msg_sz=0;
				if(dma_head < dma_tail)
				{
					for(register size_t i=dma_head; i<dma_tail; i++)
					{
						found = (found == 0 && buffer[i] == '\r') ? 1
								: (found == 1 && buffer[i] == '\n') ? 2
										: 0;
						msg[cur_msg_sz++]= buffer[i];
					}

				}
				else
				{
					for(register size_t i=dma_head; i<UART_DMA_BUFFER_SIZE; i++)
					{
						found = (found == 0 && buffer[i] == '\r') ? 1
								: (found == 1 && buffer[i] == '\n') ? 2
										: 0;
						msg[cur_msg_sz++]= buffer[i];
					}

					for(register size_t i=0; i<dma_tail; i++)
					{
						found = (found == 0 && buffer[i] == '\r') ? 1
								: (found == 1 && buffer[i] == '\n') ? 2
										: 0;
						msg[cur_msg_sz++]= buffer[i];
					}

				}

				dma_head=dma_tail;
				osMessageQueuePut(mainNmeaQueueHandle, &msg,0,0);

			}
		}while(dma_head!=(UART_DMA_BUFFER_SIZE- huart2.hdmarx->Instance->CNDTR));
		osDelay(25); // this should be the minimum time difference between each frame
	}
}
