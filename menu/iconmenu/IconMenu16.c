/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/
/*
 * Redesigned icon menu, allowing icons to be added/removed at runtime.
 * 02/10/04 - David Pye dmp@davidmpye.dyndns.org
 * You should not need to edit this file in normal circumstances - icons are
 * added/removed via boot/IconMenuInit.c
 */

#include "boot.h"
#include "video.h"
#include "memory_layout.h"
#include <shared.h>
#include <filesys.h>
#include "rc4.h"
#include "sha1.h"
#include "BootFATX.h"
#include "xbox.h"
#include "BootFlash.h"
#include "cpu.h"
#include "BootIde.h"
#include "MenuActions.h"
#include "config.h"
#include "IconMenu.h"

#define TRANSPARENTNESS 0x30
#define SELECTED 0xff

ICON *firstIcon=NULL;
ICON *selectedIcon=NULL;
ICON *firstVisibleIcon=NULL;
int timedOut=0;

void AddIcon(ICON *newIcon) {
	ICON *iconPtr = firstIcon;
	ICON *currentIcon = NULL;
	while (iconPtr != NULL) {
		currentIcon = iconPtr;
		iconPtr = iconPtr->nextIcon;
	}
	
	if (currentIcon==NULL) { 
		//This is the first icon in the chain
		firstIcon = newIcon;
	}
	//Append to the end of the chain
	else currentIcon->nextIcon = newIcon;
	iconPtr = newIcon;
	iconPtr->nextIcon = NULL;
	iconPtr->previousIcon = currentIcon; 
}

static void IconMenuDraw(int nXOffset, int nYOffset) {
	ICON *iconPtr;
	int iconcount;
	
	if (firstVisibleIcon==NULL) firstVisibleIcon = firstIcon;
	if (selectedIcon==NULL) selectedIcon = firstIcon;
	iconPtr = firstVisibleIcon;
	//There are max four 'bays' for displaying icons in - we only draw the four.
	for (iconcount=0; iconcount<4; iconcount++) {
		u8 opaqueness;
		if (iconPtr==NULL) {
			//No more icons to draw
			return;
		}
		if (iconPtr==selectedIcon) {
			//Selected icon has less transparency
			//and has a caption drawn underneath it
			opaqueness = SELECTED;
			//VIDEO_CURSOR_POSX=nXOffset+112*(iconcount+1)*4;
			VIDEO_CURSOR_POSX=nXOffset+150*(iconcount+1)*4;
			VIDEO_CURSOR_POSY=nYOffset;
			VIDEO_ATTR=0xff000000+ICONSELCOLOR;
			printk("%s\n",iconPtr->szCaption);
		}
		else {
			opaqueness = TRANSPARENTNESS;
			#ifdef NONSELECTEDSUBTITLES
	         		VIDEO_CURSOR_POSX=nXOffset+150*(iconcount+1)*4;
				VIDEO_CURSOR_POSY=nYOffset;
				VIDEO_ATTR=0xff000000+ICONNONSELCOLOR;
				printk("%s\n",iconPtr->szCaption);
			#endif	
		}	
		BootVideoJpegBlitBlend(
			//(u8 *)(FB_START+((vmode.width * (nYOffset-74))+nXOffset+(112*(iconcount+1))) * 4),
			(u8 *)(FB_START+((vmode.width * (nYOffset-74))+nXOffset+(150*(iconcount+1))) * 4),
			vmode.width, // dest bytes per line
			&jpegBackdrop, // source jpeg object
			(u8 *)(jpegBackdrop.pData+(iconPtr->iconSlot * jpegBackdrop.bpp)),
			0xff00ff|(((u32)opaqueness)<<24),
			(u8 *)(jpegBackdrop.pBackdrop + ((jpegBackdrop.width * (nYOffset-74)) + nXOffset+(112*(iconcount+1))) * jpegBackdrop.bpp),
			ICON_WIDTH, ICON_HEIGHT
		);
		iconPtr = iconPtr->nextIcon;
	}
}

void IconMenu(void) {
        unsigned char *videosavepage;
        
        u32 COUNT_start;
        u32 temp=1;
	ICON *iconPtr=NULL;

	extern int nTempCursorMbrX, nTempCursorMbrY; 
	int nTempCursorResumeX, nTempCursorResumeY ;
	int nTempCursorX, nTempCursorY;
	int nModeDependentOffset=(vmode.width-640)/2;  

	
	nTempCursorResumeX=nTempCursorMbrX;
	nTempCursorResumeY=nTempCursorMbrY;

	nTempCursorX=VIDEO_CURSOR_POSX;
	nTempCursorY=vmode.height-80;
	
	// We save the complete framebuffer to memory (we restore at exit)
	videosavepage = malloc(FB_SIZE);
	memcpy(videosavepage,(void*)FB_START,FB_SIZE);

	VIDEO_CURSOR_POSX=((110+nModeDependentOffset)<<2);
	VIDEO_CURSOR_POSY=nTempCursorY-200;
	VIDEO_ATTR=0xff000000+WELCOMECOLOR;
	printk("\2Welcome to X-Changer v2.2");
//	VIDEO_CURSOR_POSX=((200+nModeDependentOffset)<<2);
//	VIDEO_CURSOR_POSY=nTempCursorY-150;
//	VIDEO_ATTR=0xff000000+WELCOMECOLOR;
//	printk("\2for XBOX v1.6");
	
	VIDEO_CURSOR_POSX=((252+nModeDependentOffset)<<2);
	VIDEO_CURSOR_POSY=nTempCursorY-100;

//#ifndef SILENT_MODE
	//In silent mode, don't draw the menu the first time.
	//If we get a left/right xpad event, it will be registered,
	//and the menu will 'appear'. Otherwise, it will proceed quietly
	//and boot the default boot item
	VIDEO_ATTR=0xff000000+TEXTCOLOR;
	printk("Select from Menu\n");
	VIDEO_ATTR=0xff000000+TEXTCOLOR;
	IconMenuDraw(nModeDependentOffset, nTempCursorY);
//#endif
	COUNT_start = IoInputDword(0x8008);
	//Main menu event loop.
	while(1)
	{
		int changed=0;
		wait_ms(75);	
		if (risefall_xpad_BUTTON(TRIGGER_XPAD_PAD_RIGHT) == 1)
		{
			if (selectedIcon->nextIcon!=NULL) {
				//A bit ugly, but need to find the last visible icon, and see if 
				//we are moving further right from it.
				ICON *lastVisibleIcon=firstVisibleIcon;
				int i=0;
				for (i=0; i<3; i++) {
					if (lastVisibleIcon->nextIcon==NULL) break;
					lastVisibleIcon = lastVisibleIcon->nextIcon;
				}
				if (selectedIcon == lastVisibleIcon) { 
					//We are moving further right, so slide all the icons along. 
					firstVisibleIcon = firstVisibleIcon->nextIcon;	
					//As all the icons have moved, we need to refresh the entire page.
					memcpy((void*)FB_START,videosavepage,FB_SIZE);
				}
				selectedIcon = selectedIcon->nextIcon;
				changed=1;
			}
			temp=0;
		}
		else if (risefall_xpad_BUTTON(TRIGGER_XPAD_PAD_LEFT) == 1)
		{
			if (selectedIcon->previousIcon!=NULL) {
				if (selectedIcon == firstVisibleIcon) {
					//We are moving further left, so slide all the icons along. 
					firstVisibleIcon = selectedIcon->previousIcon;
					//As all the icons have moved, we need to refresh the entire page.
					memcpy((void*)FB_START,videosavepage,FB_SIZE);
				}
				selectedIcon = selectedIcon->previousIcon;
				changed=1;
			}
			temp=0;
		}
		//If anybody has toggled the xpad left/right, disable the timeout.
		//testweise auskommentiert
		//if (temp!=0) {
		//	temp = IoInputDword(0x8008) - COUNT_start;
		//}
		
		if ((risefall_xpad_BUTTON(TRIGGER_XPAD_KEY_A) == 1) || risefall_xpad_BUTTON(TRIGGER_XPAD_KEY_START) == 1 || 
				(u32)(temp>(0x369E99*BOOT_TIMEWAIT))) {
			memcpy((void*)FB_START,videosavepage,FB_SIZE);
			VIDEO_CURSOR_POSX=nTempCursorResumeX;
			VIDEO_CURSOR_POSY=nTempCursorResumeY;
			
			if (temp>(0x369E99*BOOT_TIMEWAIT)) timedOut=1;
			//Icon selected - invoke function pointer.
			if (selectedIcon->functionPtr!=NULL) selectedIcon->functionPtr(selectedIcon->functionDataPtr);
			//If we come back to this menu, make sure we are redrawn, and that we replace the saved video page
			changed=1;
			memcpy((void*)FB_START,videosavepage,FB_SIZE);
		}
		if (changed) {
			BootVideoClearScreen(&jpegBackdrop, nTempCursorY, VIDEO_CURSOR_POSY+1);
			IconMenuDraw(nModeDependentOffset, nTempCursorY);
			changed=0;
		}
		
	}
}

