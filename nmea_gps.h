#include <xc.h>
#include <pic18f4550.h>
#include "Config.h"

#include "uart.h"
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#ifndef GPS_NEO_H
#define	GPS_NEO_H

#define _GPRMC_ 1
#define _GPGGA_ 2
#define _OTHER_ 3

short GPS_Get_Data(void);
uint8_t GPS_Second(void);
uint8_t GPS_Minute(void);
uint8_t GPS_Hour(void);
uint8_t GPS_Day(void);
uint8_t GPS_Month(void);
uint8_t GPS_Year(void);
uint8_t GPS_Satellites(void);
float GPS_Latitude(void);
float GPS_Longitude(void);
float GPS_Altitude(void);
float GPS_Speed(void);
float GPS_Course(void);

#endif
