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
static struct NMEA_PAQUET vhfNmeaPaquetList[10];
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
	size_t cur_msg = 0;
	uint8_t found = 0;
	struct NMEA_PAQUET threadStartPaquet;
	//threadStartPaquet.msg= "VHF Task Start\r\n";
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
				//				uint8_t *vhfMsg = NULL;
				//				vhfMsg = pvPortMalloc(sizeof(uint8_t)*PARSER_MESSAGE_SIZE);
				//				if(vhfMsg==NULL){
				//					__asm("BKPT #0\n") ; // Break into the debugger
				//				}
				struct NMEA_PAQUET vhfPaquet;
				memset(vhfPaquet.msg,0,PARSER_MESSAGE_SIZE);
				cur_msg_sz=0;
				if(dma_head < dma_tail)
				{
					for(register size_t i=dma_head; i<dma_tail; i++)
					{
						found = (found == 0 && vhfBuffer[i] == '\r') ? 1
								: (found == 1 && vhfBuffer[i] == '\n') ? 2
										: 0;
						vhfNmeaPaquetList[cur_msg].msg[cur_msg_sz++]= vhfBuffer[i];
						if(found==2)
						{
							cur_msg = cur_msg == 10-1 ? 0 : cur_msg + 1;
							memset(vhfNmeaPaquetList[cur_msg].msg,0,PARSER_MESSAGE_SIZE);
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
					}

					for(register size_t i=0; i<dma_tail; i++)
					{
						found = (found == 0 && vhfBuffer[i] == '\r') ? 1
								: (found == 1 && vhfBuffer[i] == '\n') ? 2
										: 0;
						vhfPaquet.msg[cur_msg_sz++]= vhfBuffer[i];
					}

				}

				dma_head=dma_tail;
				switch (osMessageQueuePut(mainNmeaQueueHandle, &(vhfNmeaPaquetList[cur_msg]),0,0)) {
				case osErrorTimeout:
					//					__asm("BKPT #0\n") ; // Break into the debugger
					break;
				case osErrorResource:
					//					__asm("BKPT #0\n") ; // Break into the debugger
					break;
				default:
					break;
				}

			}
		}while(dma_head!=(UART_DMA_BUFFER_SIZE- huart2.hdmarx->Instance->CNDTR));
		osDelay(IDLE_TIME); // this should be the minimum time difference between each frame
	}
}
