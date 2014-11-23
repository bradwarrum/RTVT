/* 
* OBDConversion.cpp
*
* Created: 11/22/2014 8:10:00 PM
* Author: ee63pc3-user
*/


#include "OBDConversion.h"

uint8_t OBDConversion::hex(char c)
{
    if (c >= 48 && c <= 57) return c - 48;
	else return c - 55;
}

uint8_t OBDConversion::speed(char au, char al)
{
	uint16_t val = (((uint16_t)(16 * hex(au) + hex(al))) * 62 / 100);
	return val ? val + 1 : 0;
}

uint16_t OBDConversion::rpm(char au, char al, char bu, char bl)
{
    return ((uint16_t)((uint16_t)256 * ((uint16_t)(16 * hex(au) + hex(al))) + ((uint16_t)(16 * hex(bu) + hex(bl)))) / (uint16_t)4);
}

uint8_t OBDConversion::engine_load(char au, char al) {
	return 0;
}

uint8_t OBDConversion::throttle_pos(char au, char al) {
	return 0;
}