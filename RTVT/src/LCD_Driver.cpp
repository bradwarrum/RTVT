/*
 * LCD_Driver.cpp
 *
 * Created: 10/7/2014 10:31:36 PM
 *  Author: ee63pc3-user
 */ 

#include "LCD_Driver.h"

#define TFTWIDTH 320
#define TFTHEIGHT 480

#define TFTLCD_DELAY 0x00

static const uint8_t setupValues[] = {
 /*HX8357_SWRESET, 0,
 HX8357D_SETC, 3, 0xFF, 0x83, 0x57,
 TFTLCD_DELAY, 0,
 HX8357_SETRGB, 4, 0x00, 0x00, 0x06, 0x06,
 HX8357D_SETCOM, 1, 0x25, // -1.52V
 HX8357_SETOSC, 1, 0x68,
 //HX8357_SETOSC, 2, 0x68, 0x01, // Normal mode 70Hz, Idle mode 55 Hz
 HX8357_SETPANEL, 1, 0x05, // BGR, Gate direction swapped
 HX8357_SETPWR1, 6, 0x00, 0x15, 0x1C, 0x1C, 0x83, 0xAA,
 HX8357D_SETSTBA, 6, 0x50, 0x50, 0x01, 0x3C, 0x1E, 0x08,
 //HX8357D_SETGAMMA, 34, 0x02, 0x0A, 0x11, 0x1D, 0x23, 0x35, 0x41, 0x4b, 0x4b, 0x42, 0x3A, 0x27, 0x1B, 0x08, 0x09, 0x03, 0x02, 0x0A, 0x11, 0x1D, 0x23, 0x35, 0x41,0x4b, 0x4b, 0x42, 0x3A, 0x27, 0x1B, 0x08, 0x09, 0x03, 0x00, 0x01,
 // MEME GAMMA HERE
 HX8357D_SETCYC, 7, 0x02, 0x40, 0x00, 0x2A, 0x2A, 0x0D, 0x78,
 HX8357_COLMOD, 1, 0x55,
 HX8357_MADCTL, 1, 0xC0,
 HX8357_TEON, 1, 0x00,
 HX8357_TEARLINE, 2, 0x00, 0x02,
 HX8357_SLPOUT, 0,
 TFTLCD_DELAY, 0,
 HX8357_DISPON, 0,
 TFTLCD_DELAY, 0*/

  HX8357_SWRESET, 0,
  HX8357D_SETC, 3, 0xFF, 0x83, 0x57,
  TFTLCD_DELAY, 250,
  HX8357_SETRGB, 4, 0x00, 0x00, 0x06, 0x06,
  HX8357D_SETCOM, 1, 0x25,  // -1.52V
  HX8357_SETOSC, 1, 0x68,  // Normal mode 70Hz, Idle mode 55 Hz
  HX8357_SETPANEL, 1, 0x05,  // BGR, Gate direction swapped
  HX8357_SETPWR1, 6, 0x00, 0x15, 0x1C, 0x1C, 0x83, 0xAA,
  HX8357D_SETSTBA, 6, 0x50, 0x50, 0x01, 0x3C, 0x1E, 0x08,
  // MEME GAMMA HERE
  HX8357D_SETCYC, 7, 0x02, 0x40, 0x00, 0x2A, 0x2A, 0x0D, 0x78,
  HX8357_COLMOD, 1, 0x55,
  HX8357_MADCTL, 1, 0xC0,
  HX8357_TEON, 1, 0x00,
  HX8357_TEARLINE, 2, 0x00, 0x02,
  HX8357_SLPOUT, 0,
  TFTLCD_DELAY, 150,
  HX8357_DISPON, 0, 
  TFTLCD_DELAY, 50,
};

LCD_Driver::LCD_Driver(void) : Adafruit_GFX(TFTWIDTH, TFTHEIGHT) 
{
	init();
} 

void LCD_Driver::init()
{
	LCD_RST_PORT.DIRSET |= LCD_RST_MASK;
	LCD_RST_SET;
	
	LCD_DATA_PORT.DIRSET = 0xFF;
	LCD_CTRL_PORT.DIRSET |= LCD_RD_MASK;
	LCD_CTRL_PORT.DIRSET |= LCD_WR_MASK;
	LCD_CTRL_PORT.DIRSET |= LCD_CD_MASK;
	LCD_CTRL_PORT.DIRSET |= LCD_CS_MASK;
	
	CS_IDLE;
	WR_IDLE;
	RD_IDLE;
	CD_DATA;
	
	rotation = 0;
	cursor_x = 0;
	cursor_y = 0;
	textsize = 1;
	textcolor = 0x0000;
	_width = TFTWIDTH;
	_height = TFTHEIGHT;
}

void LCD_Driver::begin()
{
	volatile uint8_t i = 0;

	reset();
	_delay_ms(200);
	CS_ACTIVE;

	while(i < sizeof(setupValues)) {
		volatile uint8_t r = (*(const unsigned char* )(&setupValues[i++]));
		volatile uint8_t len = (*(const unsigned char *)(&setupValues[i++]));
		if(r == TFTLCD_DELAY) {
			_delay_ms(300);
		} else {
			 //Serial.print("Register $"); Serial.print(r, HEX);
			 //Serial.print(" datalen "); Serial.println(len);
			 CS_ACTIVE;
			 CD_COMMAND;
			 writeData(r);
			 CD_DATA;
			 for (volatile uint8_t d=0; d<len; d++) {
				 //uint8_t x = pgm_read_byte(&setupValues[i++]);
				 volatile uint8_t x = (*(const unsigned char *)(&setupValues[i++]));
				 writeData(x);
			 }
			 CS_IDLE;
		 }
	 }
	 return;
}

void LCD_Driver::reset()
{
  CS_IDLE;
  WR_IDLE;
  RD_IDLE;

  LCD_RST_CLR;
  _delay_ms(2);
  LCD_RST_SET;  

  CS_ACTIVE;
  CD_COMMAND;
  writeData(0x00);
  WR_STROBE;
  WR_STROBE;
  WR_STROBE;

  CS_IDLE;
}

void LCD_Driver::setRotation(uint8_t x)
{
    Adafruit_GFX::setRotation(x);
	CS_ACTIVE;

	uint8_t t;
	switch (rotation)
	{
	    case 0: t = HX8357B_MADCTL_MX | HX8357B_MADCTL_MY | HX8357B_MADCTL_RGB; break;
		case 1: t = HX8357B_MADCTL_MY | HX8357B_MADCTL_MV | HX8357B_MADCTL_RGB; break;
		case 2: t = HX8357B_MADCTL_RGB; break;
		case 3: t = HX8357B_MADCTL_MX | HX8357B_MADCTL_MV | HX8357B_MADCTL_RGB; break;
	}

	CD_COMMAND;
	writeData(ILI9341_MADCTL);
	CD_DATA;
	writeData(t);
	setAddrWindow(0, 0, _width-1, _height-1);
}

void LCD_Driver::drawPixel(int16_t x, int16_t y, uint16_t color)
{
	if((x < 0) || (y < 0) || (x >= _width) || (y >= _height)) return;
	CS_ACTIVE;
	setAddrWindow(x, y, _width-1, _height-1);
	CS_ACTIVE;
	CD_COMMAND;
	writeData(0x2C);
	CD_DATA;
	writeData(color >> 8);
	writeData(color);
	CS_IDLE;
}

void LCD_Driver::writeData(uint8_t data)
{
	LCD_DATA_PORT.OUT = data;
	WR_STROBE;
}

void LCD_Driver::fillScreen(uint16_t color) {
	setAddrWindow(0,0, _width-1, _height - 1);
	flood(color, (long)TFTWIDTH * (long) TFTHEIGHT);
}

void LCD_Driver::flood(uint16_t color, uint32_t len) {
	uint16_t blocks;
	uint8_t hi = color >> 8, lo = color;
	CS_ACTIVE;
	CD_COMMAND;

	writeData(HX8357_RAMWR);

	CD_DATA;
	writeData(hi);
	writeData(lo);

	if (hi==lo) {
	for (volatile uint32_t i = 0; i<len-1; i++) {
		WR_STROBE;
		WR_STROBE;
		}
	}
	else {
	for (volatile uint32_t i = 0; i< len-1; i++) {
		writeData(hi);
		writeData(lo);
	}
	}

	/* blocks = (uint16_t)(len / 64); // 64 pixels/block
	 if(hi == lo) {
		 // High and low bytes are identical. Leave prior data
		 // on the port(s) and just toggle the write strobe.
		 while(blocks--) {
			 i = 16; // 64 pixels/block / 4 pixels/pass
			 do {
				 WR_STROBE; WR_STROBE; WR_STROBE; WR_STROBE; // 2 bytes/pixel
				 WR_STROBE; WR_STROBE; WR_STROBE; WR_STROBE; // x 4 pixels
			 } while(--i);
		 }
		 // Fill any remaining pixels (1 to 64)
		 for(i = (uint8_t)len & 63; i--; ) {
			 WR_STROBE;
			 WR_STROBE;
		 }
		 } else {
		 while(blocks--) {
			 i = 16; // 64 pixels/block / 4 pixels/pass
			 do {
				 writeData(hi); writeData(lo); writeData(hi); writeData(lo);
				 writeData(hi); writeData(lo); writeData(hi); writeData(lo);
			 } while(--i);
		 }
		 for(i = (uint8_t)len & 63; i--; ) {
			 writeData(hi);
			 writeData(lo);
		 }
	 }*/
	 CS_IDLE;
}

/*
void LCD_Driver::writeCommand(uint8_t cmd) 
{
	CD_COMMAND;
	LCD_DATA_PORT.OUT = cmd;
	WR_STROBE;
}
*/

void LCD_Driver::readData(uint8_t cmd, uint8_t numparams) 
{
	CD_COMMAND;
	LCD_RD_SET;
	writeCommand(cmd);
	CD_DATA;
	LCD_WR_SET;
	_delay_ms(10);
	LCD_DATA_PORT.DIR= 0x00;
	_delay_ms(10);
	for (int i = 0; i < numparams; i++) {
		LCD_RD_CLR;
		_delay_ms(10);
		LCD_RD_SET;
		uint8_t data = LCD_DATA_PORT.IN;
		asm("NOP");
		_delay_ms(10);
		asm("NOP");
	}
	LCD_DATA_PORT.DIR= 0xFF;
}


void LCD_Driver::setAddrWindow(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2)
{
	CS_ACTIVE;
	uint32_t t;

	t = x1;
	t <<= 16;
	t |= x2;
	//TODO move register
	//writeRegister32(ILI9341_COLADDRSET, t);  // HX8357D uses same registers!
	writeRegister32(HX8357_CASET, t);
	t = y1;
	t <<= 16;
	t |= y2;
	//TODO move register
	//writeRegister32(ILI9341_PAGEADDRSET, t); // HX8357D uses same registers!
	writeRegister32(HX8357_PASET, t);
	CS_IDLE;
}

void LCD_Driver::writeRegister32(uint8_t r, uint32_t d) 
{
    CS_ACTIVE;
    CD_COMMAND;
    writeData(r);
    CD_DATA;
    _delay_us(10);
    writeData(d >> 24);
    _delay_us(10);
    writeData(d >> 16);
    _delay_us(10);
    writeData(d >> 8);
    _delay_us(10);
    writeData(d);
    CS_IDLE;
}

void LCD_Driver::writeStr(char * str) {
	while(*str) {
		this->write(*str++);
	}

}
