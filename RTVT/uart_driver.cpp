/*
 * uart_driver.cpp
 *
 * Created: 9/30/2014 7:11:07 PM
 *  Author: ee63pc3-user
 */ 

#include "uart_driver.h"

UartDriver::UartDriver()
{
}

void UartDriver::init(USART_t uval, PMODE pmode, CHSIZE chsize, bool stopbit, char baudctrla, char baudctrlb)
{
	this->uval = uval;
	this->uval.CTRLA = 0x10; //set receive interrupt to low priority
	this->uval.CTRLB = 0x18; //set rxen, txen
	this->uval.CTRLC = 0x00;
	this->uval.BAUDCTRLA = baudctrla;
	this->uval.BAUDCTRLB = baudctrlb;

	if (pmode == EVEN) this->uval.CTRLC |= 0x20;
	else if (pmode == ODD) this->uval.CTRLC |= 0x30;
	
	if (chsize == SIX) this->uval.CTRLC |= 0x01;
	else if (chsize == SEVEN) this->uval.CTRLC |= 0x02;
	else if (chsize == EIGHT) this->uval.CTRLC |= 0x03;
	else if (chsize == NINE) this->uval.CTRLC |= 0x07;
	
	if (stopbit) this->uval.CTRLC |= 0x08;
}