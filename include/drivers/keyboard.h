#ifndef _DRIVER_KEYBOARD_H
# define _DRIVER_KEYBOARD_H

#include <keycodes.h>
#include <sys/types.h>

static const char
scan_codes[128] =
{
	 0 ,0x1b,'1','2','3','4','5','6','7' ,'8','9','0' ,'-' ,'=', '\b','\t',
	'q','w' ,'e','r','t','y','u','i','o' ,'p','[',']' , '\n', KEY_CTRL_L ,'a' ,'s' ,
	'd','f' ,'g','h','j','k','l',';','\'','`', KEY_SHIFT_L ,'\\','z' ,'x','c' ,'v' ,
	'b','n' ,'m',',','.','/', KEY_SHIFT_R ,'*', KEY_ALT_L  ,' ', KEY_CAPSLOCK , KEY_F1, KEY_F2, KEY_F3, KEY_F4, KEY_F5,
	KEY_F6, KEY_F7, KEY_F8, KEY_F9, KEY_F10, KEY_NUMLOCK, KEY_SCROLLOCK, '7','8','9','-','4','5','6','+','1',
	'2','3','0','.', 0, 0, 0, KEY_F11, KEY_F12, 0
};

static const char
extended_scan_codes[128] =
{
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, // 0xf
	KEY_MULTIMEDIA_PREV, 0, 0, 0, 0 ,0 ,0, 0,0, KEY_MULTIMEDIA_NEXT, 0, 0, '\n', KEY_CTRL_R, 0, 0, // 0x1f
	KEY_MULTIMEDIA_MUTE, KEY_MULTIMEDIA_CALC, KEY_MULTIMEDIA_PLAY,0, KEY_MULTIMEDIA_STOP,0, 0,0 , 0, 0, 0, 0, 0, 0, KEY_MULTIMEDIA_VOL_DOWN, 0,
	KEY_MULTIMEDIA_VOL_UP, 0, KEY_MULTIMEDIA_WWW_HOME,0 ,0, '/', 0, 0, KEY_ALT_R, 0, 0 ,0,0, 0 ,0, 0,
	0, 0, 0, 0 ,0 ,0, 0,KEY_HOME,KEY_UP, KEY_PAGE_UP, 0, KEY_LEFT,0, KEY_RIGHT, 0, KEY_END,
	KEY_DOWN, KEY_PAGE_DOWN, KEY_INSERT, KEY_DELETE, 0, 0, 0, 0, 0, 0, 0, KEY_GUI_L, KEY_GUI_R, KEY_APPS, KEY_ACPI_POWER, KEY_ACPI_SLEEP,

	0, 0, 0,KEY_ACPI_WAKE, 0, KEY_MULTIMEDIA_WWW_SRCH, KEY_MULTIMEDIA_WWW_FAV, KEY_MULTIMEDIA_WWW_REF, KEY_MULTIMEDIA_WWW_STOP,
	KEY_MULTIMEDIA_WWW_FWD, KEY_MULTIMEDIA_WWW_BACK, KEY_MULTIMEDIA_MY_PC, KEY_MULTIMEDIA_MAIL, KEY_MULTIMEDIA_SEL, 0
};

# define MOD_CTRL_R		0x1
# define MOD_CTRL_L		0x2
# define MOD_ALT_R		0x4
# define MOD_ALT_L		0x8
# define MOD_GUI_R		0x10
# define MOD_GUI_L		0x20
# define MOD_SHIFT_R	0x40
# define MOD_SHIFT_L	0x80

extern u8 kbd_mod;

u8
kb_key_get();

i32
kb_read(char *buffer, u32 size);

#endif // _DRIVER_KEYBOARD_H
