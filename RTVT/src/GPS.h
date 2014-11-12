/*
 * GPS.h
 *
 * Created: 11/5/2014 3:17:03 PM
 *  Author: ee63pc3-user
 */ 

#ifndef F_CPU
#define F_CPU 32000000UL
#endif
#ifndef GPS_H_
#define GPS_H_
#include "integer.h"
#include "uart_driver.h"
#include "stdlib.h"
#include <util/delay.h>
#include <stdio.h>
#include <string.h>
#define MAX_RESP_SIZE 128

namespace NMEA {
	enum CMD {
		GGA_FIX_INFO,
		GSA_OVRL_SATDATA,
		GSV_DET_SATDATA,
		RMC_REC_MIN_DATA,
		VTG_VECT_TRCK,
		GLL_LAT_LON,
		ZDA_DAT_TIM
	};
}

class GPS {
private:
	//Response double buffering
	char buffer[2][MAX_RESP_SIZE];
	BYTE buffptr = 0;
	BYTE windex = 0;
	bool dataReady = false;
	bool readlock = false;
	UartDriver uart;
	char * cmdToString(NMEA::CMD cmd, char * buff3);
public:
	GPS();
	void init();
	void setPeriodic(NMEA::CMD cmd, BYTE rate);
	char * getMostRecent(char * dest);
	char interceptByte();
	void handleDRE();
};




#endif /* GPS_H_ */