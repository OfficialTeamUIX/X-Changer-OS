/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#include "FlashMenuActions.h"

void FlashBiosFromCD(void *cdromId) {
#ifdef FLASH
	WriteStatusRegister(0x1);      // Switch to user bios bank
	BootLoadFlashCD(*(int *)cdromId);
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
	WriteStatusRegister(0x1);      // Switch to user bios bank
	etherboot();
	printk("Something seems to be wrong! - Halting!");
	while(1);	
#endif
}

void UpdateFlashFromCD(void *cdromId) {
#ifdef FLASH
	WriteStatusRegister(0x0);      // Switch to bootloader bank
	BootLoadUpdateFlash(*(int *)cdromId);
	I2CTransmitWord(0x10, 0x0201); // start new Bios :-)
	while(1);
#endif
}
