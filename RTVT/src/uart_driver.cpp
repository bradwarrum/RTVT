/*
 * uart_driver.cpp
 *
 * Created: 9/30/2014 7:11:07 PM
 *  Author: ee63pc3-user
 */ 

#include "uart_driver.h"

UartDriver::UartDriver(int rx_buffer_size, int tx_buffer_size) : rxQueue(rx_buffer_size), txQueue(tx_buffer_size) {}
	
void UartDriver::dump(FIL * fp) {
	rxQueue.dumpToFile(fp);
}

void UartDriver::init(USART_t * uval, ParityMode::PMODE pmode, CharSize::CHSIZE chsize, bool stopbit, char baudctrla, char baudctrlb)
{
	this->uval = uval;
	this->uval->CTRLC = 0x00;
	this->uval->BAUDCTRLA = baudctrla;
	this->uval->BAUDCTRLB = baudctrlb;

	if (pmode == ParityMode::EVEN) this->uval->CTRLC |= 0x20;
	else if (pmode == ParityMode::ODD) this->uval->CTRLC |= 0x30;
	
	if (chsize == CharSize::SIX) this->uval->CTRLC |= 0x01;
	else if (chsize == CharSize::SEVEN) this->uval->CTRLC |= 0x02;
	else if (chsize == CharSize::EIGHT) this->uval->CTRLC |= 0x03;
	else if (chsize == CharSize::NINE) this->uval->CTRLC |= 0x07;
	
	if (stopbit) this->uval->CTRLC |= 0x08;

	this->uval->CTRLA = 0x3F; //set receive and transmit interrupt to high priority
	this->uval->CTRLB = 0x18; //set rxen, txen
}

void UartDriver::sendByte(char c) {
	if (this->uval->STATUS & USART_DREIF_bm) {
		//IF data register is empty, transmit the next character
		this->uval->DATA = c;
	}else {
		//try to add to the queue if it is not empty (not guaranteed)
		txQueue.enqueue(c);
	}
	uval->CTRLA |= (0x03);
}
void UartDriver::rcvByte(char c) {
	rxQueue.enqueue(c);
}

char UartDriver::rxDequeue() {
	return rxQueue.dequeue();
}
bool UartDriver::rxIsEmpty() {
	return rxQueue.isEmpty();
}
bool UartDriver::txIsEmpty() {
	return txQueue.isEmpty();
}
char UartDriver::txDequeue() {
	return txQueue.dequeue();
}
USART_t * UartDriver::getUARTPort() {
	return uval;
}

BYTE UartDriver::sendString(const char * str, int numchars, BYTE checksum) {
	 for (volatile int i = 0; i < numchars; i++) {
		if (str[i] == '\0') break;
		if (str[i] != '$' && str[i] != '*') checksum ^= str[i];
		while (txQueue.isFull()) {}
		this->sendByte(str[i]);
	 }

	 return checksum;
 }