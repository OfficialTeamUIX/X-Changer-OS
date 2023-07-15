/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/
#include "HddMenuActions.h"

/*
void ResetAfterAction(void) {
	int i;
	VIDEO_ATTR=0xffffff;
	printk("\nReboot required! XBOX will reboot in 5... ");
	for (i = 4; i > 0; i--) {
		printk("%d... ", i);
		wait_ms(1000);
	}
	printk("powering XBOX down");
	wait_ms(250);
	I2CRebootSlow(); // Power Cycle
	while(1);
} */

void LockHdd(void *driveId) {
	int nIndexDrive = *(int *)driveId;
	u8 password[20];
	unsigned uIoBase = tsaHarddiskInfo[nIndexDrive].m_fwPortBase;
	int i;

	if (!Confirm("Confirm locking of drive", "Yes", "No", 0)) return;	
	
	if (CalculateDrivePassword(nIndexDrive,password)) {
		VIDEO_ATTR=TEXTCOLOR;
		printk("Unable to calculate drive password - eeprom corrupt?");
		return;
	}
	VIDEO_ATTR=TEXTCOLOR;
	printk("Locking drive\n\n");
	printk("Cromwell locks drives with a master password of \"\2XBOXLINUX\2\"\n\nPlease remember this,");
	printk("as it could save your drive!\n\n");
	for (i=0; i < 10; i++) printk("\n");
	printk("The normal password (user password) the drive is being locked with is as follows:\n\n");
	printk("                              ");
	for (i=0; i<20; i++) {
		printk("\2%02x \2",password[i]);
		if ((i+1)%5 == 0) {
			printk("\n\n                              ");
		}
	}	
	//printk("\nLocking drive - please wait\n");
	VIDEO_ATTR=TEXTCOLOR;
	printk("\nLocking drive ... ");
	if (DriveSecurityChange(uIoBase, nIndexDrive, IDE_CMD_SECURITY_SET_PASSWORD, password)) {
		// printk("Lock command failed!");
		printk("failed!\n");
	} else printk("successful!\n");
	printk("Make a note of the password above, and ");
	printk("press Button A to reboot XBOX");

	while ((risefall_xpad_BUTTON(TRIGGER_XPAD_KEY_A) != 1)) wait_ms(100);
	wait_ms(250);
	I2CRebootSlow(); // Power Cycle
	while(1);
}

void UnlockHdd(void *driveId) {
	int nIndexDrive = *(int *)driveId;
	u8 password[20];
	unsigned uIoBase = tsaHarddiskInfo[nIndexDrive].m_fwPortBase;
	
	if (!Confirm("Confirm unlocking of drive", "Yes", "No", 0)) return;	
	
	if (CalculateDrivePassword(nIndexDrive,password)) {
		VIDEO_ATTR=TEXTCOLOR;
		printk("Unable to calculate drive password - eeprom corrupt?  Aborting\n");
		return;
	}
	if (DriveSecurityChange(uIoBase, nIndexDrive, IDE_CMD_SECURITY_DISABLE, password)) {
		VIDEO_ATTR=TEXTCOLOR;
		printk("Failed!");
	}
	VIDEO_ATTR=TEXTCOLOR;
	printk("\2\n\2\n\2This drive is now unlocked.\n\n");
	printk("\2Press Button A to reboot XBOX");

	while ((risefall_xpad_BUTTON(TRIGGER_XPAD_KEY_A) != 1)) wait_ms(100);
	wait_ms(250);
	I2CRebootSlow(); // Power Cycle
	while(1);
}


void DisplayHddPassword(void *driveId) {
	int nIndexDrive = *(int *)driveId;
	u8 password[20];
	int i;
	
	if (CalculateDrivePassword(nIndexDrive,password)) {
		VIDEO_ATTR=TEXTCOLOR;
		printk("Unable to calculate drive password - eeprom corrupt?");
		return;
	}
	
	VIDEO_ATTR=TEXTCOLOR;
	for (i=0; i < 16; i++) printk("\n");
	printk("The normal password (user password) for this drive is as follows:\n\n");
	printk("                              ");
	
	for (i=0; i<20; i++) {
		printk("\2%02x \2",password[i]);
		if ((i+1)%5 == 0) {
			printk("\n\n                              ");
		}
	}	
	printk("\n\nPress Button A to continue");

	while ((risefall_xpad_BUTTON(TRIGGER_XPAD_KEY_A) != 1)) wait_ms(100);
}
