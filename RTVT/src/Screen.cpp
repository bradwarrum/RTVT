/*
 * Screen.cpp
 *
 * Created: 11/22/2014 10:51:26 PM
 *  Author: ee63pc3-user
 */ 

 #include "Screen.h"

 Screen::Screen(LCD_Driver * lcdscreen) {lcd = lcdscreen;}

 void Screen::update(BTOBD * bt) {}
 void Screen::clear() {
	lcd->fillScreen(WHITE);
 }