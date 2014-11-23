/*
 * Screen.h
 *
 * Created: 11/22/2014 10:31:37 PM
 *  Author: ee63pc3-user
 */ 


#ifndef SCREEN_H_
#define SCREEN_H_
#include "BTOBD.h"
#include "OBDConversion.h"
#include "LCD_Driver.h"


class Screen {
protected:
	LCD_Driver * lcd;
public:
	Screen(LCD_Driver * lcdscreen);
	void update(BTOBD * bt);
	void clear();

};



#endif /* SCREEN_H_ */