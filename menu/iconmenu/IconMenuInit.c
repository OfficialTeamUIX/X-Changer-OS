/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

/* This is where you should customise the menu, by adding your own icons.
 * The code in IconMenu.c should normally be left alone.
 */
#include "include/config.h"
#include "BootIde.h"
#include "IconMenu.h"
#include "MenuActions.h"

void IconMenuInit(void) {
	ICON *iconPtr=NULL;
	
	iconPtr = (ICON *)malloc(sizeof(ICON));
	iconPtr->iconSlot = ICON_SOURCE_SLOT0;
	iconPtr->szCaption = "Original";
	//iconPtr->szCaption = "";
	iconPtr->functionPtr = BootOriginalBios;
	AddIcon(iconPtr);

	iconPtr = (ICON *)malloc(sizeof(ICON));
	iconPtr->iconSlot = ICON_SOURCE_SLOT1;
	iconPtr->szCaption = "Modbios";
	//iconPtr->szCaption = "";
	iconPtr->functionPtr = BootModBios;
	selectedIcon = iconPtr;
	AddIcon(iconPtr);
	
	iconPtr = (ICON *)malloc(sizeof(ICON));
	iconPtr->iconSlot = ICON_SOURCE_SLOT2;
	iconPtr->szCaption = "Advanced";
	//iconPtr->szCaption = "";
	iconPtr->functionPtr = AdvancedMenu;
	iconPtr->functionDataPtr = (void *)TextMenuInit();
	AddIcon(iconPtr);
}
	

