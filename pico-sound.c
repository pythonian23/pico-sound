#include <stdio.h>
#include <stdbool.h>
#include <math.h>
#include "pico/stdlib.h"
#include "pico/multicore.h"
#include "gpio.h"
#include "waves.h"

#define PIN0 2
#define PIN1 6
#define PIN2 10
#define PINN 15

Channel chan0;
Channel chan1;
Channel chan2;
Channel chann = {
	.phase_count = 8,
	.playing = false,
};

void init_channels()
{
	chan0 = new_pulse50();
	chan1 = new_pulse50();
	chan2 = new_triangle();

	gpio_init(PINN);
	gpio_set_dir(PINN, GPIO_OUT);
}

void init()
{
	init_channels();

	init_gpio(PIN0);
	init_gpio(PIN1);
	init_gpio(PIN2);

	stdio_init_all();
}

Channel *chan_select(uint8_t chan)
{
	Channel *out = NULL;
	switch (chan) {
	case 0:
		out = &chan0;
		break;
	case 1:
		out = &chan1;
		break;
	case 2:
		out = &chan2;
		break;
	case 9:
		out = &chann;
		break;
	}
	return out;
}

void main1()
{
	uint8_t byte;
	uint8_t chan;
	while (true) {
		byte = getchar();
		chan = byte & 0x0f;
		switch (byte & 0xf0) {
		case 0x80:	// Note Off
			getchar();	// Key
			getchar();	// Velocity
			channel_off(chan_select(chan));
			break;
		case 0x90:	// Note On
			uint8_t key = getchar();
			uint8_t vel = getchar();
			channel_on(chan_select(chan), key, vel);
			break;
		case 0xa0:	// Polyphonic Key Pressure
			getchar();	// Key
			getchar();	// Pressure
			break;
		case 0xb0:	// Control Change / Channel Mode Message
			getchar();	// Controller number
			getchar();	// Controller value
			break;
		case 0xc0:	// Program Change
			getchar();	// Program number
			break;
		case 0xd0:	// Pitch Bend Change
			getchar();	// LSB
			getchar();	// MSB
			break;
		case 0xe0:	// Pitch Bend Change
			getchar();	// LSB
			getchar();	// MSB
			break;
		case 0xf0:
			uint8_t control = getchar();
			uint8_t value = getchar();
			if (control == 0x07) {
				channel_set_velocity(chan_select(chan), value);
			} else if (control == 0x0b) {
				channel_set_velocity(chan_select(chan),
						     127 * value / 100);
			} else if (control == 0x78 && value == 0x00) {
				channel_off(chan_select(chan));
			} else if (control == 0x7b) {
				channel_set_velocity(chan_select(chan), 0);
			}
			break;
		}
	}
}

int main()
{
	init();
	multicore_launch_core1(main1);

	uint32_t new_t, delta_t;
	uint32_t prev_t = time_us_32();

	while (true) {
		new_t = time_us_32();
		delta_t = new_t - prev_t;
		prev_t = new_t;

		if (channel_next(&chan0, delta_t)) {
			set_out(chan0.out_fn(&chan0), PIN0);
		}
		if (channel_next(&chan1, delta_t)) {
			set_out(chan1.out_fn(&chan1), PIN1);
		}
		if (channel_next(&chan2, delta_t)) {
			set_out(chan2.out_fn(&chan2), PIN2);
		}
		if (channel_next(&chann, delta_t)) {
			gpio_put(PINN, lfsr() & 1);
		}
	}

	return 0;
}
