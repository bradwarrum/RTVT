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
#include "macro.h"
#define NUMCMDS 4
namespace OBDCMDS {
	enum CMD {
		ENTERCMDMODE,
		CONNECT,
		INQUIRY,
		GETSTATUS,
		EXITCMDMODE,
		ELM_ATI,
		ELM_ATZ,
		ELM_ATL1,
		ELM_ATH0,
		ELM_ATS1,
		ELM_ATSP0,
		SPEED,
		RPM,
		THROTTLE_POS,
		OIL_TEMP,
		FUEL_RATE,
		COOLANT_TEMP,
		ENGINE_LOAD,
		FUEL_LEVEL,
		MANIFOLD_AIR_PRESSURE,
		MANIFOLD_SURFACE_TEMP,
		AMBIENT_TEMP,
		BAROMETRIC_PRESSURE,
		FUEL_RAIL_PRESSURE,
		INTAKE_AIR_TEMP,
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
	bool inCMDMode;
	bool connected;
	uint8_t cmd_resp_size[NUMCMDS];
	uint8_t expected_resp_size, current_resp_size;
	uint8_t status_reg;
	void sendCmd(OBDCMDS::CMD cmd);
	void waitForNewline(LCD_Driver * LCD);


public:
	BTOBD();
	char interceptByte();
	bool initialize(LCD_Driver * LCD);
	bool initialize(char * address, LCD_Driver * LCD);
	void handleDRE();
	bool rxIsEmpty();
	char rxDequeue();
}; //BTOBD

#endif //__BTOBD_H__
