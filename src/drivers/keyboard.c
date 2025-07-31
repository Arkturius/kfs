#include <kernel.h>
#include <sys/types.h>
#include <sys/cdefs.h>
#include <sys/io.h>

#define KEY_ESC					0x80
#define KEY_ENTER				0x81
#define KEY_CTRL_L				0x82
#define KEY_CTRL_R				0x83
#define KEY_SHIFT_L				0x84
#define KEY_SHIFT_R				0x85
#define KEY_ALT_L				0x86
#define KEY_ALT_R				0x87
#define KEY_CAPSLOCK			0x88

#define KEY_F1					0x89
#define KEY_F2					0x8a
#define KEY_F3					0x8b
#define KEY_F4					0x8c
#define KEY_F5					0x8d
#define KEY_F6					0x8e
#define KEY_F7					0x8f
#define KEY_F8					0x90
#define KEY_F9					0x91
#define KEY_F10					0x92
#define KEY_F11					0x93
#define KEY_F12					0x94

#define KEY_NUMLOCK				0x95
#define KEY_SCROLLOCK			0x96
#define KEY_MULTIMEDIA_PREV		0x97
#define KEY_MULTIMEDIA_NEXT		0x98

#define KEY_MULTIMEDIA_MUTE		0x99
#define KEY_MULTIMEDIA_CALC		0x9a
#define KEY_MULTIMEDIA_PLAY		0x9b
#define KEY_MULTIMEDIA_STOP		0x9c
#define KEY_MULTIMEDIA_VOL_DOWN	0x9d
#define KEY_MULTIMEDIA_VOL_UP	0x9e

#define KEY_MULTIMEDIA_WWW_HOME	0x9f
#define KEY_MULTIMEDIA_WWW_SRCH	0xa0
#define KEY_MULTIMEDIA_WWW_FAV	0xa1
#define KEY_MULTIMEDIA_WWW_REF	0xa2
#define KEY_MULTIMEDIA_WWW_STOP	0xa3
#define KEY_MULTIMEDIA_WWW_FWD	0xa4
#define KEY_MULTIMEDIA_WWW_BACK	0xa5

#define KEY_MULTIMEDIA_MY_PC	0xa6
#define KEY_MULTIMEDIA_MAIL		0xa7
#define KEY_MULTIMEDIA_SEL		0xa8

#define KEY_HOME				0xa9
#define KEY_END					0xaa
#define KEY_INSERT				0xab
#define KEY_DELETE				0xac
#define KEY_PAGE_UP				0xad
#define KEY_PAGE_DOWN			0xae

#define KEY_UP					0xaf
#define KEY_DOWN				0xb0
#define KEY_LEFT				0xb1
#define KEY_RIGHT				0xb2

#define KEY_GUI_R				0xb3
#define KEY_GUI_L				0xb4

#define KEY_APPS				0xb5

#define KEY_ACPI_POWER			0xb6
#define KEY_ACPI_SLEEP			0xb7
#define KEY_ACPI_WAKE			0xb8


static const char scan_codes[128] = {
	 0 ,0x1b,'1','2','3','4','5','6','7' ,'8','9','0' ,'-' ,'=',0x7f,'\t',
	'q','w' ,'e','r','t','y','u','i','o' ,'p','[',']' ,KEY_ENTER, KEY_CTRL_L ,'a' ,'s' ,
	'd','f' ,'g','h','j','k','l',';','\'','`', KEY_SHIFT_L ,'\\','z' ,'x','c' ,'v' ,
	'b','n' ,'m',',','.','/', KEY_SHIFT_R ,'*', KEY_ALT_L  ,' ', KEY_CAPSLOCK , KEY_F1, KEY_F2, KEY_F3, KEY_F4, KEY_F5,
	KEY_F6, KEY_F7, KEY_F8, KEY_F9, KEY_F10, '7','8','9','-','4','5','6','+','1',
	'2','3','0','.', 0, 0, 0, KEY_F11, KEY_F12, 0
};

static const char extended_scan_codes[128] = {
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	KEY_MULTIMEDIA_PREV, 0, 0, 0, 0 ,0 ,0, 0, KEY_MULTIMEDIA_NEXT, 0, 0, KEY_ENTER, KEY_CTRL_R, 0, 0,
	KEY_MULTIMEDIA_MUTE, KEY_MULTIMEDIA_CALC, KEY_MULTIMEDIA_PLAY,0, KEY_MULTIMEDIA_STOP, 0, 0, 0, 0, KEY_MULTIMEDIA_VOL_DOWN, 0,
	KEY_MULTIMEDIA_VOL_UP, 0, KEY_MULTIMEDIA_WWW_HOME,0 ,0, '/', 0, 0, KEY_ALT_R, 0, 0 ,0,0, 0 ,0, 0,
	0, 0,0 ,0 ,0 ,0, 0,KEY_HOME,KEY_UP, KEY_PAGE_UP, 0, KEY_LEFT,0, KEY_RIGHT, 0, KEY_END,
	KEY_DOWN, KEY_PAGE_DOWN, KEY_INSERT, KEY_DELETE, 0, 0 ,0, KEY_GUI_L, KEY_GUI_R, KEY_APPS, KEY_ACPI_POWER, KEY_ACPI_SLEEP,

	0, 0, 0,KEY_ACPI_WAKE, 0, KEY_MULTIMEDIA_WWW_SRCH, KEY_MULTIMEDIA_WWW_FAV, KEY_MULTIMEDIA_WWW_REF, KEY_MULTIMEDIA_WWW_STOP,
	KEY_MULTIMEDIA_WWW_FWD, KEY_MULTIMEDIA_WWW_BACK, KEY_MULTIMEDIA_MY_PC, KEY_MULTIMEDIA_MAIL, KEY_MULTIMEDIA_SEL, 0
};

#define MOD_CTRL_R	0x1
#define MOD_CTRL_L	0x2
#define MOD_ALT_R	0x4
#define MOD_ALT_L	0x8
#define MOD_GUI_R	0x10
#define MOD_GUI_L	0x20
#define MOD_SHIFT_R	0x40
#define MOD_SHIFT_L	0x80

u8 kbd_mod = 0;

u8 kb_mod_update(u8 scancode, u8 released)
{
	u8 mod_key = 0xff;

	released = scancode & 0x80;
	scancode &= 0x7f;
	switch (scancode)
	{
		case KEY_CTRL_R:
			mod_key = MOD_CTRL_R;
			break;
		case KEY_CTRL_L:
			mod_key = MOD_CTRL_L;
			break;
		case KEY_ALT_R:
			mod_key = MOD_ALT_R;
			break;
		case KEY_ALT_L:
			mod_key = MOD_ALT_L;
			break;
		case KEY_GUI_R:
			mod_key = MOD_GUI_R;
			break;
		case KEY_GUI_L:
			mod_key = MOD_GUI_L;
			break;
		case KEY_SHIFT_R:
			mod_key = MOD_SHIFT_R;
			break;
		case KEY_SHIFT_L:
			mod_key = MOD_SHIFT_L;
			break;
		default:
			return(0);
	}

	if(released)
		kbd_mod &= ~mod_key;
	else
		kbd_mod |= mod_key;
	return(1);
}

u8
kb_key_get()
{
	u8 scancode;
	u8 key;
	u8 released;
	static u8 extended = 0;

	if(!(inb(0x64) & 0b1))
		return(0);

	scancode = inb(0x60);
	if(scancode == 0xE0)
	{
		extended = 1;
		return(0);
	}

	released = scancode & 0x80;
	if(extended)
		key = extended_scan_codes[scancode & 0x7f];
	else
		key = scan_codes[scancode & 0x7f];

	if(kb_mod_update(key, released))
		return(0);
	if(key >= 32 && key < 0x7f && !released)
		return(key);
	return(0);
}
