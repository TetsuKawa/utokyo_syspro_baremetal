#include "lapic_timer.h"
#include "pm_timer.h"
#include "util.h"

#define LVT_TIMER_MODE_MASK 0x00060000

volatile unsigned int * const lvt_timer = (unsigned int *)0xfee00320;
volatile unsigned int * const initial_count = (unsigned int *)0xfee00380;
volatile unsigned int * const current_count = (unsigned int *)0xfee00390;
volatile unsigned int * const divide_config = (unsigned int *)0xfee003e0;

unsigned int lapic_timer_freq_khz;

unsigned int measure_lapic_freq_khz() {
  *lvt_timer = (*lvt_timer & ~LVT_TIMER_MODE_MASK) | 0x00050000;
  *initial_count = 0xffffffff;
  pm_timer_wait_millisec(1000);
  unsigned int current = *current_count;

  if (current == 0) {
    puts("measure_lapic_freq_khz Failed");
    return 0;
  }

  lapic_timer_freq_khz = (*initial_count - current) / 1000;
  return lapic_timer_freq_khz;
}