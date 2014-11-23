/* 
* OBDConversion.h
*
* Created: 11/22/2014 8:10:00 PM
* Author: ee63pc3-user
*/


#ifndef __OBDCONVERSION_H__
#define __OBDCONVERSION_H__
#include <stdint.h>


class OBDConversion
{

public:
	static uint8_t speed(char au, char al);
	static uint16_t rpm(char au, char al, char bu, char bl);
	static uint8_t throttle_pos(char au, char al);
	static int8_t oil_temp(char au, char al);
	static uint16_t fuel_rate(char au, char al, char bu, char bl);
	static int8_t coolant_temp(char au, char al);
	static uint8_t engine_load(char au, char al);
	static uint8_t fuel_level(char au, char al);
	static uint8_t manifold_air_pressure(char au, char al);
	static int8_t ambient_temp(char au, char al);
	static uint8_t barometeric_pressure(char au, char al);
	static uint32_t fuel_rail_pressure(char au, char al, char bu, char bl);\
	static int8_t intake_air_temp(char au, char al);
	static uint8_t hex(char c);
};

#endif //__OBDCONVERSION_H__
