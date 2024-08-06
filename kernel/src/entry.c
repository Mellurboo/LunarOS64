// standard limine initialisation

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

#include "src/include/stdio.h"
#include "src/include/string.h"
#include "include/gdt.h"
#include "include/sysinfo.h"
#include "include/limine.h"
#include "include/panic.h"

// Limine version
__attribute__((used, section(".requests")))
static volatile LIMINE_BASE_REVISION(2);

__attribute__((used, section(".requests_start_marker")))
static volatile LIMINE_REQUESTS_START_MARKER;

__attribute__((used, section(".requests_end_marker")))
static volatile LIMINE_REQUESTS_END_MARKER;

void kstart(void) {
  video_init();
  kprintf("%sLunar OS - x64 UEFI%s\n", BYEL, reset);

  // print some system info things
  printInstalledMemory();
  printCPUCount();

  // setup some descriptor tables
  init_gdt();
  

  panic("The lunar kernel has finished execution, the CPU is halted and interrupts are dissabled");
}
