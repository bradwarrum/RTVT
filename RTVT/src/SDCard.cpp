/*
 * SDCard.cpp
 *
 * Created: 11/3/2014 4:44:12 PM
 *  Author: ee63pc3-user
 */ 

 #include "SDCard.h"

 DWORD get_fattime() {
	return 10200930;
 }
 DSTATUS disk_initialize (BYTE pdrv) {
	return sdcard.init(pdrv);
 }
 DSTATUS disk_status (BYTE pdrv){
	if (sdcard.IDMatch(pdrv))
		return sdcard.getStatus();
	return STA_NOINIT;
 }
 DRESULT disk_read (BYTE pdrv, BYTE* buff, DWORD sector, UINT count){
	if (!sdcard.IDMatch(pdrv) || !count) return RES_PARERR;
	return sdcard.read(buff, sector, count);
 }
 DRESULT disk_write (BYTE pdrv, const BYTE* buff, DWORD sector, UINT count){
	if (!sdcard.IDMatch(pdrv)) return RES_PARERR;
	return sdcard.write(buff, sector, count);
 }
 DRESULT disk_ioctl (BYTE pdrv, BYTE cmd, void* buff){
	if (!sdcard.IDMatch(pdrv)) return RES_PARERR;
	return sdcard.IOctl(cmd, buff);
 }

 bool SDCard::IDMatch(BYTE pdrv) {return pdrv == drvID;}
 SDCard::SDCard(SPI_t * onPort) : spi(onPort) {
	Stat = STA_NOINIT;
	SPI_PORT.DIR &= ~(SPI_CD_BM | SPI_WP_BM);
	//Check status of card detect
	if (SPI_PORT.IN & SPI_CD_BM) Stat |= STA_NODISK;
	spi.enable();
 }
 DSTATUS SDCard::getStatus() {
	return Stat;
 }
 DSTATUS SDCard::init(BYTE pdrv) {
	BYTE n, cmd, ty, ocr[4];

	drvID = pdrv;
	if (Stat & STA_NODISK) return Stat;

	spi.clockRate(0x03); // 250 kHz
	deselect();
	for (n = 10; n; n--) spi.get(); //80 clock cycles
	ty = 0;
	for (n = 16; n; n--) {
		if (send_cmd(CMD0, 0) == 1) {
			if (send_cmd(CMD8, 0x1AA) == 1) {
				for (n = 0; n<4; n++) ocr[n] = spi.get();
				if (ocr[2] == 0x01 && ocr[3] == 0xAA) {
					while(send_cmd(ACMD41, (DWORD)0x40000000));
					if (send_cmd(CMD58, 0) == 0) {
						for (n=0; n<4; n++) ocr[n] = spi.get();
						ty = (ocr[0] & 0x40) ? CT_SD2 | CT_BLOCK : CT_SD2;
					}
				}
			} else {
				if (send_cmd(ACMD41, 0) <= 1) {
					ty = CT_SD1; cmd = ACMD41;
				} else {
					ty = CT_MMC; cmd = CMD1;
				}
				while (send_cmd(cmd, 0));
				if (send_cmd(CMD16, (DWORD)512) != 0) ty = 0;
			}
			break;
		}
		
	}
	CardType = ty;
	deselect();

	if (ty) {
		Stat &= ~STA_NOINIT;
		spi.clockRate(0x81); //4MHz
	}
	return Stat;


}
 DRESULT SDCard::read(BYTE* buff, DWORD sector, UINT count) {
	if (Stat & STA_NOINIT) return RES_NOTRDY;

	if (!(CardType & CT_BLOCK)) sector *= 512;

	if (count == 1) {
		if ((send_cmd(CMD17, sector) == 0) && rcv_dataBlock(buff, 512)) count = 0;
	}
	else {
		if (send_cmd(CMD18, sector) == 0) {
			do {
				if (!rcv_dataBlock(buff, 512)) break;
				buff += 512;
			} while (--count);
			send_cmd(CMD12, 0);
		}
	}
	deselect();
	return count ? RES_ERROR : RES_OK;
 }
 #if _FS_READONLY == 0
 DRESULT SDCard::write(const BYTE* buff, DWORD sector, UINT count) {
	if (Stat & STA_NOINIT) return RES_NOTRDY;
	if (Stat & STA_PROTECT) return RES_WRPRT;

	if (!(CardType & CT_BLOCK)) sector *= 512;

	if (count == 1) {
		if ((send_cmd(CMD24, sector) == 0) && send_dataBlock(buff, 0xFE)) count = 0;
	} else {
		if (CardType & CT_SDC) send_cmd(ACMD23, count);
		if (send_cmd(CMD25, sector) == 0) {
			do {
				if (!send_dataBlock(buff, 0xFC)) break;
				buff += 512;
			} while (--count);
			if (!send_dataBlock(0, 0xFD)) count = 1;
		}
	}

	deselect();
	return count ? RES_ERROR:RES_OK;
 }
 #endif
 #if _USE_IOCTL != 0
 DRESULT SDCard::IOctl(BYTE ctrl, void* buff){
	DRESULT res;
	BYTE n, csd[16];
	BYTE *ptr = (BYTE *)buff;
	WORD csize;

	res = RES_ERROR;
	if (ctrl == CTRL_POWER) {
		switch(*ptr) {
		case 0:
			res = RES_OK;
			break;
		case 1:
			res = RES_OK;
			break;
		case 2:
			*(ptr + 1) = (BYTE) 1;
			res = RES_OK;
			break;
		default:
			res = RES_PARERR;
		}
	} else {
		if (Stat & STA_NOINIT) return RES_NOTRDY;

		switch(ctrl) {
			case CTRL_SYNC:
				if (select()) {
					deselect();
					res = RES_OK;
				}
				break;
			case GET_SECTOR_COUNT:
				if ((send_cmd(CMD9, 0) == 0) && rcv_dataBlock(csd, 16)) {
					if ((csd[0] >> 6) == 1) {
						csize = csd[9] + ((WORD) csd[8] << 8) + 1;
						*(DWORD*)buff = (DWORD)csize << 10;
					} else {
						n=(csd[5] & 15) + ((csd[10] & 128) >> 7) + ((csd[9] & 3) << 1) + 2;
						csize = (csd[8] >> 6) + ((WORD)csd[7] << 2) + ((WORD)(csd[6] & 3) << 10) + 1;
						*(DWORD*)buff = (DWORD)csize << (n-9);
					}
					res = RES_OK;
				}
				break;
			case GET_SECTOR_SIZE:
				*(WORD*)buff = 512;
				res = RES_OK;
				break;
			case GET_BLOCK_SIZE:
				if (CardType & CT_SD2) {
					if (send_cmd(ACMD13, 0) == 0) {
						spi.get();
						if (rcv_dataBlock(csd,16)) {
							for (n = 64 - 16; n; n--) spi.get();
							*(DWORD*) buff = (DWORD)0x00000010 << (csd[10] >> 4);
							res = RES_OK;
						}
					}
				} else {
					if ((send_cmd(CMD9, 0) == 0) && rcv_dataBlock(csd,16)) {
						if (CardType & CT_SD1) {
							*(DWORD*)buff = (((csd[10] & 63) << 1) + ((WORD)(csd[11] & 128) >> 7) + 1) << ((csd[13] >> 6) - 1);
						} else {
							*(DWORD*)buff = ((WORD)((csd[10] & 124) >> 2) + 1) * (((csd[11] & 3) << 3) + ((csd[11] & 224) >> 5) + 1);
						}
						res = RES_OK;
					}
				}
				break;
			case MMC_GET_TYPE:
				*ptr = CardType;
				res = RES_OK;
				break;
			case MMC_GET_CSD:
				if (send_cmd(CMD9, 0) == 0 && rcv_dataBlock(ptr, 16)) res = RES_OK;
				break;
			case MMC_GET_CID:
				if (send_cmd(CMD10, 0) == 0 && rcv_dataBlock(ptr, 16)) res = RES_OK;
				break;
			case MMC_GET_OCR:
				if (send_cmd(CMD58, 0) == 0) {
					for (n=4; n; n--) *ptr++ = spi.get();
					res = RES_OK;
				}
				break;
			case MMC_GET_SDSTAT:
				if (send_cmd(ACMD13, 0) == 0) {
					spi.get();
					if (rcv_dataBlock(ptr, 64)) res = RES_OK;
				}
				break;
			default:
				res = RES_PARERR;
			}
			deselect();
		}

		return res;
 }
 #endif

//SD Helper Functions
BYTE SDCard::send_cmd(BYTE cmd, DWORD arg) {
	BYTE n, res;
	//spi.get();
	//spi.get();
	
	if (cmd & 0x80) {
		cmd &= 0x7F;
		res = send_cmd(CMD55, 0);
		if (res > 1) return res;
	}

	
	if (cmd != CMD0) {
		deselect();
		if (!select()) return 0xFF;
	} else {
		SPI_DESELECT;
		SPI_SELECT;
	}
	spi.put(0x40 | cmd);
	spi.put((BYTE)(arg >> 24));
	spi.put((BYTE)(arg >> 16));
	spi.put((BYTE)(arg >> 8));
	spi.put((BYTE)arg);
	n = 0x01;
	if (cmd == CMD0) n = 0x95;
	if (cmd == CMD8) n = 0x87;
	spi.put(n);

	if (cmd == CMD12) spi.get();
	n = 10;
	do 
	{
		res = spi.get();
	} while ((res & 0x80) && --n);

	return res;
}
int SDCard::rcv_dataBlock(BYTE * buff, UINT btr){
	BYTE token;
	do {
		token = spi.get();
	} while((token == 0xFF));
	if (token != 0xFE) return 0;

	do {
		spi.get_buffered(buff++);
		spi.get_buffered(buff++);
		spi.get_buffered(buff++);
		spi.get_buffered(buff++);
	} while (btr -= 4);
	spi.get();
	spi.get();

	return 1;
}

int SDCard::send_dataBlock(const BYTE * buff, BYTE token) {
	BYTE resp,wc;

	if (!wait_ready()) return 0;

	spi.put(token);
	if (token != 0xFD) {
		wc = 0;
		do {
			spi.put(*buff++);
			spi.put(*buff++);
		} while (--wc);
		spi.put(0xFF);
		spi.put(0xFF);
		resp = spi.get();
		if ((resp & 0x1F) != 0x05) return 0;
	}
	return 1;
}

int SDCard::wait_ready() {
	BYTE n = 0xFF;
	spi.get();
	for (;n;n--) {
		if (spi.get() == 0xFF) return 1;
	}
	return 0;
}

int SDCard::select() {
	SPI_SELECT;
	if (!wait_ready()) {
		deselect();
		return 0;
	}
	return 1;
}