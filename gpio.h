#ifndef GPIO_H_
#define GPIO_H_

#include <stdint.h>
#include "pico/stdlib.h"

extern void init_gpio(uint lsb_pin);
extern void set_out(uint8_t, uint);

#endif				// GPIO_H_
