#include "hardware.h"

void init_frame_buffer(struct FrameBuffer *fb);
void shift_one_line();
void insert_line_break();
void putc(char c);
void puts(char *str);
void puth(unsigned long long value, unsigned char digits_len);
void putd(unsigned long long value, unsigned char digits_len);
char compare_s(const char *first, const char *second, unsigned long long bytes);
