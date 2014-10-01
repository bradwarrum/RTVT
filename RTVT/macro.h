/*
 * macro.h
 *
 * Created: 9/30/2014 11:36:57 AM
 *  Author: ee63pc3-user
 */

#ifndef MACRO_H_
#define MACRO_H_

//// Port definitions
#define LCD_CTRL_PORT PORTA
#define JTAG_PORT PORTB
#define LCD_DATA_PORT PORTE
#define LCD_AUX_PORT PORTF

//// LCD Pins
#define LCD_RD_MASK	0x01	// LCD read (pin 0)
#define LCD_RD_CLR LCD_CTRL_PORT &= ~LCD_RD_MASK
#define LCD_RD_SET LCD_CTRL_PORT |= LCD_RD_MASK
#define LCD_WR_MASK	0x02	// LCD write (pin 1)
#define LCD_WR_CLR LCD_CTRL_PORT &= ~LCD_WR_MASK
#define LCD_WR_SET LCD_CTRL_PORT |= LCD_WR_MASK
#define LCD_CD_MASK	0x04	// LCD command/data (pin2  2)
#define LCD_CD_CLR LCD_CTRL_PORT &= ~LCD_CD_MASK
#define LCD_CD_SET LCD_CTRL_PORT |= LCD_CD_MASK
#define LCD_CS_MASK	0x08	// LCD chip select (enable) (pin 3)
#define LCD_CS_CLR LCD_CTRL_PORT &= ~LCD_CS_MASK
#define LCD_CS_SET LCD_CTRL_PORT |= LCD_CS_MASK
#define LCD_BL_MASK 0x08	// LCD backlite output (PWM) (pin 3)
#define LCD_BL_CLR LCD_AUX_PORT &= ~LCD_BL_MASK
#define LCD_BL_SET LCD_AUX_PORT |= LCD_BL_MASK
#define LCD_RST_MASK 0x10	// LCD hardware reset (pin 4)
#define LCD_RST_CLR LCD_AUX_PORT &= ~LCD_RST_MASK
#define LCD_RST_SET LCD_AUX_PORT |= LCD_RST_MASK
// TODO:- define setup pins and procedure for PWM setup

//// Touch Screen Pins
#define TFT_X_P_MASK 0x80		// Touch Screen X+ (pin 7)
#define TFT_X_P_CLR LCD_CTRL_PORT &= ~TFT_X_P_MASK
#define TFT_X_P_SET LCD_CTRL_PORT |= TFT_X_P_MASK
#define TFT_Y_P_MASK 0x40		// Touch Screen Y+ (pin 6)
#define TFT_Y_P_CLR LCD_CTRL_PORT &= ~TFT_Y_P_MASK
#define TFT_Y_P_SET LCD_CTRL_PORT |= TFT_Y_P_MASK
#define TFT_X_M_MASK 0x20		// Touch Screen X- pin 5
#define TFT_X_M_CLR LCD_CTRL_PORT &= ~TFT_X_M_MASK
#define TFT_X_M_SET LCD_CTRL_PORT |= TFT_X_M_MASK
#define TFT_Y_M_MASK 0x10		// Touch Screen Y- pin 4
#define TFT_Y_M_CLR LCD_CTRL_PORT &= ~TFT_Y_M_MASK
#define TFT_Y_M_SET LCD_CTRL_PORT |= TFT_Y_M_MASK

//// SD Card

#endif /* MACRO_H_ */