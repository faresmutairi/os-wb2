#include "framebuffer.h"

#define FB_ADDRESS 0x000B8000
#define FB_COLS 80
#define FB_ROWS 25

static unsigned short cursor_x = 0;
static unsigned short cursor_y = 0;

static unsigned char fb_color = 0x0F; // white-on-black

static void fb_move_cursor() {
    unsigned short pos = cursor_y * FB_COLS + cursor_x;
    outb(0x3D4, 14);
    outb(0x3D5, pos >> 8);
    outb(0x3D4, 15);
    outb(0x3D5, pos & 0xFF);
}

void fb_write_cell(unsigned int index, char c, unsigned char fg, unsigned char bg) {
    char *fb = (char*) FB_ADDRESS;
    fb[index * 2] = c;
    fb[index * 2 + 1] = (bg << 4) | (fg & 0x0F);
}

void fb_clear() {
    for (int i = 0; i < FB_COLS * FB_ROWS; i++) {
        fb_write_cell(i, ' ', 0x0F, 0x00);
    }
    cursor_x = 0;
    cursor_y = 0;
    fb_move_cursor();
}

void fb_write_char(char c) {
    if (c == '\n') {
        cursor_x = 0;
        cursor_y++;
        fb_move_cursor();
        return;
    }

    fb_write_cell(cursor_y * FB_COLS + cursor_x, c, 0x0F, 0x00);
    cursor_x++;

    if (cursor_x >= FB_COLS) {
        cursor_x = 0;
        cursor_y++;
    }
    fb_move_cursor();
}

void fb_write(const char* s) {
    for (int i = 0; s[i] != '\0'; i++) {
        fb_write_char(s[i]);
    }
}

void fb_write_int(int num) {
    char buf[16];
    int i = 0;

    if (num == 0) {
        fb_write_char('0');
        return;
    }

    if (num < 0) {
        fb_write_char('-');
        num = -num;
    }

    while (num > 0) {
        buf[i++] = '0' + (num % 10);
        num /= 10;
    }

    while (i--) {
        fb_write_char(buf[i]);
    }
}

void fb_write_at(const char *s, int x, int y) {
    cursor_x = x;
    cursor_y = y;
    fb_move_cursor();
    fb_write(s);
}
