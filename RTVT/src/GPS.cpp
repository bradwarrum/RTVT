/*
 * GPS.cpp
 *
 * Created: 11/5/2014 3:17:12 PM
 *  Author: ee63pc3-user
 */ 
 #include "GPS.h"

 GPS::GPS() : uart(128, 8) {
 	uart.init(&USARTD1, ParityMode::DISABLED, CharSize::EIGHT, false, 12, 0x50);//4800 baud
 };

 void GPS::init() {
 	uart.sendString("$PSRF105,1*3E\r\n", 15, 0);
	_delay_ms(100);
	setPeriodic(NMEA::GGA_FIX_INFO, 0);
	_delay_ms(100);
	setPeriodic(NMEA::GLL_LAT_LON, 0);
	_delay_ms(100);
	setPeriodic(NMEA::GSV_DET_SATDATA, 0);
	_delay_ms(100);
	setPeriodic(NMEA::GSA_OVRL_SATDATA, 0);
	_delay_ms(100);
	setPeriodic(NMEA::RMC_REC_MIN_DATA, 3);
	_delay_ms(100);
	setPeriodic(NMEA::VTG_VECT_TRCK, 0);
	_delay_ms(100);
	setPeriodic(NMEA::ZDA_DAT_TIM, 0);
 }

 void GPS::setPeriodic(NMEA::CMD cmd, BYTE rate) {
	//Buffer for rate to string conversion
	BYTE checksum = 0;
	char itoabuff[4];

	//Queue the header in the transmission buffer
	checksum = uart.sendString("$PSRF103,", 9, checksum);
	checksum = uart.sendString(cmdToString(cmd, itoabuff), 2, checksum);
	checksum = uart.sendString(",00,", 4, checksum);
	sprintf(itoabuff, "%02d", rate);
	checksum = uart.sendString(itoabuff, 3, checksum);
	//uart.sendString(itoa((int)rate, itoabuff, 10), 3);
	checksum = uart.sendString(",01*", 4, checksum);
	uart.sendString(itoa(checksum, itoabuff, 16), 2, checksum);
	uart.sendString("\r\n", 2, checksum);
 }

 char * GPS::cmdToString(NMEA::CMD cmd, char * buff3) {
	buff3[0] = '0';
	switch(cmd) {
	case NMEA::GGA_FIX_INFO:
		buff3[1] = '0';
		break;
	case NMEA::GLL_LAT_LON:
		buff3[1] = '1';
		break;
	case NMEA::GSA_OVRL_SATDATA:
		buff3[1] = '2';
		break;
	case NMEA::GSV_DET_SATDATA:
		buff3[1] = '3';
		break;
	case NMEA::RMC_REC_MIN_DATA:
		buff3[1] = '4';
		break;
	case NMEA::VTG_VECT_TRCK:
		buff3[1] = '5';
		break;
	case NMEA::ZDA_DAT_TIM:
		buff3[1] = '8';
	}
	return buff3;
 }

 char GPS::interceptByte() {
	 char c = this->uart.getUARTPort()->DATA;
	 //driver.rcvByte(c);
	 if(c != '\n') 
		buffer[buffptr][windex++] = c;
	 else {
		if (!readlock) {
			 buffer[buffptr][windex] = '\0';
			 buffptr = (buffptr == 0) ? 1 : 0;
			 dataReady = true;
		}
		windex = 0;

	 }
	 return c;
 }

 char * GPS::getMostRecent(char * dest) {
	if (!dataReady) return nullptr;
	readlock = true;
	BYTE rptr = (buffptr == 0) ? 1 : 0;
	strcpy(dest, buffer[buffptr]);
	readlock = false;
	dataReady = false;
	return dest;
 }

 void GPS::handleDRE() {
	 if (!uart.txIsEmpty()) {
		 uart.getUARTPort()->DATA = uart.txDequeue();
	 } else {
		 uart.getUARTPort()->CTRLA &= ~(0x03); //Disable DREint
	 }
 }