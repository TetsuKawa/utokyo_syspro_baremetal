#include "hardware.h"
#include "segmentation.h"
#include "util.h"

void start(void *SystemTable __attribute__ ((unused)), struct HardwareInfo *_hardware_info) {
  // From here - Put this part at the top of start() function
  // Do not use _hardware_info directry since this data is located in UEFI-app space
  hardware_info = *_hardware_info;
  init_segmentation();
  // To here - Put this part at the top of start() function

  // To here - sample code
  init_frame_buffer(&hardware_info.fb);

  // Do not delete it!
  while (1);
}
