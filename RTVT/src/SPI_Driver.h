/*
 * SPI_Driver.h
 *
 * Created: 10/31/2014 2:44:41 PM
 *  Author: ee63pc3-user
 */ 


#ifndef SPI_DRIVER_H_
#define SPI_DRIVER_H_
#include "ByteQueue.h"
#include <avr/io.h>
#include "integer.h"

class SPI_Driver {
public:
	SPI_Driver(SPI_t * spi_unit);
	void clockRate(char bit710);
	void disable();
	void enable();
	bool txOK();
	void put(BYTE c);
	BYTE get();
	void get_buffered(BYTE * dst);
private:
	SPI_t * spi;
};




#endif /* SPI_DRIVER_H_ */