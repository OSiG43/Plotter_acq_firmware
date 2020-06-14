/*
 * utils.h
 *
 *  Created on: 14 juin 2020
 *      Author: Simon GALAND
 */

#ifndef INC_UTILS_H_
#define INC_UTILS_H_

#include "cmsis_os.h"
#include "Constants.h"
#include "shared.h"
#include "minmea.h"

void sendNparsePaquet(NMEA_PAQUET paquet, enum NMEAHANDLER_ENUM nmeaHandler );


#endif /* INC_UTILS_H_ */
