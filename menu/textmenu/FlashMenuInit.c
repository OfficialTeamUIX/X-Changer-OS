/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#include "include/boot.h"
#include "BootIde.h"
#include "TextMenu.h"
#include "FlashMenuActions.h"

TEXTMENU* FlashMenuInit(void) {
	TEXTMENUITEM *itemPtr;
	TEXTMENU *menuPtr;
	int i=0;

	menuPtr = (TEXTMENU*)malloc(sizeof(TEXTMENU));
	memset(menuPtr,0x00,sizeof(TEXTMENU));
	strcpy(menuPtr->szCaption, "Flash Menu");
	
	for (i=0; i<2; ++i) {
		if (tsaHarddiskInfo[i].m_fDriveExists && tsaHarddiskInfo[i].m_fAtapi) {
   			char *driveName=malloc(sizeof(char)*32);
			itemPtr = (TEXTMENUITEM*)malloc(sizeof(TEXTMENUITEM));
			memset(itemPtr,0x00,sizeof(TEXTMENUITEM));
                        sprintf(itemPtr->szCaption,"Flash Bank A from CD/DVD");
			itemPtr->functionPtr= FlashBiosFromCD;
    			itemPtr->functionDataPtr = malloc(sizeof(int));
                        *(int*)itemPtr->functionDataPtr = i;
			TextMenuAddItem(menuPtr, itemPtr);
		}
	}
	
	for (i=0; i<2; ++i) {
		if (tsaHarddiskInfo[i].m_fDriveExists && tsaHarddiskInfo[i].m_fAtapi) {
   			char *driveName=malloc(sizeof(char)*32);
			itemPtr = (TEXTMENUITEM*)malloc(sizeof(TEXTMENUITEM));
			memset(itemPtr,0x00,sizeof(TEXTMENUITEM));
                        sprintf(itemPtr->szCaption,"Flash Bank B from CD/DVD");
			itemPtr->functionPtr= FlashBiosFromCD2;
    			itemPtr->functionDataPtr = malloc(sizeof(int));
                        *(int*)itemPtr->functionDataPtr = i;
			TextMenuAddItem(menuPtr, itemPtr);
		}
	}
	
	itemPtr = (TEXTMENUITEM*)malloc(sizeof(TEXTMENUITEM));
	memset(itemPtr,0x00,sizeof(TEXTMENUITEM));
	strcpy(itemPtr->szCaption, "Flash Bank A via Network");
	itemPtr->functionPtr= NetworkFlashBios;
	itemPtr->functionDataPtr = NULL;
	TextMenuAddItem(menuPtr, itemPtr);

	itemPtr = (TEXTMENUITEM*)malloc(sizeof(TEXTMENUITEM));
	memset(itemPtr,0x00,sizeof(TEXTMENUITEM));
	strcpy(itemPtr->szCaption, "Flash Bank B via Network");
	itemPtr->functionPtr= NetworkFlashBios2;
	itemPtr->functionDataPtr = NULL;
	TextMenuAddItem(menuPtr, itemPtr);
	
	for (i=0; i<2; ++i) {
		if (tsaHarddiskInfo[i].m_fDriveExists && tsaHarddiskInfo[i].m_fAtapi) {
   			char *driveName=malloc(sizeof(char)*32);
			itemPtr = (TEXTMENUITEM*)malloc(sizeof(TEXTMENUITEM));
			memset(itemPtr,0x00,sizeof(TEXTMENUITEM));
                        sprintf(itemPtr->szCaption,"Update OS from CD-ROM");
			itemPtr->functionPtr= UpdateFlashFromCD;
    			itemPtr->functionDataPtr = malloc(sizeof(int));
                        *(int*)itemPtr->functionDataPtr = i;
			TextMenuAddItem(menuPtr, itemPtr);
		}
	}
	
	itemPtr = (TEXTMENUITEM*)malloc(sizeof(TEXTMENUITEM));
	memset(itemPtr,0x00,sizeof(TEXTMENUITEM));
	strcpy(itemPtr->szCaption, "Update OS via Network");
	itemPtr->functionPtr= NetworkUpdate;
	itemPtr->functionDataPtr = NULL;
	TextMenuAddItem(menuPtr, itemPtr);
	
	return menuPtr;
}
