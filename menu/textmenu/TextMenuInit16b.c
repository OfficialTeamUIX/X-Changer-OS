/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#include "include/config.h"
#include "TextMenu.h"

#include "VideoInitialization.h"
#include "MenuActions.h"

TEXTMENU *TextMenuInit(void) {
	
	TEXTMENUITEM *itemPtr;
	TEXTMENU *menuPtr;
	
	//Create the root menu - MANDATORY
	menuPtr = malloc(sizeof(TEXTMENU));
	strcpy(menuPtr->szCaption, "XC OS for XBOX 1.6b");
	menuPtr->firstMenuItem=NULL;
	
	//Boot Originalbios
	itemPtr = (TEXTMENUITEM*)malloc(sizeof(TEXTMENUITEM));
	memset(itemPtr,0x00,sizeof(TEXTMENUITEM));
	strcpy(itemPtr->szCaption, "Boot Original Bios");
	itemPtr->functionPtr = BootOriginalBios;
	itemPtr->functionDataPtr = NULL;
	TextMenuAddItem(menuPtr, itemPtr);
	
	//Boot 256k Modbios
	itemPtr = (TEXTMENUITEM*)malloc(sizeof(TEXTMENUITEM));
	memset(itemPtr,0x00,sizeof(TEXTMENUITEM));
	strcpy(itemPtr->szCaption, "Boot Bank A (256k)");
	itemPtr->functionPtr = BootModBios;
	itemPtr->functionDataPtr = NULL;
	TextMenuAddItem(menuPtr, itemPtr);

	//Boot 512k Modbios
	itemPtr = (TEXTMENUITEM*)malloc(sizeof(TEXTMENUITEM));
	memset(itemPtr,0x00,sizeof(TEXTMENUITEM));
	strcpy(itemPtr->szCaption, "Boot Bank B (512k)");
	itemPtr->functionPtr = BootModBios2;
	itemPtr->functionDataPtr = NULL;
	TextMenuAddItem(menuPtr, itemPtr);
	
	//Advanced Menu
	itemPtr = (TEXTMENUITEM*)malloc(sizeof(TEXTMENUITEM));
	memset(itemPtr,0x00,sizeof(TEXTMENUITEM));
	strcpy(itemPtr->szCaption, "Advanced Menu");
	itemPtr->functionPtr=DrawChildTextMenu;
	itemPtr->functionDataPtr = (void *)AdvancedMenuInit();
	TextMenuAddItem(menuPtr, itemPtr);
	
	return menuPtr;
}
