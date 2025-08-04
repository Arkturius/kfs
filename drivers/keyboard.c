/**
 * drivers/keyboard.c
 */

#include <keycodes.h>
#include <kernel.h>
#include <sys/types.h>
#include <sys/cdefs.h>
#include <sys/io.h>
#include <drivers/keyboard.h>
#include <printk.h>

u8 kbd_mod = 0;

u8 kb_mod_update(u8 key, u8 released)
{
	u8 mod_key = 0xff;

	switch (key)
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

	if (!(inb(0x64) & 0b1))
		return(0);

	scancode = inb(0x60);
	if (scancode == 0xE0)
	{
		extended = 1;
		return(0);
	}


	released = scancode & 0x80;
	if (extended)
		key = extended_scan_codes[scancode & 0x7f];
	else
		key = scan_codes[scancode & 0x7f];
	extended = 0;

	if (kb_mod_update(key, released))
		return(0);

	if (key >= 'a' && key <= 'z' && kbd_mod & (MOD_SHIFT_R | MOD_SHIFT_L))
		key -= 32;

    if (released)
        return (0);
    if (key >= 32 && key < 0x7f)
		return (key);
    if (key >= 8 && key <= 13)
        return (key);
    if (key == KEY_DOWN || key == KEY_UP || key == KEY_LEFT || key == KEY_RIGHT)
        return (key);
    return(0);
}

i32
kb_read(char *buffer, u32 size)
{
    i32 n = 0;

    while (1)
    {
        u8  c = kb_key_get();

        if (!c)
            continue ;
        if (c == '\n')
            break ;
        if (c == 8 && n != 0)
            buffer[--n] = 0;
        else if (c >= 32 && c < 127)
            buffer[n++] = c;
        else
            continue ;
        vga_putc(c);
        if (!size--)
            break ;
    }
    return (n);
}
