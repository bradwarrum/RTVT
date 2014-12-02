/* 
* BTOBD.cpp
*
* Created: 10/27/2014 6:36:08 PM
* Author: ee63pc3-user
*/


#include "BTOBD.h"
#include "OBDConversion.h"

// default constructor
BTOBD::BTOBD() : driver(128, 128)
{
	//driver.init(&USARTD0, ParityMode::DISABLED, CharSize::EIGHT, false, 12, 0x40); // 9600 bps
	driver.init(&USARTD0, ParityMode::DISABLED, CharSize::EIGHT, false, 33, 0xF0); // 115200 bps
	responses = 0;
	status_reg = 0;
	cmdcount = 0;
	cmdlock = -1;

	for (int i = 0; i < 36; i++) buffer[i] = '0';

} //BTOBD

void BTOBD::dump(FIL * fp) {
	driver.dump(fp);
}
char BTOBD::interceptByte() {
	char c = driver.getUARTPort()->DATA;
	driver.rcvByte(c);
	if (c == '>') this->responses ++;
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
    int count = 0;

	sendCmd(OBDCMDS::ENTERCMDMODE);
	waitForNewline(LCD);
	sendCmd(OBDCMDS::FACTORY_RESET);
	waitForNewline(LCD);
	sendCmd(OBDCMDS::REBOOT);
	_delay_ms(3000);
	while(!driver.rxIsEmpty()) {
			char c = driver.rxDequeue();
			LCD->write(c);		
	}
	sendCmd(OBDCMDS::ENTERCMDMODE);
	waitForNewline(LCD);
	/*sendCmd(OBDCMDS::INQUIRY);
	for (int i = 0; i < 100; i++) {
		_delay_ms(300);
		while(!driver.rxIsEmpty()) {
			char c = driver.rxDequeue();
			LCD->write(c);
		}
	}*/
	do {
		LCD->fillScreen(BLACK);
		LCD->setCursor(0,0);
		count = 0;
		sendCmd(OBDCMDS::CONNECT);
		for (int i = 0; i<100; i++) {
			_delay_ms(150);
			while(!driver.rxIsEmpty()) {
				char c = driver.rxDequeue();
				LCD->write(c);
				if (c == '\n') count++;
			}
		}
	} while (count != 1);
	LCD->writeStr("Connected\n");	

	//sendCmd(OBDCMDS::ELM_ATI);
	//waitForNewline(LCD);
	sendCmd(OBDCMDS::ELM_ATZ);
	waitForPrompt(LCD);
	sendCmd(OBDCMDS::ELM_ATE0);
	waitForPrompt(LCD);
	sendCmd(OBDCMDS::ELM_ATL1);
	waitForPrompt(LCD);
	sendCmd(OBDCMDS::ELM_ATH0);
	waitForPrompt(LCD);
	//sendCmd(OBDCMDS::ELM_ATS1);
	//waitForNewline(LCD);
	sendCmd(OBDCMDS::ELM_ATSP0);
	waitForPrompt(LCD);
	responses = 0;
						sendCmd(OBDCMDS::RPM);
						waitForPrompt(LCD);
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
		asm("NOP");
	}
}

void BTOBD::waitForPrompt(LCD_Driver * LCD) {
	while(1) {
		if (!driver.rxIsEmpty()) {
			char c= driver.rxDequeue();
			LCD->write(c);
			if (c == '>') {
				//LCD->write('\n');
				break;
			}
		}
	}
}

void BTOBD::sendCmd(OBDCMDS::CMD cmd) {
	this->currcmd = cmd;

	switch (cmd) {
		case OBDCMDS::ENTERCMDMODE:
			driver.sendByte('$');
			driver.sendByte('$');
			driver.sendByte('$'); break;
		case OBDCMDS::EXITCMDMODE:
			//driver.sendByte('\n');
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
			/*driver.sendByte('0');
			driver.sendByte('0');
			driver.sendByte('1');
			driver.sendByte('9');
			driver.sendByte('5');
			driver.sendByte('D');
			driver.sendByte('E');
			driver.sendByte('8');
			driver.sendByte('0');
			driver.sendByte('5');
			driver.sendByte('7');
			driver.sendByte('A');*/
			driver.sendByte('\n'); break;
		case OBDCMDS::INQUIRY:
			driver.sendByte('I');
			driver.sendByte('\n'); break;
		case OBDCMDS::GETSTATUS:
			driver.sendByte('G');
			driver.sendByte('K');
			driver.sendByte('\n'); break;
		case OBDCMDS::FACTORY_RESET:
			driver.sendByte('S');
			driver.sendByte('F');
			driver.sendByte(',');
			driver.sendByte('1');
			driver.sendByte('\n'); break;
		case OBDCMDS::REBOOT:
			driver.sendByte('R');
			driver.sendByte(',');
			driver.sendByte('1');
			driver.sendByte('\n');
		case OBDCMDS::ELM_ATI:
			driver.sendByte('A');
			driver.sendByte('T');
			driver.sendByte('I');
			driver.sendByte('\r');
			driver.sendByte('\n'); break;
		case OBDCMDS::ELM_ATZ:
			driver.sendByte('A');
			driver.sendByte('T');
			driver.sendByte('Z');
			driver.sendByte('\r');
			driver.sendByte('\n'); break;
		case OBDCMDS::ELM_ATE0:
			driver.sendByte('A');
			driver.sendByte('T');
			driver.sendByte('E');
			driver.sendByte('0');
			driver.sendByte('\r');
			driver.sendByte('\n'); break;
		case OBDCMDS::ELM_ATL1:
			driver.sendByte('A');
			driver.sendByte('T');
			driver.sendByte('L');
			driver.sendByte('1');
			driver.sendByte('\r');
			driver.sendByte('\n'); break;
		case OBDCMDS::ELM_ATH0:
			driver.sendByte('A');
			driver.sendByte('T');
			driver.sendByte('H');
			driver.sendByte('0');
			driver.sendByte('\r');
			driver.sendByte('\n'); break;
		case OBDCMDS::ELM_ATS1:
			driver.sendByte('A');
			driver.sendByte('T');
			driver.sendByte('S');
			driver.sendByte('1');
			driver.sendByte('\r');
			driver.sendByte('\n'); break;
		case OBDCMDS::ELM_ATSP0:
			driver.sendByte('A');
			driver.sendByte('T');
			driver.sendByte('S');
			driver.sendByte('P');
			driver.sendByte('0');
			driver.sendByte('\r');
			driver.sendByte('\n'); break;
		case OBDCMDS::RPM:
			driver.sendByte('0');
			driver.sendByte('1');
			driver.sendByte('0');
			driver.sendByte('C');
			driver.sendByte('\r');
			driver.sendByte('\n');
			this->currupper = '0';
			this->currlower = 'C'; 
			break;
		case OBDCMDS::SPEED:
			driver.sendByte('0');
			driver.sendByte('1');
			driver.sendByte('0');
			driver.sendByte('D');
			driver.sendByte('\r');
			driver.sendByte('\n');
			this->currupper = '0';
			this->currlower = 'D';
			break;
		case OBDCMDS::THROTTLE_POS:
			driver.sendByte('0');
			driver.sendByte('1');
			driver.sendByte('1');
			driver.sendByte('1');
			driver.sendByte('\r');
			driver.sendByte('\n');
			this->currupper = '1';
			this->currlower = '1';
			break;
		case OBDCMDS::OIL_TEMP:
			driver.sendByte('0');
			driver.sendByte('1');
			driver.sendByte('5');
			driver.sendByte('C');
			driver.sendByte('\r');
			driver.sendByte('\n');
			this->currupper = '5';
			this->currlower = 'C'; 
			break;
		case OBDCMDS::FUEL_RATE:
			driver.sendByte('0');
			driver.sendByte('1');
			driver.sendByte('5');
			driver.sendByte('E');
			driver.sendByte('\r');
			driver.sendByte('\n'); 
			this->currupper = '5';
			this->currlower = 'E';
			break;
		case OBDCMDS::COOLANT_TEMP:
			driver.sendByte('0');
			driver.sendByte('1');
			driver.sendByte('6');
			driver.sendByte('7');
			driver.sendByte('\r');
			driver.sendByte('\n'); 
			this->currupper = '6';
			this->currlower = '7';
			break;
		case OBDCMDS::ENGINE_LOAD:
			driver.sendByte('0');
			driver.sendByte('1');
			driver.sendByte('0');
			driver.sendByte('4');
			driver.sendByte('\r');
			driver.sendByte('\n'); 
			this->currupper = '0';
			this->currlower = '4';
			break;
		case OBDCMDS::FUEL_LEVEL:
			driver.sendByte('0');
			driver.sendByte('1');
			driver.sendByte('2');
			driver.sendByte('F');
			driver.sendByte('\r');
			driver.sendByte('\n'); 
			this->currupper = '2';
			this->currlower = 'F';
			break;
		case OBDCMDS::MANIFOLD_AIR_PRESSURE:
			driver.sendByte('0');
			driver.sendByte('1');
			driver.sendByte('8');
			driver.sendByte('7');
			driver.sendByte('\r');
			driver.sendByte('\n'); 
			this->currupper = '8';
			this->currlower = '7';
			break;			
		case OBDCMDS::AMBIENT_TEMP:
			driver.sendByte('0');
			driver.sendByte('1');
			driver.sendByte('4');
			driver.sendByte('6');
			driver.sendByte('\r');
			driver.sendByte('\n'); 
			this->currupper = '4';
			this->currlower = '6';
			break;	
		case OBDCMDS::BAROMETRIC_PRESSURE:
			driver.sendByte('0');
			driver.sendByte('1');
			driver.sendByte('3');
			driver.sendByte('3');
			driver.sendByte('\r');
			driver.sendByte('\n'); 
			this->currupper = '3';
			this->currlower = '3';
			break;	
		case OBDCMDS::FUEL_RAIL_PRESSURE:
			driver.sendByte('0');
			driver.sendByte('1');
			driver.sendByte('5');
			driver.sendByte('9');
			driver.sendByte('\r');
			driver.sendByte('\n'); 
			this->currupper = '5';
			this->currlower = '9';
			break;	
		case OBDCMDS::INTAKE_AIR_TEMP:
			driver.sendByte('0');
			driver.sendByte('1');
			driver.sendByte('6');
			driver.sendByte('8');
			driver.sendByte('\r');
			driver.sendByte('\n');
			this->currupper = '6';
			this->currlower = '8';
			break;	
		case OBDCMDS::PID_SUPPORT_1_20:
			driver.sendByte('0');
			driver.sendByte('1');
			driver.sendByte('0');
			driver.sendByte('0');
			driver.sendByte('\r');
			driver.sendByte('\n'); 
			this->currupper = '0';
			this->currlower = '0';
			break;	
		case OBDCMDS::PID_SUPPORT_21_40:
			driver.sendByte('0');
			driver.sendByte('1');
			driver.sendByte('2');
			driver.sendByte('0');
			driver.sendByte('\r');
			driver.sendByte('\n'); 
			this->currupper = '2';
			this->currlower = '0';
			break;	
		case OBDCMDS::PID_SUPPORT_41_60:
			driver.sendByte('0');
			driver.sendByte('1');
			driver.sendByte('4');
			driver.sendByte('0');
			driver.sendByte('\r');
			driver.sendByte('\n'); 
			this->currupper = '4';
			this->currlower = '0';
			break;	
		case OBDCMDS::PID_SUPPORT_61_80:
			driver.sendByte('0');
			driver.sendByte('1');
			driver.sendByte('6');
			driver.sendByte('0');
			driver.sendByte('\r');
			driver.sendByte('\n'); 
			this->currupper = '6';
			this->currlower = '0';
			break;	
		case OBDCMDS::PID_SUPPORT_81_A0:
			driver.sendByte('0');
			driver.sendByte('1');
			driver.sendByte('8');
			driver.sendByte('0');
			driver.sendByte('\r');
			driver.sendByte('\n');
			this->currupper = '8';
			this->currlower = '0';	
	}
}

bool BTOBD::rcvResp()
{
    bool rcv = false;

	while (responses)
	{
	    char upper, lower;
		uint8_t same = 0;

	    driver.rxDequeue();
		driver.rxDequeue();
		driver.rxDequeue();
		upper = driver.rxDequeue();
		lower = driver.rxDequeue();
		driver.rxDequeue();

		if (upper != this->currupper || lower != this->currlower) 
		{
		    while(driver.rxDequeue() != '>' && !driver.rxIsEmpty());
		}
		else if (cmdlock != (int8_t)currcmd)
		{
		    for (int i = offsets[currcmd]; i < offsets[currcmd] + sizes[currcmd]; i++)
		    {
			    char c = driver.rxDequeue();
				if (c == ' ') { i--; continue; }
				
				if (buffer[i] == c) same++;
			    else buffer[i] = c;
		    }
			while(driver.rxDequeue() != '>' && !driver.rxIsEmpty());
			if (same != sizes[currcmd]) this->status_reg |= (1 << currcmd);
		}

		asm("cli");
		responses--;
		asm("sei");
		rcv = true;
	}

	return rcv;
}

uint16_t BTOBD::getStatus() { return this->status_reg; }

void BTOBD::clearStatus() { this->status_reg = 0; }

void BTOBD::vomit(LCD_Driver * LCD)
{
    for (int i = 0; i < 32; i++) LCD->write(buffer[i]);
	LCD->write('\n');
	char buf[12];
	LCD->writeStr(itoa(OBDConversion::speed(buffer[0], buffer[1]), buf, 10));
	LCD->write('\n');
	LCD->writeStr(itoa(OBDConversion::rpm(buffer[2], buffer[3], buffer[4], buffer[5]), buf, 10));
	LCD->write('\n');
}

void BTOBD::sendCmd(FIL * fp)
{
	//FIX
	char c[4];
    sendCmd(this->cmdorder[this->cmdcount]);
	f_puts("Sent cmd ", fp);
	f_puts(itoa(this->cmdcount, c, 10), fp);
	f_putc('\n', fp);
	if (++cmdcount == 4) cmdcount = 0;
}

bool BTOBD::rxIsEmpty() {
	return driver.rxIsEmpty();
}

char BTOBD::rxDequeue() {
	return driver.rxDequeue();
}

CBUFFER BTOBD::getRecent(OBDCMDS::CMD cmd) {
	CBUFFER returnval;
	uint8_t cint = offsets[cmd];
	cmdlock = (int8_t) cmd;
	returnval.au = buffer[cint++];
	returnval.al = buffer[cint++];
	if (sizes[cmd] == 4) {
		returnval.bu = buffer[cint++];
		returnval.bl = buffer[cint++];
	}
	cmdlock = -1;
	return returnval;
}