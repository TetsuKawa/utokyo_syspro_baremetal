#include "hardware.h"

unsigned int read_24bit_value(unsigned short port);
unsigned int read_32bit_value(unsigned short port);

void init_acpi_pm_timer(struct RSDP *rsdp);
void pm_timer_wait_millisec(unsigned int msec);
