/*
 * utils.c
 *
 *  Created on: 14 juin 2020
 *      Author: Simon GALAND
 */
#include <parser.h>

void sendNparsePaquet(NMEA_PAQUET paquet, enum NMEAHANDLER_ENUM nmeaHandler ){
	osMessageQueuePut(mainNmeaQueueHandle, &paquet,0,0);
	switch (nmeaHandler) {
		case GPS:
			osMessageQueuePut(vhfNmeaQueueHandle, &paquet,0,0);
			break;
		case WI:
			osMessageQueuePut(pilotNmeaQueueHandle, &paquet,0,0);
			break;
		default:
			break;
	}
}

