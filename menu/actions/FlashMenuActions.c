/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#include "FlashMenuActions.h"

extern int GlobalExpectedFlashFileSize;

void FlashBiosFromCD(void *cdromId) {
#ifdef FLASH
	WriteToIO(0x1912, 0x01);	// switch to 256k user bank
	BootLoadFlashCD(*(int *)cdromId, 0x40000);
	#ifndef NOANI_MENU
	  I2CTransmitWord(0x10, 0x1b00 + ( I2CTransmitByteGetReturn(0x10, 0x1b) & 0xfb )); // clear noani-bit
	#endif
	#ifdef NOANI_MENU
	  I2CTransmitWord(0x10, 0x1b00 + ( I2CTransmitByteGetReturn(0x10, 0x1b) | 0x04 )); // set noani-bit
	#endif
	I2CTransmitWord(0x10, 0x0201); // start new Bios :-)
	while(1);
#endif
}

void FlashBiosFromCD2(void *cdromId) {
#ifdef FLASH
	WriteToIO(0x1912, 0x02);	// switch to 512k user bank
	BootLoadFlashCD(*(int *)cdromId, 0x80000);
	#ifndef NOANI_MENU
	  I2CTransmitWord(0x10, 0x1b00 + ( I2CTransmitByteGetReturn(0x10, 0x1b) & 0xfb )); // clear noani-bit
	#endif
	#ifdef NOANI_MENU
	  I2CTransmitWord(0x10, 0x1b00 + ( I2CTransmitByteGetReturn(0x10, 0x1b) | 0x04 )); // set noani-bit
	#endif
	I2CTransmitWord(0x10, 0x0201); // start new Bios :-)
	while(1);
#endif
}

void NetworkFlashBios(void *dummy) {
#ifdef FLASH
	WriteToIO(0x1912, 0x01);	// switch to 256k user bank
	GlobalExpectedFlashFileSize = 0x40000;
	etherboot();
	printk("Something seems to be wrong! - Halting!");
	while(1);	
#endif
}

void NetworkFlashBios2(void *dummy) {
#ifdef FLASH
	WriteToIO(0x1912, 0x02);	// switch to 512k user bank
	GlobalExpectedFlashFileSize = 0x80000;
	etherboot();
	printk("Something seems to be wrong! - Halting!");
	while(1);	
#endif
}

void NetworkUpdate(void *dummy) {
#ifdef FLASH
	WriteToIO(0x1912, 0x00);	// switch to OS bank
	GlobalExpectedFlashFileSize = 0x40000;
	etherboot();
	printk("Something seems to be wrong! - Halting!");
	while(1);	
#endif
}

void UpdateFlashFromCD(void *cdromId) {
#ifdef FLASH
	WriteToIO(0x1912, 0x00);	// switch to OS bank
	BootLoadUpdateFlash(*(int *)cdromId);
	I2CTransmitWord(0x10, 0x0201); // start new Bios :-)
	while(1);
#endif
}
