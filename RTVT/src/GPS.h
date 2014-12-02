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
#include "LCD_Driver.h"
#include "stdlib.h"
#include <util/delay.h>
#include <stdio.h>
#include <string.h>
#include <stdint.h>

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
namespace GPSVAL {
	enum VAL {
		FIX_TIME,
		STATUS,
		LAT,
		NS,
		LONG,
		EW,
		SPEED,
		COURSE,
		DATE
	};
}

class GPS {
private:
	//Response double buffering
	uint8_t offsets[11] = {0,10,11,22,23,34,35,41,47,53,59};
	uint8_t sizes[10] = {10,1,11,1,11,1,6,6,6,6};
	uint16_t status_reg;
	char buffer[59];
	bool allowRcv = false;
	uint8_t dataReady = 0;
	UartDriver uart;
	char * cmdToString(NMEA::CMD cmd, char * buff3);
public:
	GPS();
	void init();
	void setPeriodic(NMEA::CMD cmd, BYTE rate);
	uint16_t getStatus();
	void clearStatus();
	void vomit(LCD_Driver * lcd);
	char * retrieve(GPSVAL::VAL value, char * b);
	void updateRegisters();
	void interceptByte();
	void startReceiving();
	void handleDRE();
	void dump(FIL * fp);
};




#endif /* GPS_H_ */