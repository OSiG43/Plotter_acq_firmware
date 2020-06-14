/*
 * HardConstants.h
 *
 *  Created on: 9 mai 2020
 *      Author: Lenovo X1
 */

#ifndef CONSTANTS_H_
#define CONSTANTS_H_
#include "stm32f1xx_hal.h"
#include "cmsis_os.h"

#define AUX_PORT GPIOB
#define AUX_1_PIN GPIO_PIN_4
#define AUX_2_PIN GPIO_PIN_5

//Concerning DMA and uart receiving
#define UART_DMA_BUFFER_SIZE 2048
#define PARSER_MESSAGE_SIZE 90
typedef struct {                                // object data type
  uint8_t msg[PARSER_MESSAGE_SIZE];
  size_t len;
}NMEA_PAQUET;

enum NMEAHANDLER_ENUM{
	VHF,
	GPS,
	PILOT,
	WI
};

#define DEFAULT_UART_TASK_STACK_SIZE 128*4
#define DEFAULT_UART_TASK_PRIORITY osPriorityNormal

//Concerning peripheral
UART_HandleTypeDef huart4;	//Wind instrument UART
UART_HandleTypeDef huart5; //pc UART
UART_HandleTypeDef huart1; //GPS UART
UART_HandleTypeDef huart2; //VHF UART
UART_HandleTypeDef huart3; //Pilot UART
DMA_HandleTypeDef hdma_usart2_tx;
DMA_HandleTypeDef hdma_usart2_rx;

#endif /* CONSTANTS_H_ */
