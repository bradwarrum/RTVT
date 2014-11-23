/*
 * MainScreen.h
 *
 * Created: 11/22/2014 10:42:15 PM
 *  Author: ee63pc3-user
 */ 


#ifndef MAINSCREEN_H_
#define MAINSCREEN_H_
#include "Screen.h"
#include "string.h"
class MainScreen : public Screen {
public:
	MainScreen(LCD_Driver * lcdscreen);
	void update(BTOBD * bt);
	void clear();

private:
	void draw(OBDCMDS::CMD cmd, BTOBD * bt);
};



#endif /* MAINSCREEN_H_ */