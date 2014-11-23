/*
 * Touch.h
 *
 * Created: 11/12/2014 12:51:41 PM
 *  Author: ee63pc3-user
 */ 

#ifndef F_CPU
#define F_CPU 32000000UL
#endif
#ifndef TOUCH_H_
#define TOUCH_H_
#include <util/delay.h>

#include "macro.h"
#include <avr/io.h>

class Touch
{
	public:
	Touch();
	void process();
	int getX();
	int getY();

	private:
	void startX();
	void startY();
	void setupX();
	void setupY();
	int getCycle();
	int cycle;
	int xout, yout, xin;
};



#endif /* TOUCH_H_ */