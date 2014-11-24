/*
 * RTVT.cpp
 *
 * Created: 9/30/2014 7:07:40 PM
 *  Author: ee63pc3-user
 */ 

#ifndef F_CPU
#define F_CPU 32000000UL
#endif
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include "LCD_Driver.h"
#include "BTOBD.h"
#include "SPI_Driver.h"
#include "uart_driver.h"
#include "GPS.h"
#include "Touch.h"
#include "MainScreen.h"
#include <ff.h>
BTOBD * OBDref;
LCD_Driver * LCDref;
GPS * GPSref;
Touch * Touchref;
// Utilizes USARTD0 for Bluetooth receive


ISR(USARTD0_RXC_vect)
{
	OBDref->interceptByte();
}
ISR(USARTD1_RXC_vect) {
	GPSref->interceptByte();
}
ISR(USARTD0_TXC_vect) {

}
ISR(USARTD1_TXC_vect) {

}
ISR(USARTD0_DRE_vect) {
	OBDref->handleDRE();
}
ISR(USARTD1_DRE_vect) {
	GPSref->handleDRE();
}
ISR(BADISR_vect) {
	asm("NOP");
}

ISR(SPIE_INT_vect) {
	
}



ISR(TCC0_CCA_vect)
{
	//Reset the timer
	TCC0.CNT = 0;

	Touchref->process();
	
	if (Touchref->getX() > 0x0400 || Touchref->getY() > 0x0400) PORTF.OUTSET = 0x01;
	else PORTF.OUTCLR = 0x01;
}

void setClockTo2MHz()
{
	CCP = CCP_IOREG_gc;
	OSC.CTRL = OSC_RC2MEN_bm;
	while(!(OSC.STATUS & OSC_RC2MRDY_bm));
	CCP = CCP_IOREG_gc;
	CLK.CTRL = CLK_SCLKSEL_RC2M_gc;   
}

void setClockTo32MHz()
{
    CCP = CCP_IOREG_gc;              // disable register security for oscillator update
    OSC.CTRL = OSC_RC32MEN_bm;			   // enable 32MHz oscillator
    while(!(OSC.STATUS & OSC_RC32MRDY_bm)); // wait for oscillator to be ready
    CCP = CCP_IOREG_gc;              // disable register security for clock update
    CLK.CTRL = CLK_SCLKSEL_RC32M_gc; // switch to 32MHz clock
}

FRESULT open_append(FIL * fp, const char * path) {
	FRESULT fr;

	fr = f_open(fp, path, FA_WRITE | FA_OPEN_ALWAYS);
	if (fr == FR_OK) {
		fr = f_lseek(fp, f_size(fp));
		if (fr != FR_OK) f_close(fp);
	}
	return fr;
}


int main(void)
{
	asm("cli");
	setClockTo32MHz();
	_delay_ms(1000);

	// Set pins as outputs
	PORTD.OUT = 0x88;
	PORTD.DIR = 0x88;
	PORTE.DIR = 0xB0;
	PORTF.DIR = 0x01;

	// LCD setup
	LCD_Driver LCD;
	LCDref = &LCD;
	LCD.begin();
	LCD.fillScreen(0x0000);
	LCD.setRotation(3);
	LCD.setCursor(0,0);
	LCD.setTextColor(0xFFFF, 0x0000);
	LCD.setTextSize(2);
	LCD.setTextWrap(true);

	// Touch setup
	//Touch toucj;
	//Touchref = &toucj;
	//_delay_ms(1000);
	
	// GPS setup
	GPS GPSModule;
	GPSref = &GPSModule;

	// BT setup
	BTOBD OBDModule;
	OBDref = &OBDModule;

	PMIC.CTRL |= 0x07;
	asm("sei");


	bool send = true;
	//OBDModule.initialize("000666643F8F", LCDref);
	OBDModule.initialize("00195DE8057A", LCDref);
	GPSModule.init();
	MainScreen ms(LCDref);
	ms.clear();
	GPSModule.startReceiving();
	while (1) {
		if (send) {
			OBDModule.sendCmd();
		}
		send = OBDModule.rcvResp();
		GPSModule.updateRegisters();

		//GPSModule.vomit(LCDref);
		ms.update(OBDref, GPSref);
	}
	
	//FRESULT fr;
	//FATFS fs;
	//FIL fil;
	//int err;

	//f_mount(&fs, "", 0);

	//fr = open_append(&fil, "test.txt");
	//if (fr != FR_OK) return 1;
	//err = f_puts((TCHAR *) "abcdefghijklmnopqrstuvwxyz\nabcdefghijklmnopqrstuvwxyz\nabcdefghijklmnopqrstuvwxyz\n", &fil);
	//if (err == -1) return 2;
	//f_close(&fil);

	//while (1) { if (!OBDModule.rxIsEmpty()) LCD.write(OBDModule.rxDequeue()); }
}



/*
int main()
{
    while (1);
}
*/
