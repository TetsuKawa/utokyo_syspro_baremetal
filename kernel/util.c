#include "util.h"

#define FONT_HEIGHT 8
#define FONT_WIDTH sizeof(char)*8
extern unsigned char font[128][FONT_HEIGHT];
static struct FrameBuffer *FB;
static unsigned int fb_y;
static unsigned int fb_x;
static unsigned int padding_x;
static unsigned int padding_y;

void init_frame_buffer(struct FrameBuffer *fb) {
  FB = fb;
  padding_y = 8;
  padding_x = 8;
  fb_y = padding_y;
  fb_x = padding_x;

  for (unsigned int i = 0; i < FB->height; i++) {
    for (unsigned int j = 0; j < FB->width; j++) {
      struct Pixel *pixel = FB->base + FB->width * i + j;
      // white
      pixel->r = 255;
      pixel->g = 255;
      pixel->b = 255;
    }
  }
};

void shift_one_line() {
  for (unsigned int i = padding_y; i < (FB->height - padding_y); i++) {
    for (unsigned int j = padding_x; j < (FB->width - padding_x); j++) {
      struct Pixel *pixel = FB->base + FB->width * i + j;

      if ((i + padding_y + FONT_HEIGHT) > FB->height) {
        // white
        pixel->r = 255;
        pixel->g = 255;
        pixel->b = 255;
      } else {
        struct Pixel *next_line_pixel = FB->base + FB->width * (i + FONT_HEIGHT) + j;
        // copy next line pixel
        pixel->r = next_line_pixel->r;
        pixel->g = next_line_pixel->g;
        pixel->b = next_line_pixel->b;
      }
    }
  }
};

void insert_line_break() {
  // delete current cursor position
  for (unsigned int i = 0; i < FONT_HEIGHT; i++) {
    for (unsigned int j = 0; j < FONT_WIDTH; j++) {
      struct Pixel *pixel = FB->base + FB->width * (fb_y + i) + fb_x + j;
      pixel->r = pixel->g = pixel->b = 255;
    }
  }

  if ((fb_y + padding_y + FONT_HEIGHT * 2) > FB->height) {
    shift_one_line();
    fb_x = padding_x;
  } else {
    fb_y += FONT_HEIGHT;
    fb_x = padding_x;
  }
};

void putc(char c) {
  for (unsigned int i = 0; i < FONT_HEIGHT; i++) {
    for (unsigned int j = 0; j < FONT_WIDTH; j++) {
      unsigned char mask = 1;
      struct Pixel *pixel = FB->base + FB->width * (fb_y + i) + fb_x + j;
      pixel->r = pixel->g = pixel->b = ((font[(unsigned char)c][i] >> (FONT_WIDTH - j)) & mask) ? 0 : 255;
    }
  }
  fb_x += FONT_WIDTH;

  // current cursor position
  for (unsigned int i = 0; i < FONT_HEIGHT; i++) {
    for (unsigned int j = 0; j < FONT_WIDTH; j++) {
      struct Pixel *pixel = FB->base + FB->width * (fb_y + i) + fb_x + j;
      pixel->r = pixel->g = pixel->b = 0;
    }
  } 
};

void puts(char *str) {
  for (unsigned int i = 0; str[i] != '\0'; i++) {
    // \n or \r
    if (str[i] == '\n' || str[i] == '\r') {
      insert_line_break();
      continue;
  }

    if ((fb_x + padding_x + FONT_WIDTH) > FB->width) {
      insert_line_break();
    }

    putc(str[i]);
  }
};

void puth(unsigned long long value, unsigned char digits_len) {
  for (int i = digits_len - 1; i >= 0; i--) {
      unsigned char offset = (value >> (4 * i)) & 0x0f;
      char c = offset < 0xa ? '0' + offset : 'A' + (offset - 0xa);
      putc(c);
  }
}

char compare_s(const char *first, const char *second, unsigned long long bytes) {
  for (unsigned long long i = 0; i < bytes; i++) {
    if (first[i] != second[i]) return 0;
  }
  return 1;
}

