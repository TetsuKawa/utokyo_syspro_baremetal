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
  // putc('T');
  int i = 0;

  while(i < 30) {
    putc('T');
    putc('.');
    putc('K');
    puts("\n\n");
    puts("Hello, SysPro!\n");
    puts("\n");
    puth(255, 5);
    puts("\n");
    i++;
  };

  i = 0;
  while(i < 500) {
    puts("T");
    i++;
  }

  // Do not delete it!
  while (1);
}
