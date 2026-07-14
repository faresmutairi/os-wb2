#ifndef FRAMEBUFFER_H
#define FRAMEBUFFER_H

typedef unsigned char  uint8_t;
typedef unsigned short uint16_t;
typedef unsigned int   uint32_t;

enum {
    FB_WIDTH = 80,
    FB_HEIGHT = 25
};

void fb_clear(void);
void fb_set_color(uint8_t foreground, uint8_t background);
void fb_move_cursor(uint16_t x, uint16_t y);
void fb_write_char(char character);
void fb_write(const char *text);
void fb_write_at(const char *text, uint16_t x, uint16_t y);
void fb_write_int(int value);
void fb_write_hex(uint32_t value);

#endif

