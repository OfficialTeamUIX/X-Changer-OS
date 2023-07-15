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

TEXTMENU* AdvancedMenuInit(void) {
	TEXTMENUITEM *itemPtr;
	TEXTMENU *menuPtr;
	int i=0;

	menuPtr = (TEXTMENU*)malloc(sizeof(TEXTMENU));
	memset(menuPtr,0x00,sizeof(TEXTMENU));
	strcpy(menuPtr->szCaption, "Advanced Menu");
	
	//Linux Boot Menu moved from Iconmenu
	itemPtr = (TEXTMENUITEM*)malloc(sizeof(TEXTMENUITEM));
	memset(itemPtr,0x00,sizeof(TEXTMENUITEM));
	strcpy(itemPtr->szCaption, "Linux Boot Menu");
	itemPtr->functionPtr=DrawChildTextMenu;
	itemPtr->functionDataPtr = (void *)LinuxMenuInit();
	TextMenuAddItem(menuPtr, itemPtr);
	
	//LED control menu
	itemPtr = (TEXTMENUITEM*)malloc(sizeof(TEXTMENUITEM));
	memset(itemPtr,0x00,sizeof(TEXTMENUITEM));
	strcpy(itemPtr->szCaption, "LED Control Menu");
	itemPtr->functionPtr=DrawChildTextMenu;
	itemPtr->functionDataPtr = (void *)LEDMenuInit();
	TextMenuAddItem(menuPtr, itemPtr);
	
	//VIDEO SETTINGS MENU
	itemPtr = (TEXTMENUITEM*)malloc(sizeof(TEXTMENUITEM));
	memset(itemPtr,0x00,sizeof(TEXTMENUITEM));
	strcpy(itemPtr->szCaption, "Video Settings");
	itemPtr->functionPtr=DrawChildTextMenu;
	itemPtr->functionDataPtr = (void *)VideoMenuInit();
	TextMenuAddItem(menuPtr, itemPtr);

	//HDD MENU
	itemPtr = (TEXTMENUITEM*)malloc(sizeof(TEXTMENUITEM));
	memset(itemPtr,0x00,sizeof(TEXTMENUITEM));
	strcpy(itemPtr->szCaption, "Hdd Menu");
	itemPtr->functionPtr=DrawChildTextMenu;
	itemPtr->functionDataPtr = (void *)HddMenuInit();
	TextMenuAddItem(menuPtr, itemPtr);
	
#ifdef FLASH
	//FLASH MENU
	itemPtr = (TEXTMENUITEM*)malloc(sizeof(TEXTMENUITEM));
	memset(itemPtr,0x00,sizeof(TEXTMENUITEM));
	strcpy(itemPtr->szCaption, "Flash Menu");
	itemPtr->functionPtr=DrawChildTextMenu;
	itemPtr->functionDataPtr = (void *)FlashMenuInit();
	TextMenuAddItem(menuPtr, itemPtr);
#endif
	itemPtr = (TEXTMENUITEM*)malloc(sizeof(TEXTMENUITEM));
	memset(itemPtr,0x00,sizeof(TEXTMENUITEM));
	strcpy(itemPtr->szCaption, "Reset Menu");
	itemPtr->functionPtr=DrawChildTextMenu;
	itemPtr->functionDataPtr = (void *)ResetMenuInit();
	TextMenuAddItem(menuPtr, itemPtr);
	
	return menuPtr;
}
