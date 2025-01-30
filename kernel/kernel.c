#include "hardware.h"
#include "segmentation.h"
#include "util.h"
#include "pm_timer.h"
#include "lapic_timer.h"

void start(void *SystemTable __attribute__ ((unused)), struct HardwareInfo *_hardware_info) {
  // From here - Put this part at the top of start() function
  // Do not use _hardware_info directry since this data is located in UEFI-app space
  hardware_info = *_hardware_info;
  init_segmentation();
  // To here - Put this part at the top of start() function

  // To here - sample code
  init_frame_buffer(&hardware_info.fb);
  init_acpi_pm_timer(hardware_info.rsdp);
  puts("lapic kHz: ");
  unsigned int kHz = measure_lapic_freq_khz();
  putd(kHz, 8);
  // Do not delete it!
  while (1);
}
