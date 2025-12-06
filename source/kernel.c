#include "../drivers/framebuffer.h"

int sum_of_three(int a, int b, int c) {
    return a + b + c;
}

int multiply(int a, int b) {
    return a * b;
}

int square(int x) {
    return x * x;
}

void kernel_main(unsigned int magic) {
    fb_clear();
    fb_write("Welcome to MyOS!\n");
    fb_write("C functions test:\n");

    int r1 = sum_of_three(1,2,3);
    int r2 = multiply(3,4);
    int r3 = square(9);

    fb_write("sum_of_three(1,2,3) = ");
    fb_write_int(r1);
    fb_write("\n");

    fb_write("multiply(3,4) = ");
    fb_write_int(r2);
    fb_write("\n");

    fb_write("square(9) = ");
    fb_write_int(r3);
    fb_write("\n");

    fb_write("\nFramebuffer test:\n");
    fb_write_at("Hello at (10,5)!", 10, 5);
}
