

//--------------------------------------------------------------------------//
// HX8357 Driver Registers
//--------------------------------------------------------------------------//
#define HX8357_EXIT_SLEEP_MODE          	0x11
#define HX8357_SET_DISPLAY_OFF          	0x28
#define HX8357_SET_DISPLAY_ON           	0x29
#define HX8357_SET_COLUMN_ADDRESS       	0x2A
#define HX8357_SET_PAGE_ADDRESS         	0x2B
#define HX8357_WRITE_MEMORY_START       	0x2C
#define HX8357_READ_MEMORY_START        	0x2E
#define HX8357_SET_TEAR_ON              	0x35
#define HX8357_SET_ADDRESS_MODE         	0x36
#define HX8357_SET_PIXEL_FORMAT         	0x3A
#define HX8357_WRITE_MEMORY_CONTINUE    	0x3C
#define HX8357_READ_MEMORY_CONTINUE     	0x3E
#define HX8357_SET_INTERNAL_OSCILLATOR   	0xB0
#define HX8357_SET_POWER_CONTROL         	0xB1
#define HX8357_SET_DISPLAY_MODE          	0xB4
#define HX8357_SET_VCOM_VOLTAGE         	0xB6
#define HX8357_ENABLE_EXTENSION_COMMAND 	0xB9
#define HX8357_SET_PANEL_DRIVING        	0xC0    // not documented!
#define HX8357_SET_PANEL_CHARACTERISTIC  	0xCC
#define HX8357_SET_GAMMA_CURVE           	0xE0

#define HX8357_INVON 						0x21
#define HX8357_INVOFF						0x20
#define HX8357_DISPLAYOFF					0x28
#define HX8357_DISPLAYON					0x29
#include "LCD_Driver.h"
//==============================================================
// Initialise HX8357 LCD Driver IC
//==============================================================
#define TFTWIDTH 320
#define TFTHEIGHT 480
LCD_Driver::LCD_Driver(void) : Adafruit_GFX(TFTWIDTH, TFTHEIGHT) {
	init();
	begin();
}

void LCD_Driver::init(){

	//SET RESET OUT
	LCD_RST_PORT.DIRSET |= LCD_RST_MASK;
	//disable reset
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

void LCD_Driver::reset()
{
	CS_IDLE;
	WR_IDLE;
	RD_IDLE;

	
	LCD_RST_CLR;
	_delay_ms(2);
	LCD_RST_SET;
	//_delay_ms(200);
	

	CS_ACTIVE;
	CD_COMMAND;
	writeData(0x00);
	WR_STROBE;
	WR_STROBE;
	WR_STROBE;

	CS_IDLE;
}

void LCD_Driver::begin() 
{
	//_comm->initializeDevice();
	
    writeCommand(HX8357_EXIT_SLEEP_MODE); //Sleep Out
    _delay_ms(150);
	writeCommand(HX8357_ENABLE_EXTENSION_COMMAND);
	writeData(0xFF);
	writeData(0x83);
	writeData(0x57);
	_delay_ms(1);
	writeCommand(HX8357_SET_POWER_CONTROL);
	writeData(0x00);
	writeData(0x12);
	writeData(0x12);
	writeData(0x12);
	writeData(0xC3);
	writeData(0x44);
	_delay_ms(1);
	writeCommand(HX8357_SET_DISPLAY_MODE);
	writeData(0x02);
	writeData(0x40);
	writeData(0x00);
	writeData(0x2A);
	writeData(0x2A);
	writeData(0x20);
	writeData(0x91);
	_delay_ms(1);
	writeCommand(HX8357_SET_VCOM_VOLTAGE);
	writeData(0x38);
	_delay_ms(1);
	writeCommand(HX8357_SET_INTERNAL_OSCILLATOR);
	writeData(0x68);
	writeCommand(0xE3); //Unknown Command
	writeData(0x2F);
	writeData(0x1F);
	writeCommand(0xB5); //Set BGP
	writeData(0x01);
	writeData(0x01);
	writeData(0x67);
	writeCommand(HX8357_SET_PANEL_DRIVING);
	writeData(0x70);
	writeData(0x70);
	writeData(0x01);
	writeData(0x3C);
	writeData(0xC8);
	writeData(0x08);
	_delay_ms(1);
	writeCommand(0xC2); // Set Gate EQ
	writeData(0x00);
	writeData(0x08);
	writeData(0x04);
	_delay_ms(1);
	writeCommand(HX8357_SET_PANEL_CHARACTERISTIC);
	writeData(0x09);
	_delay_ms(1);
	writeCommand(HX8357_SET_GAMMA_CURVE);
	writeData(0x01);
	writeData(0x02);
	writeData(0x03);
	writeData(0x05);
	writeData(0x0E);
	writeData(0x22);
	writeData(0x32);
	writeData(0x3B);
	writeData(0x5C);
	writeData(0x54);
	writeData(0x4C);
	writeData(0x41);
	writeData(0x3D);
	writeData(0x37);
	writeData(0x31);
	writeData(0x21);
	writeData(0x01);
	writeData(0x02);
	writeData(0x03);
	writeData(0x05);
	writeData(0x0E);
	writeData(0x22);
	writeData(0x32);
	writeData(0x3B);
	writeData(0x5C);
	writeData(0x54);
	writeData(0x4C);
	writeData(0x41);
	writeData(0x3D);
	writeData(0x37);
	writeData(0x31);
	writeData(0x21);
	writeData(0x00);
	writeData(0x01);
	_delay_ms(1);
	writeCommand(HX8357_SET_PIXEL_FORMAT); //COLMOD RGB888
	writeData(0x55);
	writeCommand(HX8357_SET_ADDRESS_MODE);
	writeData(0x00);
	writeCommand(HX8357_SET_TEAR_ON); //TE ON
	writeData(0x00);
	_delay_ms(10);
	writeCommand(HX8357_SET_DISPLAY_ON); //Display On
	_delay_ms(10);
	writeCommand(HX8357_WRITE_MEMORY_START); //Write SRAM Data
}

void LCD_Driver::setAddrWindow(int x0, int y0, int x1, int y1) 
{
 writeCommand(HX8357_SET_COLUMN_ADDRESS); // Column addr set
 writeData(x0 >> 8);
 writeData(x0);     // XSTART 
 writeData(x1 >> 8);
 writeData(x1);     // XEND

 writeCommand(HX8357_SET_PAGE_ADDRESS); // Row addr set
 writeData(y0 >> 8);
 writeData(y0);     // YSTART
 writeData(y1 >> 8);
 writeData(y1);     // YEND

 writeCommand(HX8357_WRITE_MEMORY_START); //Write SRAM Data
}

void LCD_Driver::drawPixel(int16_t x, int16_t y, uint16_t color) 
{
	if((x < 0) ||(x >= _width) || (y < 0) || (y >= _height)) 
		return;
	setAddrWindow(x,y,x+1,y+1);
	writeData(color >> 8);
	writeData(color);
}

void LCD_Driver::writeData(uint8_t data){
	CS_ACTIVE;
	CD_DATA;
	LCD_DATA_PORT.OUT = data;
	_delay_ms(1);
	WR_ACTIVE;
	_delay_ms(1);
	WR_IDLE;
	_delay_ms(1);
}

void LCD_Driver::writeCommand(uint8_t data){
	CS_ACTIVE;
	CD_COMMAND;
	LCD_DATA_PORT.OUT = data;
	_delay_ms(1);
	WR_ACTIVE;
	_delay_ms(1);
	WR_IDLE;
	_delay_ms(1);
}

void LCD_Driver::readData(uint8_t cmd, uint8_t numparams) {}
void LCD_Driver::writeRegister32(uint8_t r, uint32_t d) {}




