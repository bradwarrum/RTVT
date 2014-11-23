/*
 * MainScreen.cpp
 *
 * Created: 11/22/2014 10:41:41 PM
 *  Author: ee63pc3-user
 */ 

 #include "MainScreen.h"

 MainScreen::MainScreen(LCD_Driver * lcdscreen) : Screen(lcdscreen) {}
 
 void MainScreen::update(BTOBD * bt) {
	uint16_t status = bt->getStatus();
	bt->clearStatus();
	OBDCMDS::CMD cmd = OBDCMDS::SPEED;
	while (status) {
		if (status & 1) draw(cmd, bt);
		status >>= 1;
		cmd = static_cast<OBDCMDS::CMD>(((int)cmd)+ 1);
	}

 }
 void MainScreen::clear() {
	lcd->fillScreen(BLACK);
	lcd->drawFastVLine(239, 0, 320, WHITE);
	lcd->drawFastHLine(0, 119, 480, WHITE);
	lcd->drawFastHLine(0, 239, 480, WHITE);
	lcd->setTextColor(WHITE, BLACK);
	lcd->setTextSize(2);
	lcd->setCursor(200,50);
	lcd->writeStr("MPH");
	lcd->setCursor(200,180);
	lcd->writeStr("RPM");
	lcd->setCursor(10,300);
	lcd->writeStr("Lat:");
	lcd->setCursor(249, 300);
	lcd->writeStr("Lon:");
	lcd->setCursor(429,75);
	lcd->writeStr("Eng\nLoad");
	lcd->setCursor(429,195);
	lcd->writeStr("Pdl\nPos");

	lcd->setTextSize(9);
	lcd->setCursor(30, 30);
	lcd->writeStr("000");
	lcd->setCursor(259,30);
	lcd->writeStr("100");
	lcd->setCursor(259,149);
	lcd->writeStr("100");


	lcd->setTextSize(8);
	lcd->setCursor(5,149);
	lcd->writeStr("0000");
	lcd->setCursor(429,10);
	lcd->write('%');
	lcd->setCursor(429,129);
	lcd->write('%');
 }
 void MainScreen::draw(OBDCMDS::CMD cmd, BTOBD * bt){
 	CBUFFER resp = bt->getRecent(cmd);
	uint16_t val;
	char wbuf[5];
	switch(cmd) {
	case OBDCMDS::SPEED:
		lcd->setCursor(30,30);
		lcd->setTextSize(9);
		val = OBDConversion::speed(resp.au, resp.al);
		if (val < 100) lcd->write(' ');
		if (val < 10) lcd->write(' ');
		lcd->writeStr(itoa(val, wbuf, 10));
		break;
	case OBDCMDS::RPM:
		lcd->setCursor(5,149);
		lcd->setTextSize(8);
		val = OBDConversion::rpm(resp.au, resp.al, resp.bu, resp.bl);
		if (val >= 10000) val = 9999;
		if (val < 1000) lcd->write(' ');
		if (val < 100) lcd->write(' ');
		if (val < 10) lcd->write(' ');
		lcd->writeStr(itoa(val, wbuf, 10));
		break;
	case OBDCMDS::ENGINE_LOAD:
		lcd->setCursor(259,30);
		lcd->setTextSize(9);
		val = OBDConversion::engine_load(resp.au, resp.al);
		
		if (val < 100) lcd->write(' ');
		if (val < 10) lcd->write(' ');
		lcd->writeStr(itoa(val, wbuf, 10));
		break;
	case OBDCMDS::THROTTLE_POS:
		lcd->setCursor(259, 149);
		lcd->setTextSize(9);
		val = OBDConversion::throttle_pos(resp.au, resp.al);
		if (val < 100) lcd->write(' ');
		if (val < 10) lcd->write(' ');
		lcd->writeStr(itoa(val, wbuf, 10));
		break;
	}
 }