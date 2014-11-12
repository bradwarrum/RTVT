/*
 * LCD_Driver.h
 *
 * Created: 10/7/2014 10:37:54 PM
 *  Author: ee63pc3-user
 */ 

#ifndef F_CPU
#define F_CPU 2000000UL
#endif
#include "Adafruit_GFX.h"
#include "registers.h"
#include "macro.h"
#include <util/delay.h>
#include <avr/pgmspace.h>

#ifndef LCD_DRIVER_H_
#define LCD_DRIVER_H_

class LCD_Driver : public Adafruit_GFX{
	public:
	
	LCD_Driver();
	void init();
	void begin();
	void reset();
	void drawPixel(int16_t x, int16_t y, uint16_t color);
	void writeData(uint8_t data);
	void writeCommand(uint8_t cmd);
	void readData(uint8_t cmd, uint8_t numparams);
	void setAddrWindow(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2);
	void writeRegister32(uint8_t r, uint32_t d);
    void fillScreen(uint16_t color);
	void flood(uint16_t color, uint32_t len);
	};



#endif /* LCD_DRIVER_H_ */