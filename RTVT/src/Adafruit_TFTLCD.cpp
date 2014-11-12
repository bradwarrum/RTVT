// IMPORTANT: LIBRARY MUST BE SPECIFICALLY CONFIGURED FOR EITHER TFT SHIELD
// OR BREAKOUT BOARD USAGE.  SEE RELEVANT COMMENTS IN Adafruit_TFTLCD.h

// Graphics library by ladyada/adafruit with init code from Rossum
// MIT license

#if defined(__SAM3X8E__)
	#include <include/pio.h>
    #define PROGMEM
    #define pgm_read_byte(addr) (*(const unsigned char *)(addr))
    #define pgm_read_word(addr) (*(const unsigned short *)(addr))
#endif
#ifdef __AVR__
	#include <avr/pgmspace.h>
#endif
//TODO: our libraries
//#include "pins_arduino.h"
//#include "wiring_private.h"
#include "Adafruit_TFTLCD.h"
//#include "pin_magic.h"

#define TFTWIDTH   320
#define TFTHEIGHT  480

// LCD controller chip identifiers
#define ID_932X    0
#define ID_7575    1
#define ID_9341    2
#define ID_HX8357D    3
#define ID_UNKNOWN 0xFF



// Constructor for breakout board (configurable LCD control lines).
// Can still use this w/shield, but parameters are ignored.
Adafruit_TFTLCD::Adafruit_TFTLCD(
  uint8_t cs, uint8_t cd, uint8_t wr, uint8_t rd, uint8_t reset) :
  Adafruit_GFX(TFTWIDTH, TFTHEIGHT) {

//TODO: KILL
/*#ifndef USE_ADAFRUIT_SHIELD_PINOUT
  // Convert pin numbers to registers and bitmasks
  _reset     = reset;
  #ifdef __AVR__
    csPort     = portOutputRegister(digitalPinToPort(cs));
    cdPort     = portOutputRegister(digitalPinToPort(cd));
    wrPort     = portOutputRegister(digitalPinToPort(wr));
    rdPort     = portOutputRegister(digitalPinToPort(rd));
  #endif
  #if defined(__SAM3X8E__)
    csPort     = digitalPinToPort(cs);
    cdPort     = digitalPinToPort(cd);
    wrPort     = digitalPinToPort(wr);
    rdPort     = digitalPinToPort(rd);
  #endif
  csPinSet   = digitalPinToBitMask(cs);
  cdPinSet   = digitalPinToBitMask(cd);
  wrPinSet   = digitalPinToBitMask(wr);
  rdPinSet   = digitalPinToBitMask(rd);
  csPinUnset = ~csPinSet;
  cdPinUnset = ~cdPinSet;
  wrPinUnset = ~wrPinSet;
  rdPinUnset = ~rdPinSet;
  #ifdef __AVR__
    *csPort   |=  csPinSet; // Set all control bits to HIGH (idle)
    *cdPort   |=  cdPinSet; // Signals are ACTIVE LOW
    *wrPort   |=  wrPinSet;
    *rdPort   |=  rdPinSet;
  #endif
  #if defined(__SAM3X8E__)
    csPort->PIO_SODR  |=  csPinSet; // Set all control bits to HIGH (idle)
    cdPort->PIO_SODR  |=  cdPinSet; // Signals are ACTIVE LOW
    wrPort->PIO_SODR  |=  wrPinSet;
    rdPort->PIO_SODR  |=  rdPinSet;
  #endif
  pinMode(cs, OUTPUT);    // Enable outputs
  pinMode(cd, OUTPUT);
  pinMode(wr, OUTPUT);
  pinMode(rd, OUTPUT);
  if(reset) {
    digitalWrite(reset, HIGH);
    pinMode(reset, OUTPUT);
  }
#endif*/

  init();
}

// Constructor for shield (fixed LCD control lines)
Adafruit_TFTLCD::Adafruit_TFTLCD(void) : Adafruit_GFX(TFTWIDTH, TFTHEIGHT) {
  init();
}

// Initialization common to both shield & breakout configs
void Adafruit_TFTLCD::init(void) {

//TODO: pin directions here
//TODO: kill.
/*
#ifdef USE_ADAFRUIT_SHIELD_PINOUT
  CS_IDLE; // Set all control bits to idle state
  WR_IDLE;
  RD_IDLE;
  CD_DATA;
  digitalWrite(5, HIGH); // Reset line
  //TODO: convert to AVR
  pinMode(A3, OUTPUT);   // Enable outputs
  pinMode(A2, OUTPUT);
  pinMode(A1, OUTPUT);
  pinMode(A0, OUTPUT);
  pinMode( 5, OUTPUT);
#endif*/


  setWriteDir(); // Set up LCD data port(s) for WRITE operations

  rotation  = 0;
  cursor_y  = cursor_x = 0;
  textsize  = 1;
  textcolor = 0xFFFF;
  _width    = TFTWIDTH;
  _height   = TFTHEIGHT;
}

// Initialization command tables for different LCD controllers
#define TFTLCD_DELAY 0xFF

static const uint8_t HX8357D_regValues[] PROGMEM = {
  HX8357_SWRESET, 0,
  HX8357D_SETC, 3, 0xFF, 0x83, 0x57,
  TFTLCD_DELAY, 250,
  HX8357_SETRGB, 4, 0x00, 0x00, 0x06, 0x06,
  HX8357D_SETCOM, 1, 0x25,  // -1.52V
  HX8357_SETOSC, 1, 0x68,  // Normal mode 70Hz, Idle mode 55 Hz
  HX8357_SETPANEL, 1, 0x05,  // BGR, Gate direction swapped
  HX8357_SETPWR1, 6, 0x00, 0x15, 0x1C, 0x1C, 0x83, 0xAA,
  HX8357D_SETSTBA, 6, 0x50, 0x50, 0x01, 0x3C, 0x1E, 0x08,
  // MEME GAMMA HERE
  HX8357D_SETCYC, 7, 0x02, 0x40, 0x00, 0x2A, 0x2A, 0x0D, 0x78,
  HX8357_COLMOD, 1, 0x55,
  HX8357_MADCTL, 1, 0xC0,
  HX8357_TEON, 1, 0x00,
  HX8357_TEARLINE, 2, 0x00, 0x02,
  HX8357_SLPOUT, 0,
  TFTLCD_DELAY, 150,
  HX8357_DISPON, 0, 
  TFTLCD_DELAY, 50,
};

void Adafruit_TFTLCD::begin(uint16_t id) {
	uint8_t i = 0;

	reset();

	_delay_ms(200);
	// HX8357D
	driver = ID_HX8357D;
	CS_ACTIVE;
	 while(i < sizeof(HX8357D_regValues)) {
	  uint8_t r = pgm_read_byte(&HX8357D_regValues[i++]);
	  uint8_t len = pgm_read_byte(&HX8357D_regValues[i++]);
	  if(r == TFTLCD_DELAY) {
		  //TODO: wut _delay_ms(len);
		_delay_ms(len);
	  } else {
	//Serial.print("Register $"); Serial.print(r, HEX);
	//Serial.print(" datalen "); Serial.println(len);

	CS_ACTIVE;
	CD_COMMAND;
	write8(r);
	CD_DATA;
	for (uint8_t d=0; d<len; d++) {
	  uint8_t x = pgm_read_byte(&HX8357D_regValues[i++]);
	  write8(x);
	}
	CS_IDLE;

	  }
	}
	 return;
}

void Adafruit_TFTLCD::reset(void) {

  CS_IDLE;
//  CD_DATA;
  WR_IDLE;
  RD_IDLE;

//TODO: kill
/*
#ifdef USE_ADAFRUIT_SHIELD_PINOUT
  digitalWrite(5, LOW);
  delay(2);
  digitalWrite(5, HIGH);
#else
  if(_reset) {
    digitalWrite(_reset, LOW);
    delay(2);
    digitalWrite(_reset, HIGH);
  }
#endif
*/

  // Data transfer sync
  CS_ACTIVE;
  CD_COMMAND;
  write8(0x00);
  for(uint8_t i=0; i<3; i++) WR_STROBE; // Three extra 0x00s
  CS_IDLE;
}

// Sets the LCD address window (and address counter, on 932X).
// Relevant to rect/screen fills and H/V lines.  Input coordinates are
// assumed pre-sorted (e.g. x2 >= x1).
void Adafruit_TFTLCD::setAddrWindow(int x1, int y1, int x2, int y2) {
	CS_ACTIVE;
	uint32_t t;

	t = x1;
	t <<= 16;
	t |= x2;
	//TODO move register
	writeRegister32(ILI9341_COLADDRSET, t);  // HX8357D uses same registers!
	t = y1;
	t <<= 16;
	t |= y2;
	//TODO move register
	writeRegister32(ILI9341_PAGEADDRSET, t); // HX8357D uses same registers!
	CS_IDLE;
}

// Unlike the 932X drivers that set the address window to the full screen
// by default (using the address counter for drawPixel operations), the
// 7575 needs the address window set on all graphics operations.  In order
// to save a few register writes on each pixel drawn, the lower-right
// corner of the address window is reset after most fill operations, so
// that drawPixel only needs to change the upper left each time.
/*void Adafruit_TFTLCD::setLR(void) {
  CS_ACTIVE;
  writeRegisterPair(HX8347G_COLADDREND_HI, HX8347G_COLADDREND_LO, _width  - 1);
  writeRegisterPair(HX8347G_ROWADDREND_HI, HX8347G_ROWADDREND_LO, _height - 1);
  CS_IDLE;
}*/

// Fast block fill operation for fillScreen, fillRect, H/V line, etc.
// Requires setAddrWindow() has previously been called to set the fill
// bounds.  'len' is inclusive, MUST be >= 1.
void Adafruit_TFTLCD::flood(uint16_t color, uint32_t len) {
	uint16_t blocks;
	uint8_t  i, hi = color >> 8,
			  lo = color;

	CS_ACTIVE;
	CD_COMMAND;
	write8(HX8357_RAMWR);

	// Write first pixel normally, decrement counter by 1
	CD_DATA;
	write8(hi);
	write8(lo);
	len--;

	blocks = (uint16_t)(len / 64); // 64 pixels/block
	if(hi == lo) {
	// High and low bytes are identical.  Leave prior data
	// on the port(s) and just toggle the write strobe.
	while(blocks--) {
	  i = 16; // 64 pixels/block / 4 pixels/pass
	  do {
		WR_STROBE; WR_STROBE; WR_STROBE; WR_STROBE; // 2 bytes/pixel
		WR_STROBE; WR_STROBE; WR_STROBE; WR_STROBE; // x 4 pixels
	  } while(--i);
	}
	// Fill any remaining pixels (1 to 64)
	for(i = (uint8_t)len & 63; i--; ) {
	  WR_STROBE;
	  WR_STROBE;
	}
	} else {
	while(blocks--) {
	  i = 16; // 64 pixels/block / 4 pixels/pass
	  do {
		write8(hi); write8(lo); write8(hi); write8(lo);
		write8(hi); write8(lo); write8(hi); write8(lo);
	  } while(--i);
	}
	for(i = (uint8_t)len & 63; i--; ) {
	  write8(hi);
	  write8(lo);
	}
	}
	CS_IDLE;
}

/*void Adafruit_TFTLCD::drawFastHLine(int16_t x, int16_t y, int16_t length,
  uint16_t color)
{
	int16_t x2;

	// Initial off-screen clipping
	if((length <= 0     ) ||
		(y      <  0     ) || ( y                  >= _height) ||
		(x      >= _width) || ((x2 = (x+length-1)) <  0      )) return;

	if(x < 0) {        // Clip left
		length += x;
		x       = 0;
	}
	if(x2 >= _width) { // Clip right
		x2      = _width - 1;
		length  = x2 - x + 1;
	}

	setAddrWindow(x, y, x2, y);
	flood(color, length);
	setLR();
}*/

/*void Adafruit_TFTLCD::drawFastVLine(int16_t x, int16_t y, int16_t length,
  uint16_t color)
{
	int16_t y2;

	// Initial off-screen clipping
	if((length <= 0      ) ||
	 (x      <  0      ) || ( x                  >= _width) ||
	 (y      >= _height) || ((y2 = (y+length-1)) <  0     )) return;
	if(y < 0) {         // Clip top
	length += y;
	y       = 0;
	}
	if(y2 >= _height) { // Clip bottom
	y2      = _height - 1;
	length  = y2 - y + 1;
	}

	setAddrWindow(x, y, x, y2);
	flood(color, length);
	setLR();
}*/
/*
void Adafruit_TFTLCD::fillRect(int16_t x1, int16_t y1, int16_t w, int16_t h, 
  uint16_t fillcolor) {
	int16_t  x2, y2;

	// Initial off-screen clipping
	if( (w            <= 0     ) ||  (h             <= 0      ) ||
	  (x1           >= _width) ||  (y1            >= _height) ||
	 ((x2 = x1+w-1) <  0     ) || ((y2  = y1+h-1) <  0      )) return;
	if(x1 < 0) { // Clip left
	w += x1;
	x1 = 0;
	}
	if(y1 < 0) { // Clip top
	h += y1;
	y1 = 0;
	}
	if(x2 >= _width) { // Clip right
	x2 = _width - 1;
	w  = x2 - x1 + 1;
	}
	if(y2 >= _height) { // Clip bottom
	y2 = _height - 1;
	h  = y2 - y1 + 1;
	}

	setAddrWindow(x1, y1, x2, y2);
	flood(fillcolor, (uint32_t)w * (uint32_t)h);
	setLR();
}*/

void Adafruit_TFTLCD::fillScreen(uint16_t color) {
	setAddrWindow(0, 0, _width - 1, _height - 1);
	flood(color, (long)TFTWIDTH * (long)TFTHEIGHT);
}

void Adafruit_TFTLCD::drawPixel(int16_t x, int16_t y, uint16_t color) {
	// Clip
	if((x < 0) || (y < 0) || (x >= _width) || (y >= _height)) return;
	setAddrWindow(x, y, _width-1, _height-1);
	CS_ACTIVE;
	CD_COMMAND; 
	write8(0x2C);
	CD_DATA; 
	write8(color >> 8); write8(color);


	CS_IDLE;
}

// Issues 'raw' an array of 16-bit color values to the LCD; used
// externally by BMP examples.  Assumes that setWindowAddr() has
// previously been set to define the bounds.  Max 255 pixels at
// a time (BMP examples read in small chunks due to limited RAM).
void Adafruit_TFTLCD::pushColors(uint16_t *data, uint8_t len, bool first) {
	uint16_t color;
	uint8_t  hi, lo;
	CS_ACTIVE;
	if(first == true) { // Issue GRAM write command only on first call
		CD_COMMAND;
		write8(0x2C);
	}
	CD_DATA;
	while(len--) {
		color = *data++;
		hi    = color >> 8; // Don't simplify or merge these
		lo    = color;      // lines, there's macro shenanigans
		write8(hi);         // going on.
		write8(lo);
	}
	CS_IDLE;
}

void Adafruit_TFTLCD::setRotation(uint8_t x) {
	// Call parent rotation func first -- sets up rotation flags, etc.
	Adafruit_GFX::setRotation(x);
	// Then perform hardware-specific rotation operations...
	CS_ACTIVE;
	// MEME, HX8357D uses same registers but different values
	uint16_t t;

	switch (rotation) {
	case 2:
		t = ILI9341_MADCTL_MX | ILI9341_MADCTL_BGR;
	break;
	case 3:
		t = ILI9341_MADCTL_MV | ILI9341_MADCTL_BGR;
	break;
	case 0:
		t = ILI9341_MADCTL_MY | ILI9341_MADCTL_BGR;
	break;
	case 1:
		t = ILI9341_MADCTL_MX | ILI9341_MADCTL_MY | ILI9341_MADCTL_MV | ILI9341_MADCTL_BGR;
	 break;
	 default:
		t = 0;
	break;
	}
	writeRegister8(ILI9341_MADCTL, t ); // MADCTL
	setAddrWindow(0, 0, _width - 1, _height - 1); // CS_IDLE happens here

}

#ifdef read8isFunctionalized
  #define read8(x) x=read8fn()
#endif

// Because this function is used infrequently, it configures the ports for
// the read operation, reads the data, then restores the ports to the write
// configuration.  Write operations happen a LOT, so it's advantageous to
// leave the ports in that state as a default.
/*uint16_t Adafruit_TFTLCD::readPixel(int16_t x, int16_t y) {
	if((x < 0) || (y < 0) || (x >= _width) || (y >= _height)) return 0;

	CS_ACTIVE;
	return 0;
}*/

// Ditto with the read/write port directions, as above.
uint16_t Adafruit_TFTLCD::readID(void) {

  uint8_t hi, lo;

  /*
  for (uint8_t i=0; i<128; i++) {
    Serial.print("$"); Serial.print(i, HEX);
    Serial.print(" = 0x"); Serial.println(readReg(i), HEX);
  }
  */

  if (readReg(0x04) == 0x8000) { // eh close enough
    // setc!
    /*
      Serial.println("!");
      for (uint8_t i=0; i<254; i++) {
      Serial.print("$"); Serial.print(i, HEX);
      Serial.print(" = 0x"); Serial.println(readReg(i), HEX);
      }
    */
    writeRegister24(HX8357D_SETC, 0xFF8357);
    _delay_ms(300);
    //Serial.println(readReg(0xD0), HEX);
    if (readReg(0xD0) == 0x990000) {
      return 0x8357;
    }
  }

  uint16_t id = readReg(0xD3);
  if (id == 0x9341) {
    return id;
  }

  CS_ACTIVE;
  CD_COMMAND;
  write8(0x00);
  WR_STROBE;     // Repeat prior byte (0x00)
  setReadDir();  // Set up LCD data port(s) for READ operations
  CD_DATA;
  read8(hi);
  read8(lo);
  setWriteDir();  // Restore LCD data port(s) to WRITE configuration
  CS_IDLE;

  id = hi; id <<= 8; id |= lo;
  return id;
}

uint32_t Adafruit_TFTLCD::readReg(uint8_t r) {
  uint32_t id;
  uint8_t x;

  // try reading register #4
  CS_ACTIVE;
  CD_COMMAND;
  write8(r);
  setReadDir();  // Set up LCD data port(s) for READ operations
  CD_DATA;
  _delay_us(50);
  read8(x);
  id = x;          // Do not merge or otherwise simplify
  id <<= 8;              // these lines.  It's an unfortunate
  read8(x);
  id  |= x;        // shenanigans that are going on.
  id <<= 8;              // these lines.  It's an unfortunate
  read8(x);
  id  |= x;        // shenanigans that are going on.
  id <<= 8;              // these lines.  It's an unfortunate
  read8(x);
  id  |= x;        // shenanigans that are going on.
  CS_IDLE;
  setWriteDir();  // Restore LCD data port(s) to WRITE configuration

  //Serial.print("Read $"); Serial.print(r, HEX); 
  //Serial.print(":\t0x"); Serial.println(id, HEX);
  return id;
}

// Pass 8-bit (each) R,G,B, get back 16-bit packed color
uint16_t Adafruit_TFTLCD::color565(uint8_t r, uint8_t g, uint8_t b) {
  return ((r & 0xF8) << 8) | ((g & 0xFC) << 3) | (b >> 3);
}

// For I/O macros that were left undefined, declare function
// versions that reference the inline macros just once:
//#ifndef write8
//void Adafruit_TFTLCD::write8(uint8_t value) {
//  write8inline(value);
//}
//#endif
/*
#ifdef read8isFunctionalized
uint8_t Adafruit_TFTLCD::read8fn(void) {
  uint8_t result;
  read8inline(result);
  return result;
}
#endif*/

#ifndef setWriteDir
//TODO: look
void Adafruit_TFTLCD::setWriteDir(void) {
  //setWriteDirInline();
  //TODO: ACTUALLY MACRO THIS STUFF
  PORTC.DIRSET = 0xFF;
}
#endif

/*
#ifndef setReadDir
void Adafruit_TFTLCD::setReadDir(void) {
  setReadDirInline();
}
#endif

#ifndef writeRegister8
void Adafruit_TFTLCD::writeRegister8(uint8_t a, uint8_t d) {
  writeRegister8inline(a, d);
}
#endif

#ifndef writeRegister16
void Adafruit_TFTLCD::writeRegister16(uint16_t a, uint16_t d) {
  writeRegister16inline(a, d);
}
#endif
*/
/*#ifndef writeRegisterPair
void Adafruit_TFTLCD::writeRegisterPair(uint8_t aH, uint8_t aL, uint16_t d) {
  writeRegisterPairInline(aH, aL, d);
}
#endif*/



void Adafruit_TFTLCD::writeRegister24(uint8_t r, uint32_t d) {
  CS_ACTIVE;
  CD_COMMAND;
  write8(r);
  CD_DATA;
  _delay_us(10);
  write8(d >> 16);
  _delay_us(10);
  write8(d >> 8);
  _delay_us(10);
  write8(d);
  CS_IDLE;

}


void Adafruit_TFTLCD::writeRegister32(uint8_t r, uint32_t d) {
  CS_ACTIVE;
  CD_COMMAND;
  write8(r);
  CD_DATA;
  _delay_us(10);
  write8(d >> 24);
  _delay_us(10);
  write8(d >> 16);
  _delay_us(10);
  write8(d >> 8);
  _delay_us(10);
  write8(d);
  CS_IDLE;
}
