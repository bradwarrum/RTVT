/*
 * GPS.cpp
 *
 * Created: 11/5/2014 3:17:12 PM
 *  Author: ee63pc3-user
 */ 
 #include "GPS.h"

 GPS::GPS() : uart(220, 80) {
 	uart.init(&USARTD1, ParityMode::DISABLED, CharSize::EIGHT, false, 12, 0x50);//4800 baud
	status_reg = 0;
	for (uint8_t i = 0; i < 59; i++) buffer[i] = '\0';
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
	setPeriodic(NMEA::RMC_REC_MIN_DATA, 1);
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

 void GPS::interceptByte() {
	 char c = this->uart.getUARTPort()->DATA;
	 if (allowRcv) {
	 uart.rcvByte(c);
	 if(c == '\n') {
		dataReady++;
	 }
	 }
 }

void GPS::updateRegisters() {
	if (!dataReady) return;
	while (dataReady > 1) {
		while (uart.rxDequeue() != '\n');
		asm("cli");
		dataReady--;
		asm("sei");
	}
	char c;
	uint8_t wptr;
	while((c = uart.rxDequeue()) != ',');
	for(uint8_t cmd = 0; cmd < 10; cmd++) {
		wptr = offsets[cmd];
		while ((c = uart.rxDequeue()) != ',') {
			if (buffer[wptr] != c) status_reg |= (1 << cmd);
			buffer[wptr++] = c;
		}
		while (wptr < offsets[cmd +1]) {
			if (buffer[wptr] != '\0') status_reg |= (1 << cmd);
			buffer[wptr++] = '\0';
		}
	}
	while (uart.rxDequeue() != '\n');

	asm("cli");
	dataReady--;
	asm("sei");
}
uint16_t GPS::getStatus() {
	return status_reg;
}
void GPS::clearStatus() {
	status_reg = 0;
	};

void GPS::vomit(LCD_Driver * lcd) {
	if (!status_reg) return;
	status_reg = 0;
	lcd->writeStr("Lat: ");
	uint8_t rptr = offsets[GPSVAL::LAT];
	for (uint8_t i = offsets[GPSVAL::LAT]; i<offsets[GPSVAL::LAT] + sizes[GPSVAL::LAT]; i++) {
		lcd->write(buffer[i]);
	}
	lcd->write(' ');
	for (uint8_t i = offsets[GPSVAL::LONG]; i<offsets[GPSVAL::LONG] + sizes[GPSVAL::LONG]; i++) {
			lcd->write(buffer[i]);
	}
	lcd->write('\n');
	/*while(!uart.rxIsEmpty()) {
		char c = uart.rxDequeue();
		lcd->write(c);
		
	}*/

}

char * GPS::retrieve(GPSVAL::VAL value, char * b) {
	uint8_t i,y;
	y = 0;
	for (i = offsets[value]; i<offsets[value] + sizes[value]; i++, y++) {
		b[y] = buffer[i];
		if (buffer[i] == '\0') return b;
	}
	b[y] = '\0';
	return b;
}

 void GPS::handleDRE() {
	 if (!uart.txIsEmpty()) {
		 uart.getUARTPort()->DATA = uart.txDequeue();
	 } else {
		 uart.getUARTPort()->CTRLA &= ~(0x03); //Disable DREint
	 }
 }
 
 void GPS::startReceiving() {
	 allowRcv = true;
 }