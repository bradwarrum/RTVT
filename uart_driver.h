/*
 * uart_driver.h
 *
 * Created: 9/30/2014 7:11:48 PM
 *  Author: ee63pc3-user
 */

#include <avr/io.h>

#ifndef UART_DRIVER_H_
#define UART_DRIVER_H_

class UartDriver
{
	public:
	enum PMODE
	{
		DISABLED, EVEN, ODD
	}; //uart parity mode
	
	enum CHSIZE
	{
		FIVE, SIX, SEVEN, EIGHT, NINE
	}; //character size (number of data bits per frame)
	
	UartDriver();
	void init(USART_t uval, PMODE pmode, CHSIZE chsize, bool stopbit, char baudctrla, char baudctrlb);
	
	private:
	USART_t uval;
	
};

#endif /* UART_DRIVER_H_ */