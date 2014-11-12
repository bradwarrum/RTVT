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
#include <ff.h>

BTOBD * OBDref;
LCD_Driver * LCDref;
GPS * GPSref;
// Utilizes USARTD0 for bluetooth receive

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
		PORTD.OUT = 0x88;
		PORTD.DIR = 0x88;
		PORTE.DIR = 0xB0;
	LCD_Driver LCD;
	LCDref = &LCD;
		LCD.begin();
		LCD.fillScreen(0xFFFF);
		LCD.setCursor(0,0);
		LCD.setTextColor(0x0000, 0xFFFF);
		LCD.setTextSize(2);
		LCD.setTextWrap(true);
	//SPI_Driver spi(&SPIE);
	//spi.enable();
	/*UartDriver BluT(64);// = new UartDriver(64); //64 character buffer, rx and tx
	BluT.init(&USARTD0, ParityMode::DISABLED, CharSize::EIGHT, false, 12, 0x40); // 9600 bps*/
	GPS GPSModule;
	GPSref = &GPSModule;
	_delay_ms(1000);

	//BTOBD OBDModule;

	PMIC.CTRL |= 0x07;
	asm("sei");
	_delay_ms(500);
	GPSModule.init();
	while(1){}
	/*while (1) {
		for (int i = 0; i < 64; i++) spi.put(i);
	}*/
	
	/*FRESULT fr;
	FATFS fs;
	FIL fil;

	f_mount(&fs, "", 0);
	fr = open_append(&fil, "logfile.txt");
	if (fr != FR_OK) return 1;
	f_puts("HELLO MOFOS\n", &fil);
	f_close(&fil);*/
	/*OBDref = &OBDModule;
	OBDModule.initialize("000666643F8F", LCDref);

	while (1) { while(!OBDModule.rxIsEmpty()) LCD.write(OBDModule.rxDequeue()); }*/

	
}