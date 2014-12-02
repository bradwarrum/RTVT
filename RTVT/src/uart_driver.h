/*
 * uart_driver.h
 *
 * Created: 9/30/2014 7:11:48 PM
 *  Author: ee63pc3-user
 */


#ifndef UART_DRIVER_H_
#define UART_DRIVER_H_
#include <avr/io.h>
#include "ByteQueue.h"
#include "integer.h"
namespace ParityMode {
	enum PMODE {
		DISABLED, EVEN, ODD
	};
}
namespace CharSize {
	enum CHSIZE {
			FIVE, SIX, SEVEN, EIGHT, NINE
	};
}


class UartDriver
{
	public:
	/*typedef enum PMODE
	{
		DISABLED, EVEN, ODD
	}ParityMode; //uart parity mode
	
	typedef enum CHSIZE
	{
		FIVE, SIX, SEVEN, EIGHT, NINE
	}; //character size (number of data bits per frame)*/
	
	UartDriver(int rx_buffer_size, int tx_buffer_size);
	void init(USART_t * uval, ParityMode::PMODE pmode, CharSize::CHSIZE chsize, bool stopbit, char baudctrla, char baudctrlb);
	void sendByte(char c);
	BYTE sendString(const char * str, int numchars, BYTE checksum);
	void rcvByte(char c);
	bool txIsEmpty();
	bool rxIsEmpty();
	char txDequeue();
	char rxDequeue();
	USART_t * getUARTPort();
	void dump(FIL * fp);
	
	private:
	USART_t * uval;
	ByteQueue rxQueue, txQueue;

	
};

#endif /* UART_DRIVER_H_ */