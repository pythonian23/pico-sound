#include "pico/stdlib.h"
#include "gpio.h"

void init_gpio(uint lsb_pin)
{
	for (int i = 0; i < 4; i++) {
		gpio_init(lsb_pin + i);
		gpio_set_dir(lsb_pin + i, GPIO_OUT);
	}
}

void set_out(uint8_t val, uint pin)
{
	gpio_put_masked(0b1111 << pin, ((uint32_t) val) << pin);
}
