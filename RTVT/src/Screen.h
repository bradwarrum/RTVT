/*
 * Screen.h
 *
 * Created: 11/22/2014 10:31:37 PM
 *  Author: ee63pc3-user
 */ 


#ifndef SCREEN_H_
#define SCREEN_H_
#include "LCD_Driver.h"
#include "BTOBD.h"
#include "GPS.h"
#include "OBDConversion.h"


class Screen {
protected:
	LCD_Driver * lcd;
public:
	Screen(LCD_Driver * lcdscreen);
	void update(BTOBD * bt, GPS * gps);
	void clear();

};



#endif /* SCREEN_H_ */