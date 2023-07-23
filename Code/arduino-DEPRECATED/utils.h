#ifndef UTILS_H
#define UTILS_H

#include "hardware/gpio.h"
#include "hardware/sync.h"
#include "hardware/structs/ioqspi.h"
#include "hardware/structs/sio.h"

bool __no_inline_not_in_flash_func(get_bootsel_button)();

#endif
