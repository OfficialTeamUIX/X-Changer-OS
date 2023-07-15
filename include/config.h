////////////////////// compile-time options ////////////////////////////////

//Cromwell version number
#define VERSION "2.41-dev"

// selects between the supported video modes, see boot.h for enum listing those available
//#define VIDEO_PREFERRED_MODE VIDEO_MODE_800x600
#define VIDEO_PREFERRED_MODE VIDEO_MODE_640x480

//Uncomment to include BIOS flashing support from CD
//Uncommenting for this release - it's broken for now.
#define FLASH

//Uncomment to enable the 'advanced menu'
#define ADVANCED_MENU

//Time to wait in seconds before auto-selecting default item
#define BOOT_TIMEWAIT 15

//Uncomment to make connected Xpads rumble briefly at init.
//#define XPAD_VIBRA_STARTUP

//Uncomment for ultra-quiet mode. Menu is still present, but not
//shown unless you move the xpad. Just backdrop->boot, otherwise
#define SILENT_MODE

//Obsolete


// display a line like Composite 480 detected if uncommented
#undef REPORT_VIDEO_MODE

// show the MBR table if the MBR is valid
#undef DISPLAY_MBR_INFO

#undef DEBUG_MODE
// enable logging to serial port.  You probably don't have this.
#define INCLUDE_SERIAL 0
// enable trace message printing for debugging - with serial only
#define PRINT_TRACE 0

/***********************************************
 *   X-Changer Boot Menu Settings start here   * 
 ***********************************************/

// Remove or comment to have the animation during quickboot
#define NOANI

// Remove or comment to have the animation during boot from menu
//#define NOANI_MENU

// Textmenu Color: Header 
#define HEADCOLOR 0xff0000

// Textmenu Color: Non-selected Items
#define NONSELCOLOR 0x808080

// Textmenu Color: Selected Item
#define SELCOLOR 0xffffff

// Textcolor
#define TEXTCOLOR 0xffffff

// Welcomecolor
#define WELCOMECOLOR 0xffffff

// Remove or comment to disable subtitles for non-selected items in icon menu
//#define NONSELECTEDSUBTITLES

// Color of icon menu subtitle: selected item
#define ICONSELCOLOR 0xffffff

// Color of icon menu subtitle: non-selected item
#define ICONNONSELCOLOR 0xff3333

// Erase progress bar
#define ERASEPBAR 0x808080

// Flash progress bar
#define FLASHPBAR 0xff0000

/********************************************************************************/

