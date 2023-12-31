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

TEXTMENU *LEDMenuInit(void) {
	TEXTMENUITEM *itemPtr;
	TEXTMENU *menuPtr;

	menuPtr = (TEXTMENU*)malloc(sizeof(TEXTMENU));
	memset(menuPtr,0x00,sizeof(TEXTMENU));
	strcpy(menuPtr->szCaption, "LED Control Menu");

	itemPtr = (TEXTMENUITEM*)malloc(sizeof(TEXTMENUITEM));
	memset(itemPtr,0x00,sizeof(TEXTMENUITEM));
	strcpy(itemPtr->szCaption, "RED");
	itemPtr->functionPtr=(void*)I2cSetFrontpanelLed;
	itemPtr->functionDataPtr = (u8*)(I2C_LED_RED0 | I2C_LED_RED1 | I2C_LED_RED2 | I2C_LED_RED3 );
	TextMenuAddItem(menuPtr, itemPtr);

	itemPtr = (TEXTMENUITEM*)malloc(sizeof(TEXTMENUITEM));
	memset(itemPtr,0x00,sizeof(TEXTMENUITEM));
	strcpy(itemPtr->szCaption, "GREEN");
	itemPtr->functionPtr=(void*)I2cSetFrontpanelLed;
	itemPtr->functionDataPtr = (u8*)(I2C_LED_GREEN0 | I2C_LED_GREEN1 | I2C_LED_GREEN2 | I2C_LED_GREEN3 );
	TextMenuAddItem(menuPtr, itemPtr);

	itemPtr = (TEXTMENUITEM*)malloc(sizeof(TEXTMENUITEM));
	memset(itemPtr,0x00,sizeof(TEXTMENUITEM));
	strcpy(itemPtr->szCaption, "ORANGE");
	itemPtr->functionPtr=(void*)I2cSetFrontpanelLed;
	itemPtr->functionDataPtr = (u8*)(I2C_LED_RED0 | I2C_LED_RED1 | I2C_LED_RED2 | I2C_LED_RED3 | I2C_LED_GREEN0 | I2C_LED_GREEN1 | I2C_LED_GREEN2 | I2C_LED_GREEN3);
	TextMenuAddItem(menuPtr, itemPtr);

	itemPtr = (TEXTMENUITEM*)malloc(sizeof(TEXTMENUITEM));
	memset(itemPtr,0x00,sizeof(TEXTMENUITEM));
	strcpy(itemPtr->szCaption, "X-Changer default");
	itemPtr->functionPtr=(void*)I2cSetFrontpanelLed;
	itemPtr->functionDataPtr = (u8*)(I2C_LED_RED0 | I2C_LED_GREEN1 | I2C_LED_RED2 | I2C_LED_GREEN2);
	TextMenuAddItem(menuPtr, itemPtr);

	/*
	itemPtr = (TEXTMENUITEM*)malloc(sizeof(TEXTMENUITEM));
	memset(itemPtr,0x00,sizeof(TEXTMENUITEM));
	strcpy(itemPtr->szCaption, "X-Changer default");
	itemPtr->functionPtr=(void*)I2cSetFrontpanelLed;
	itemPtr->functionDataPtr = (u8*)(I2C_LED_RED0);
	TextMenuAddItem(menuPtr, itemPtr);
	*/
	
	return menuPtr;
}
