/*
 * SPI_Driver.cpp
 *
 * Created: 10/31/2014 2:42:42 PM
 *  Author: ee63pc3-user
 */ 

 #include "SPI_Driver.h"
 SPI_Driver::SPI_Driver(SPI_t * spi_unit) {
 	spi = spi_unit;
 	spi->CTRL = 0x93; //Initially 250khz clock rate
	//spi->INTCTRL = 0x03; // High level interrupt
	spi->INTCTRL = 0x00; // No interrupt, polling mode
 }
 void SPI_Driver::clockRate(char bit710) {
	spi->CTRL &= ~0x83;
	spi->CTRL |= bit710;
 }
 void SPI_Driver::enable() {
	spi->CTRL |= 0x40;
 }
 void SPI_Driver::disable() {
	spi->CTRL &= ~0x40;
 }
 bool SPI_Driver::txOK() {
	return spi->STATUS & 0x80 ? true : false;
 }
 void SPI_Driver::put(BYTE c) {
	/*if (!tx_in_prog) {
		spi->DATA = c;
		tx_in_prog = true;
	}else {
		MOQueue.enqueue(c);
	}*/
	spi->DATA = c;
	while(!(spi->STATUS & SPI_IF_bm)){}
 }
 BYTE SPI_Driver::get() {
	spi->DATA = 0xFF;
	while(!(spi->STATUS & SPI_IF_bm)){}
	return spi->DATA;
 }
 void SPI_Driver::get_buffered(BYTE * dst) {
	spi->DATA = 0xFF;
	while (!(spi->STATUS & SPI_IF_bm)) {}
	*(dst)=spi->DATA;
 }


