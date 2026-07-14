#include "framebuffer.h"

#define FRAMEBUFFER_ADDRESS ((volatile uint16_t *)0x000B8000)
#define VGA_COMMAND_PORT 0x3D4
#define VGA_DATA_PORT    0x3D5

static uint16_t cursor_x;
static uint16_t cursor_y;
static uint8_t color = 0x0F;

static inline void outb(uint16_t port, uint8_t value)
{
    __asm__ volatile ("outb %0, %1" : : "a"(value), "Nd"(port));
}

static uint16_t make_cell(char character)
{
    return (uint16_t)(uint8_t)character | ((uint16_t)color << 8);
}

static void update_hardware_cursor(void)
{
    uint16_t position = (uint16_t)(cursor_y * FB_WIDTH + cursor_x);

    outb(VGA_COMMAND_PORT, 14);
    outb(VGA_DATA_PORT, (uint8_t)(position >> 8));
    outb(VGA_COMMAND_PORT, 15);
    outb(VGA_DATA_PORT, (uint8_t)(position & 0xFF));
}

static void scroll_if_needed(void)
{
    uint16_t x;
    uint16_t y;

    if (cursor_y < FB_HEIGHT) {
        return;
    }

    for (y = 1; y < FB_HEIGHT; ++y) {
        for (x = 0; x < FB_WIDTH; ++x) {
            FRAMEBUFFER_ADDRESS[(y - 1) * FB_WIDTH + x] =
                FRAMEBUFFER_ADDRESS[y * FB_WIDTH + x];
        }
    }

    for (x = 0; x < FB_WIDTH; ++x) {
        FRAMEBUFFER_ADDRESS[(FB_HEIGHT - 1) * FB_WIDTH + x] = make_cell(' ');
    }

    cursor_y = FB_HEIGHT - 1;
}

void fb_set_color(uint8_t foreground, uint8_t background)
{
    color = (uint8_t)((background << 4) | (foreground & 0x0F));
}

void fb_move_cursor(uint16_t x, uint16_t y)
{
    if (x >= FB_WIDTH) {
        x = FB_WIDTH - 1;
    }
    if (y >= FB_HEIGHT) {
        y = FB_HEIGHT - 1;
    }

    cursor_x = x;
    cursor_y = y;
    update_hardware_cursor();
}

void fb_clear(void)
{
    uint32_t index;

    for (index = 0; index < FB_WIDTH * FB_HEIGHT; ++index) {
        FRAMEBUFFER_ADDRESS[index] = make_cell(' ');
    }

    cursor_x = 0;
    cursor_y = 0;
    update_hardware_cursor();
}

void fb_write_char(char character)
{
    if (character == '\n') {
        cursor_x = 0;
        ++cursor_y;
    } else if (character == '\r') {
        cursor_x = 0;
    } else if (character == '\t') {
        cursor_x = (uint16_t)((cursor_x + 4) & ~3U);
    } else if (character == '\b') {
        if (cursor_x > 0) {
            --cursor_x;
            FRAMEBUFFER_ADDRESS[cursor_y * FB_WIDTH + cursor_x] = make_cell(' ');
        }
    } else {
        FRAMEBUFFER_ADDRESS[cursor_y * FB_WIDTH + cursor_x] = make_cell(character);
        ++cursor_x;
    }

    if (cursor_x >= FB_WIDTH) {
        cursor_x = 0;
        ++cursor_y;
    }

    scroll_if_needed();
    update_hardware_cursor();
}

void fb_write(const char *text)
{
    if (text == 0) {
        return;
    }

    while (*text != '\0') {
        fb_write_char(*text++);
    }
}

void fb_write_at(const char *text, uint16_t x, uint16_t y)
{
    fb_move_cursor(x, y);
    fb_write(text);
}

void fb_write_int(int value)
{
    char digits[12];
    unsigned int magnitude;
    unsigned int count = 0;

    if (value == 0) {
        fb_write_char('0');
        return;
    }

    if (value < 0) {
        fb_write_char('-');
        magnitude = 0U - (unsigned int)value;
    } else {
        magnitude = (unsigned int)value;
    }

    while (magnitude > 0U) {
        digits[count++] = (char)('0' + (magnitude % 10U));
        magnitude /= 10U;
    }

    while (count > 0U) {
        fb_write_char(digits[--count]);
    }
}

void fb_write_hex(uint32_t value)
{
    static const char hexadecimal[] = "0123456789ABCDEF";
    int shift;

    fb_write("0x");
    for (shift = 28; shift >= 0; shift -= 4) {
        fb_write_char(hexadecimal[(value >> shift) & 0x0F]);
    }
}

