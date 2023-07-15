/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#include "include/boot.h"
#include "TextMenu.h"
#include "VideoInitialization.h"

void InitFatXIcons(TEXTMENU *menuPtr);
void InitNativeIcons(TEXTMENU *menuPtr);

TEXTMENU *LinuxMenuInit(void) {
	TEXTMENUITEM *itemPtr;
	TEXTMENU *menuPtr;
	int i;
	
	menuPtr = (TEXTMENU*)malloc(sizeof(TEXTMENU));
	memset(menuPtr,0x00,sizeof(TEXTMENU));
	strcpy(menuPtr->szCaption, "Linux Boot Menu");

	for (i=0; i<2; ++i) {
		//Add the cdrom icon - if you have two cdroms, you'll get two icons!
		if (tsaHarddiskInfo[i].m_fAtapi) {
			char *driveName=malloc(sizeof(char)*14);
			sprintf(driveName,"CD-ROM (hd%c)",i ? 'b':'a');
			itemPtr = (TEXTMENUITEM*)malloc(sizeof(TEXTMENUITEM));
			memset(itemPtr,0x00,sizeof(TEXTMENUITEM));
			strcpy(itemPtr->szCaption, driveName);
			itemPtr->functionPtr = BootFromCD;
			itemPtr->functionDataPtr = malloc(sizeof(int));
			*(int*)itemPtr->functionDataPtr = i;
			TextMenuAddItem(menuPtr, itemPtr);
		}
	}
	//Load the config file from FATX and native, and add the icons, if found.
	InitFatXIcons(menuPtr);
	InitNativeIcons(menuPtr);

#if 0
//#ifdef ETHERBOOT
	//Etherboot icon - if it's compiled in, it's always available.
	itemPtr = (TEXTMENUITEM*)malloc(sizeof(TEXTMENUITEM));
	memset(itemPtr,0x00,sizeof(TEXTMENUITEM));
	strcpy(itemPtr->szCaption, "Etherboot");
	itemPtr->functionPtr = BootFromEtherboot;
	TextMenuAddItem(menuPtr, itemPtr);
#endif	
	
	return menuPtr;
}

void InitFatXIcons(TEXTMENU *menuPtr) {
	TEXTMENUITEM *itemPtr;
	u8 ba[512];
	int driveId=0;
	
	if (tsaHarddiskInfo[driveId].m_fDriveExists && !tsaHarddiskInfo[driveId].m_fAtapi) {
		memset(ba,0x00,512);
		BootIdeReadSector(driveId, ba, 3, 0, 512);
		if (!strncmp("BRFR",ba,4)) {
			//Got a FATX formatted HDD
			CONFIGENTRY *entry = (CONFIGENTRY*)LoadConfigFatX();
			if (entry !=NULL) {
				//There is a config file present.
				itemPtr = (TEXTMENUITEM*)malloc(sizeof(TEXTMENUITEM));
				memset(itemPtr,0x00,sizeof(TEXTMENUITEM));
				strcpy(itemPtr->szCaption, "FatX (E:)");
				itemPtr->functionPtr = DrawBootMenu;
				itemPtr->functionDataPtr = (void *)entry;
				TextMenuAddItem(menuPtr, itemPtr);
			}
		}
	}
}

void InitNativeIcons(TEXTMENU *menuPtr) {
	TEXTMENUITEM *itemPtr;
	u8 ba[512];
	int driveId;	

	for (driveId=0; driveId<2; driveId++) {
		if (tsaHarddiskInfo[driveId].m_fDriveExists && !tsaHarddiskInfo[driveId].m_fAtapi) {
			volatile u8 *pb;
			int n=0, nPos=0;
			
			memset(ba,0x00,512);
			BootIdeReadSector(driveId, ba, 0, 0, 512);
			        
			//See if there is an MBR - no MBR means no native boot options.
			if( !(ba[0x1fe]==0x55) || !(ba[0x1ff]==0xaa)) return;
	
			(volatile u8 *)pb=&ba[0x1be];
			//Check the primary partitions
			for (n=0; n<4; n++,pb+=16) {
				if(pb[0]&0x80) {
					//Bootable flag IS set on this partition.
					CONFIGENTRY *entry = (CONFIGENTRY*)LoadConfigNative(driveId, n);
					if (entry!=NULL) {
						//There is a valid config file here.
						//Add an icon for this partition 
						char *driveName=malloc(sizeof(char)*14);
						sprintf(driveName, "hd%c%d", driveId+'a', n);
						itemPtr = (TEXTMENUITEM*)malloc(sizeof(TEXTMENUITEM));
						memset(itemPtr,0x00,sizeof(TEXTMENUITEM));
						strcpy(itemPtr->szCaption, driveName);
						itemPtr->functionPtr = DrawBootMenu;
						itemPtr->functionDataPtr = (void *)entry;
						TextMenuAddItem(menuPtr, itemPtr);
					}
				}
			}
			
		}
	}
}

