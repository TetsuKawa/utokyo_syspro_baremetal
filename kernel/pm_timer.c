#include "pm_timer.h"
#include "util.h"

#define TMR_VAL_EXT_BIT_OFFSET 8

const unsigned int freq_hz = 3579545;
unsigned short pm_timer_blk;
char pm_timer_is_32;

void init_acpi_pm_timer(struct RSDP *rsdp) {
  struct XSDT *xsdt = (struct XSDT *)rsdp->xsdt_address;
  unsigned int other_tables_length = (xsdt->sdth.length - 36) / 8;

  for (unsigned int i = 0; i < other_tables_length; i++) {
    unsigned int offset = 36 + i * 8;
    unsigned long long *other_tables = (unsigned long long *)((char *)xsdt + offset);
    unsigned long long table_address = *other_tables;
    struct SDTH *header = (struct SDTH *)table_address;
    char *signature_ptr = header->signature;

    if (compare_s(signature_ptr, "FACP", 4)) {
      struct FADT *fadt = (struct FADT*)(*(unsigned long long *)((char *)xsdt + offset));
      pm_timer_blk = (unsigned short)fadt->PM_TMR_BLK;
      pm_timer_is_32 = (fadt->flags >> TMR_VAL_EXT_BIT_OFFSET) & 1;
      break;
    }
  }
};

void pm_timer_wait_millisec(unsigned int msec) {
  unsigned int target_ticks = freq_hz * msec / 1000;
  unsigned int counted_ticks = 0;

  if (pm_timer_is_32) {
    const unsigned int max_count = 0xFFFFFFFF;
    unsigned int start = read_32bit_value(pm_timer_blk);

    while (counted_ticks < target_ticks) {
      unsigned int current = read_32bit_value(pm_timer_blk);
      if (current >= start) {
        counted_ticks = current - start;
      } else {
        counted_ticks = (max_count - start) + current + 1;
      }
    }
  } else {
    const unsigned long long max_count = 0x00FFFFFF;
    unsigned long long start = read_24bit_value(pm_timer_blk);

    while (counted_ticks < target_ticks) {
      unsigned long long current = read_24bit_value(pm_timer_blk);
      if (current >= start) {
        counted_ticks = current - start;
      } else {
        counted_ticks = (max_count - start) + current + 1;
      }
    }
  }
};