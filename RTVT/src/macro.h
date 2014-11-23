/*
 * macro.h
 *
 * Created: 9/30/2014 11:36:57 AM
 *  Author: ee63pc3-user
 */

#ifndef MACRO_H_
#define MACRO_H_

//// Clock macros
#define CLK_32MHZ { CCP = CCP_IOREG_gc; OSC.CTRL = OSC_RC32MEN_bm; while(!(OSC.STATUS & OSC_RC32MRDY_bm)); CCP = CCP_IOREG_gc; CLK.CTRL = CLK_SCLKSEL_RC32M_gc;}

//// Port definitions
#define LCD_CTRL_PORT PORTA
#define TFT_CTRL_PORT PORTA
#define JTAG_PORT PORTB
#define LCD_DATA_PORT PORTC 
#define LCD_RST_PORT PORTD

//// LCD Pins
#define LCD_RD_MASK	0x08	// LCD read
#define LCD_RD_CLR LCD_CTRL_PORT.OUT &= ~LCD_RD_MASK
#define LCD_RD_SET LCD_CTRL_PORT.OUT |= LCD_RD_MASK
#define LCD_WR_MASK	0x04	// LCD write
#define LCD_WR_CLR LCD_CTRL_PORT.OUT &= ~LCD_WR_MASK
#define LCD_WR_SET LCD_CTRL_PORT.OUT |= LCD_WR_MASK
#define LCD_CD_MASK	0x02	// LCD command/data
#define LCD_CD_CLR LCD_CTRL_PORT.OUT &= ~LCD_CD_MASK
#define LCD_CD_SET LCD_CTRL_PORT.OUT |= LCD_CD_MASK
#define LCD_CS_MASK	0x01	// LCD chip select (enable)
#define LCD_CS_CLR LCD_CTRL_PORT.OUT &= ~LCD_CS_MASK
#define LCD_CS_SET LCD_CTRL_PORT.OUT |= LCD_CS_MASK

//backlight special
#define LCD_BL_MASK 0x01	// LCD backlite output (PWM) (pin 3)
#define LCD_BL_CLR PORTD.OUT &= ~LCD_BL_MASK
#define LCD_BL_SET PORTD.OUT |= LCD_BL_MASK
//end backlight

//reset special
#define LCD_RST_MASK 0x01	// LCD hardware reset
#define LCD_RST_CLR LCD_RST_PORT.OUT &= ~LCD_RST_MASK
#define LCD_RST_SET LCD_RST_PORT.OUT |= LCD_RST_MASK
//end reset special

// TODO:- define setup pins and procedure for PWM setup

//// Touch Screen Pins

#define TFT_X_P_MASK 0x10		// Touch Screen X+ (pin 7)
#define TFT_X_P_CLR TFT_CTRL_PORT.OUT &= ~TFT_X_P_MASK
#define TFT_X_P_SET TFT_CTRL_PORT.OUT |= TFT_X_P_MASK
#define TFT_Y_P_MASK 0x20		// Touch Screen Y+ (pin 6)
#define TFT_Y_P_CLR TFT_CTRL_PORT.OUT &= ~TFT_Y_P_MASK
#define TFT_Y_P_SET TFT_CTRL_PORT.OUT |= TFT_Y_P_MASK
#define TFT_X_M_MASK 0x40		// Touch Screen X- (pin 5)
#define TFT_X_M_CLR TFT_CTRL_PORT.OUT &= ~TFT_X_M_MASK
#define TFT_X_M_SET TFT_CTRL_PORT.OUT |= TFT_X_M_MASK
#define TFT_Y_M_MASK 0x80		// Touch Screen Y- (pin 4)
#define TFT_Y_M_CLR TFT_CTRL_PORT.OUT &= ~TFT_Y_M_MASK
#define TFT_Y_M_SET TFT_CTRL_PORT.OUT |= TFT_Y_M_MASK

//// SD Card / SPI
#define SPI_CS_BM 0x10
#define SPI_CD_BM 0x08
#define SPI_WP_BM 0x04
#define SPI_PORT PORTE
#define SPI_DESELECT {SPI_PORT.OUTSET = SPI_CS_BM;}
#define SPI_SELECT {SPI_PORT.OUTCLR = SPI_CS_BM;}


//// LCD library macros

#define RD_IDLE LCD_RD_SET
#define WR_IDLE LCD_WR_SET
#define CD_DATA LCD_CD_SET
#define CS_IDLE LCD_CS_SET

#define RD_ACTIVE LCD_RD_CLR
#define WR_ACTIVE LCD_WR_CLR
#define CD_COMMAND LCD_CD_CLR
#define CS_ACTIVE LCD_CS_CLR

#define WR_STROBE { WR_ACTIVE; WR_IDLE; }

//Bluetooth OBD New Data Available Masks
#define OBD_SPEED 0x01;
#define OBD_RPM 0x02;
#define OBD_THROTTLE_POS 0x04;
	

#endif /* MACRO_H_ */