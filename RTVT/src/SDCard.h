/*
 * SDCard.h
 *
 * Created: 11/3/2014 4:43:56 PM
 *  Author: ee63pc3-user
 */ 


#ifndef SDCARD_H_
#define SDCARD_H_
#include "diskio.h"
#include "SPI_Driver.h"
#include "macro.h"
#include "ffconf.h"
#include "ff.h"


/* Definitions for MMC/SDC command */
#define CMD0	(0x40+0)	// GO_IDLE_STATE
#define CMD1	(0x40+1)	// SEND_OP_COND (MMC)
#define	ACMD41	(0xC0+41)	// SEND_OP_COND (SDC)
#define CMD8	(0x40+8)	// SEND_IF_COND
#define CMD9	(0x40+9)	// SEND_CSD
#define CMD10	(0x40+10)	// SEND_CID
#define CMD12	(0x40+12)	// STOP_TRANSMISSION
#define ACMD13	(0xC0+13)	// SD_STATUS (SDC)
#define CMD16	(0x40+16)	// SET_BLOCKLEN
#define CMD17	(0x40+17)	// READ_SINGLE_BLOCK
#define CMD18	(0x40+18)	// READ_MULTIPLE_BLOCK
#define CMD23	(0x40+23)	// SET_BLOCK_COUNT (MMC)
#define	ACMD23	(0xC0+23)	// SET_WR_BLK_ERASE_COUNT (SDC)
#define CMD24	(0x40+24)	// WRITE_BLOCK
#define CMD25	(0x40+25)	// WRITE_MULTIPLE_BLOCK
#define CMD55	(0x40+55)	// APP_CMD
#define CMD58	(0x40+58)	// READ_OCR

#define deselect() {SPI_DESELECT; spi.get();}



class SDCard {
private:
	SPI_Driver spi;
	BYTE drvID, CardType;
	DSTATUS Stat;
	BYTE send_cmd(BYTE cmd, DWORD arg);
	int rcv_dataBlock(BYTE * buff, UINT btr);
	int send_dataBlock(const BYTE * buff, BYTE token);
	int wait_ready();
	int select();
public:
	SDCard(SPI_t* onPort);
	bool IDMatch(BYTE pdrv);
	DSTATUS init(BYTE pdrv);
	DSTATUS getStatus();
	DRESULT read(BYTE* buff, DWORD sector, UINT count);
	DRESULT write(const BYTE* buff, DWORD sector, UINT count);
	DRESULT IOctl(BYTE ctrl, void* buff);
};
SDCard sdcard(&SPIE);





#endif /* SDCARD_H_ */