#include "../drivers/framebuffer.h"

/*
 * These functions are deliberately external (not static) and the coursework
 * Makefile uses -O0.  This keeps all three functions visible in kernel.elf for
 * inspection and viva demonstration instead of allowing the compiler to fold
 * the results into constants.
 */
int sum_of_three(int first, int second, int third)
{
    return first + second + third;
}

int multiply(int first, int second)
{
    return first * second;
}

int square(int value)
{
    return value * value;
}

void kernel_main(uint32_t marker)
{
    fb_set_color(15, 0);
    fb_clear();

    fb_write("MyOS - Advanced Systems Programming\n");
    fb_write("EAX CAFEBABE test: ");
    fb_write_hex(marker);
    if (marker == 0xCAFEBABE) {
        fb_write(" [PASS]\n\n");
    } else {
        fb_write(" [FAIL]\n\n");
    }

    fb_write("C mathematics tests\n");
    fb_write("sum_of_three(1, 2, 3) = ");
    fb_write_int(sum_of_three(1, 2, 3));
    fb_write("\n");

    fb_write("multiply(3, 4) = ");
    fb_write_int(multiply(3, 4));
    fb_write("\n");

    fb_write("square(9) = ");
    fb_write_int(square(9));
    fb_write("\n");

    fb_set_color(10, 0);
    fb_write_at("Framebuffer write_at test (10, 10)", 10, 10);

    fb_set_color(15, 0);
    fb_move_cursor(0, 12);
    fb_write("Kernel tasks completed successfully.");
}
