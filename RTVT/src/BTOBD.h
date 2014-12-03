/* 
* BTOBD.h
*
* Created: 10/27/2014 6:36:08 PM
* Author: ee63pc3-user
*/

#ifndef F_CPU
#define F_CPU 32000000UL
#endif
#ifndef __BTOBD_H__
#define __BTOBD_H__
#include "uart_driver.h"
#include "LCD_Driver.h"
#include <util/delay.h>
#include <avr/io.h>
#include "macro.h"

typedef struct {
	char au;
	char al;
	char bu;
	char bl;
} CBUFFER;

namespace OBDCMDS {
	enum CMD {
	    SPEED,
	    RPM,
	    THROTTLE_POS,
	    OIL_TEMP,
	    FUEL_RATE,
	    COOLANT_TEMP,
	    ENGINE_LOAD,
	    FUEL_LEVEL,
	    MANIFOLD_AIR_PRESSURE,
	    AMBIENT_TEMP,
	    BAROMETRIC_PRESSURE,
	    FUEL_RAIL_PRESSURE,
	    INTAKE_AIR_TEMP,
		ENTERCMDMODE,
		CONNECT,
		INQUIRY,
		GETSTATUS,
		FACTORY_RESET,
		REBOOT,
		EXITCMDMODE,
		ELM_ATI,
		ELM_ATZ,
		ELM_ATE0,
		ELM_ATL1,
		ELM_ATH0,
		ELM_ATS1,
		ELM_ATSP0,
		PID_SUPPORT_1_20,
		PID_SUPPORT_21_40,
		PID_SUPPORT_41_60,
		PID_SUPPORT_61_80,
		PID_SUPPORT_81_A0
	};
}


class BTOBD
{
private: 
	UartDriver driver;
	char bt_addr[12];
	uint16_t status_reg;
	uint8_t sizes[13] =   {2, 4, 2, 2, 4, 2, 2, 2, 2, 2, 2, 4, 2};
	uint8_t offsets[13] = {0, 2, 6, 8,10,14,16,18,20,22,24,26,30};
	OBDCMDS::CMD cmdorder[4] = {OBDCMDS::SPEED, OBDCMDS::RPM, OBDCMDS::THROTTLE_POS, OBDCMDS::ENGINE_LOAD}; /*, OBDCMDS::OIL_TEMP, OBDCMDS::FUEL_RATE, 
	                             OBDCMDS::COOLANT_TEMP, OBDCMDS::FUEL_LEVEL, OBDCMDS::MANIFOLD_AIR_PRESSURE,
								 OBDCMDS::AMBIENT_TEMP, OBDCMDS::BAROMETRIC_PRESSURE, OBDCMDS::FUEL_RAIL_PRESSURE, 
								 OBDCMDS::INTAKE_AIR_TEMP};*/
	char buffer[32];
	volatile uint8_t cmdlock;
	OBDCMDS::CMD currcmd;
	char currupper, currlower;
	volatile uint8_t responses;
	int8_t cmdcount;
	void waitForNewline(LCD_Driver * LCD);


public:
	BTOBD();
	char interceptByte();
	bool initialize(LCD_Driver * LCD);
	bool initialize(char * address, LCD_Driver * LCD);
	void handleDRE();
	bool rxIsEmpty();
	char rxDequeue();
	void sendCmd();
	void sendCmd(OBDCMDS::CMD cmd);
	bool rcvResp();
	uint16_t getStatus();
	void clearStatus();
	void vomit(LCD_Driver * LCD);
	void dump(FIL * fp);
	CBUFFER getRecent(OBDCMDS::CMD cmd);
		void waitForPrompt(LCD_Driver * LCD);
}; //BTOBD

#endif //__BTOBD_H__
