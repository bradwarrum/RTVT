/*
 * Touch.cpp
 *
 * Created: 11/12/2014 12:51:28 PM
 *  Author: ee63pc3-user
 */ 

#include "Touch.h"

Touch::Touch()
 {
	TFT_CTRL_PORT.DIRSET = 0xC0;
	TFT_Y_M_CLR;
	TFT_X_M_CLR;

	TCC0.CTRLA = 0x00; //Off at first
	TCC0.CTRLB = 0x10; //CCA on, no waveform generation
	TCC0.CCA = 1000; //Check approximately at 4 Hz
	TCC0.INTCTRLB = 0x01; //Enable CCA interrupts

	ADCA.CTRLA = 0x01;
	ADCA.CTRLB = 0x10;
	ADCA.CH0.CTRL = 0x02; //Single ended positive input
	ADCA.PRESCALER = 0x03; //1 MHz
	//ADCA.CH0.INTCTRL = 0x01; //Low level interrupt enable
	ADCA.REFCTRL = 0x10;

	TCC0.CTRLA = 0x07; // Prescaler /1024

	ADCA.CH0.INTFLAGS |= 0x01;
	this->startY();
	this->cycle = 3;
 }

int Touch::getCycle()
{
    cycle = (++cycle == 4) ? 0 : cycle;
	return cycle;
}

void Touch::startX() {

	// Start ADC
	ADCA.CH0.MUXCTRL = 0x27;
	ADCA.CH0.CTRL = 0x82;

	// Get X from xp
	//while(!ADCA.CH0.INTFLAGS);

	//return ADCA.CH0.RES;
}

void Touch::startY() {

	// Start ADC
	ADCA.CH0.MUXCTRL = 0x2F;
	ADCA.CH0.CTRL = 0x82;
		 
	// Get Y from yp
	//while(!(ADCA.CH0.INTFLAGS & 0x01));

	//return ADCA.CH0.RES;
}

void Touch::setupX()
{
    while(!(ADCA.CH0.INTFLAGS & 0x01));
	xout = xin;
	yout = ADCA.CH0.RES;

    //// Set yp high, set xp input
	TFT_CTRL_PORT.DIRCLR = 0x50;
	TFT_CTRL_PORT.DIRSET = 0xA0;
	TFT_Y_P_SET;
	TFT_Y_M_CLR;
}

void Touch::setupY()
{
    while(!(ADCA.CH0.INTFLAGS & 0x01));
	xin = ADCA.CH0.RES;

    // Set xp high, set yp input
	TFT_CTRL_PORT.DIRCLR = 0xA0;
	TFT_CTRL_PORT.DIRSET = 0x50;
	TFT_X_P_SET;
	TFT_X_M_CLR;
}

void Touch::process()
{
    switch (this->getCycle())
	{
	    case 0: setupX(); break;
		case 1: startX(); break;
		case 2: setupY(); break;
		case 3: startY(); break;
		default: asm("nop");
	}
}

int Touch::getX()
{
    return this->xout;
}

int Touch::getY()
{
    return this->yout;
}