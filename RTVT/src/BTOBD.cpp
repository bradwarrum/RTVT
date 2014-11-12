/* 
* BTOBD.cpp
*
* Created: 10/27/2014 6:36:08 PM
* Author: ee63pc3-user
*/


#include "BTOBD.h"

// default constructor
BTOBD::BTOBD() : driver(5, 128)
{
	//driver.init(&USARTD0, ParityMode::DISABLED, CharSize::EIGHT, false, 12, 0x40); // 9600 bps
	driver.init(&USARTD0, ParityMode::DISABLED, CharSize::EIGHT, false, 33, 0xF0); // 115200 bps
	inCMDMode = false;
	connected = false;
	status_reg=0;
	/*
	cmd_resp_size[0] = 5;
	cmd_resp_size[1] = 15;
	cmd_resp_size[2] = 3;
	cmd_resp_size[3] = 5;*/

} //BTOBD
char BTOBD::interceptByte() {
	char c = driver.getUARTPort()->DATA;
	driver.rcvByte(c);
	return c;
}

void BTOBD::handleDRE() {
	if (!driver.txIsEmpty()) {
		driver.getUARTPort()->DATA = driver.txDequeue();
		} else {
		driver.getUARTPort()->CTRLA &= ~(0x03); //Disable DREint
	}
}

bool BTOBD::initialize(LCD_Driver * LCD) {
	sendCmd(OBDCMDS::ENTERCMDMODE);
	waitForNewline(LCD);
	/*
	sendCmd(OBDCMDS::INQUIRY);
	for (int i = 0; i<200; i++) {
		_delay_ms(100);
		while(!driver.rxIsEmpty()) {
			LCD->write(driver.rxDequeue());
		}
	}
	*/
	sendCmd(OBDCMDS::CONNECT);
	for (int i = 0; i<100; i++) {
		_delay_ms(100);
		while(!driver.rxIsEmpty()) {
			LCD->write(driver.rxDequeue());
		}
	}
	sendCmd(OBDCMDS::ENTERCMDMODE);
	waitForNewline(LCD);
	sendCmd(OBDCMDS::GETSTATUS);
	waitForNewline(LCD);
	sendCmd(OBDCMDS::EXITCMDMODE);
	waitForNewline(LCD);
	/*
	sendCmd(OBDCMDS::EXITCMDMODE);
	while (1) 
	{
	    if (!driver.rxIsEmpty())
		{
	        char c = driver.rxDequeue();
		    LCD->write(c);
		    if (c == '\n') break;
		}
	}
	*/
	sendCmd(OBDCMDS::ELM_ATI);
	waitForNewline(LCD);
	sendCmd(OBDCMDS::ELM_ATZ);
	waitForNewline(LCD);
	sendCmd(OBDCMDS::ELM_ATL1);
	waitForNewline(LCD);
	sendCmd(OBDCMDS::ELM_ATH0);
	waitForNewline(LCD);
	sendCmd(OBDCMDS::ELM_ATS1);
	waitForNewline(LCD);
	sendCmd(OBDCMDS::ELM_ATSP0);
	waitForNewline(LCD);
	asm("NOP");
	return true;
}
bool BTOBD::initialize(char * address, LCD_Driver * LCD) {
	for (int i = 0; i < 12; i++) {
		bt_addr[i] = address[i];
	}
	return this->initialize(LCD);
}
void BTOBD::waitForNewline(LCD_Driver * LCD) {
	while (1)
	{
		if (!driver.rxIsEmpty())
		{
			char c = driver.rxDequeue();
			LCD->write(c);
			if (c == '\n') break;
		}
	}
}

void BTOBD::sendCmd(OBDCMDS::CMD cmd) {
	//current_resp_size = 0;
	//expected_resp_size = cmd_resp_size[cmd];
	switch (cmd) {
		case OBDCMDS::ENTERCMDMODE:
			driver.sendByte('$');
			driver.sendByte('$');
			driver.sendByte('$'); break;
		case OBDCMDS::EXITCMDMODE:
			driver.sendByte('-');
			driver.sendByte('-');
			driver.sendByte('-');
			driver.sendByte('\n'); break;
		case OBDCMDS::CONNECT:
			driver.sendByte('C');
			driver.sendByte(',');
			for (int i = 0; i < 12; i++) {
				driver.sendByte(bt_addr[i]);
			}
			driver.sendByte('\n'); break;
		case OBDCMDS::INQUIRY:
			driver.sendByte('I');
			driver.sendByte('\n'); break;
		case OBDCMDS::GETSTATUS:
			driver.sendByte('G');
			driver.sendByte('K');
			driver.sendByte('\n'); break;
		case OBDCMDS::ELM_ATI:
			driver.sendByte('A');
			driver.sendByte('T');
			driver.sendByte('I');
			driver.sendByte('\n'); break;
		case OBDCMDS::ELM_ATZ:
			driver.sendByte('A');
			driver.sendByte('T');
			driver.sendByte('Z');
			driver.sendByte('\n'); break;
		case OBDCMDS::ELM_ATL1:
			driver.sendByte('A');
			driver.sendByte('T');
			driver.sendByte('L');
			driver.sendByte('1');
			driver.sendByte('\n'); break;
		case OBDCMDS::ELM_ATH0:
			driver.sendByte('A');
			driver.sendByte('T');
			driver.sendByte('H');
			driver.sendByte('0');
			driver.sendByte('\n'); break;
		case OBDCMDS::ELM_ATS1:
			driver.sendByte('A');
			driver.sendByte('T');
			driver.sendByte('S');
			driver.sendByte('1');
			driver.sendByte('\n'); break;
		case OBDCMDS::ELM_ATSP0:
			driver.sendByte('A');
			driver.sendByte('T');
			driver.sendByte('S');
			driver.sendByte('P');
			driver.sendByte('0');
			driver.sendByte('\n'); break;
		case OBDCMDS::RPM:
			driver.sendByte('0');
			driver.sendByte('1');
			driver.sendByte('0');
			driver.sendByte('c');
			driver.sendByte('\n'); break;
		case OBDCMDS::SPEED:
			driver.sendByte('0');
			driver.sendByte('1');
			driver.sendByte('0');
			driver.sendByte('d');
			driver.sendByte('\n'); break;
		case OBDCMDS::THROTTLE_POS:
			driver.sendByte('0');
			driver.sendByte('1');
			driver.sendByte('1');
			driver.sendByte('1');
			driver.sendByte('\n'); break;
		case OBDCMDS::OIL_TEMP:
			driver.sendByte('0');
			driver.sendByte('1');
			driver.sendByte('5');
			driver.sendByte('C');
			driver.sendByte('\n'); break;
		case OBDCMDS::FUEL_RATE:
			driver.sendByte('0');
			driver.sendByte('1');
			driver.sendByte('5');
			driver.sendByte('E');
			driver.sendByte('\n'); break;
		case OBDCMDS::COOLANT_TEMP:
			driver.sendByte('0');
			driver.sendByte('1');
			driver.sendByte('6');
			driver.sendByte('7');
			driver.sendByte('\n'); break;
		case OBDCMDS::ENGINE_LOAD:
			driver.sendByte('0');
			driver.sendByte('1');
			driver.sendByte('0');
			driver.sendByte('4');
			driver.sendByte('\n'); break;
		case OBDCMDS::FUEL_LEVEL:
			driver.sendByte('0');
			driver.sendByte('1');
			driver.sendByte('2');
			driver.sendByte('F');
			driver.sendByte('\n'); break;
		case OBDCMDS::MANIFOLD_AIR_PRESSURE:
			driver.sendByte('0');
			driver.sendByte('1');
			driver.sendByte('8');
			driver.sendByte('7');
			driver.sendByte('\n'); break;
		case OBDCMDS::MANIFOLD_SURFACE_TEMP:
			driver.sendByte('0');
			driver.sendByte('1');
			driver.sendByte('8');
			driver.sendByte('4');
			driver.sendByte('\n'); break;			
		case OBDCMDS::AMBIENT_TEMP:
			driver.sendByte('0');
			driver.sendByte('1');
			driver.sendByte('4');
			driver.sendByte('6');
			driver.sendByte('\n'); break;	
		case OBDCMDS::BAROMETRIC_PRESSURE:
			driver.sendByte('0');
			driver.sendByte('1');
			driver.sendByte('3');
			driver.sendByte('3');
			driver.sendByte('\n'); break;	
		case OBDCMDS::FUEL_RAIL_PRESSURE:
			driver.sendByte('0');
			driver.sendByte('1');
			driver.sendByte('5');
			driver.sendByte('9');
			driver.sendByte('\n'); break;	
		case OBDCMDS::INTAKE_AIR_TEMP:
			driver.sendByte('0');
			driver.sendByte('1');
			driver.sendByte('6');
			driver.sendByte('8');
			driver.sendByte('\n'); break;	
		case OBDCMDS::PID_SUPPORT_1_20:
			driver.sendByte('0');
			driver.sendByte('1');
			driver.sendByte('0');
			driver.sendByte('0');
			driver.sendByte('\n'); break;	
		case OBDCMDS::PID_SUPPORT_21_40:
			driver.sendByte('0');
			driver.sendByte('1');
			driver.sendByte('2');
			driver.sendByte('0');
			driver.sendByte('\n'); break;	
		case OBDCMDS::PID_SUPPORT_41_60:
			driver.sendByte('0');
			driver.sendByte('1');
			driver.sendByte('4');
			driver.sendByte('0');
			driver.sendByte('\n'); break;	
		case OBDCMDS::PID_SUPPORT_61_80:
			driver.sendByte('0');
			driver.sendByte('1');
			driver.sendByte('6');
			driver.sendByte('0');
			driver.sendByte('\n'); break;	
		case OBDCMDS::PID_SUPPORT_81_A0:
			driver.sendByte('0');
			driver.sendByte('1');
			driver.sendByte('8');
			driver.sendByte('0');
			driver.sendByte('\n');	
	}
}

bool BTOBD::rxIsEmpty() {
	return driver.rxIsEmpty();
}

char BTOBD::rxDequeue() {
	return driver.rxDequeue();
}