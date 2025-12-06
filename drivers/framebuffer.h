#ifndef FB_H
#define FB_H

void fb_write_cell(unsigned int index, char c, unsigned char fg, unsigned char bg);
void fb_clear();
void fb_write(const char* s);
void fb_write_char(char c);
void fb_write_int(int num);
void fb_write_at(const char* s, int x, int y);

static inline void outb(unsigned short port, unsigned char value) {
    asm volatile ("outb %0, %1" : : "a"(value), "Nd"(port));
}

#endif
