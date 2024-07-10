#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <math.h>
#include "waves.h"

static float m2f(int note)
{
	return (440 / 32) * pow(2, ((note - 9) / 12.0));
}

void channel_on(Channel *chan, uint8_t key, uint8_t vel)
{
	if (chan == NULL)
		return;
	chan->playing = true;
	chan->phase_period = 1000000 / m2f(key) / chan->phase_count;
	chan->velocity = vel;
}

void channel_off(Channel *chan)
{
	if (chan == NULL)
		return;
	chan->playing = false;
}

void channel_set_velocity(Channel *chan, uint8_t vel)
{
	if (chan == NULL)
		return;
	chan->velocity = vel;
}

bool channel_next(Channel *chan, uint32_t delta_t)
{
	if (chan == NULL)
		return false;
	if (!chan->playing)
		return false;
	chan->accum_t += delta_t;
	if (chan->accum_t >= chan->phase_period) {
		chan->accum_t -= chan->phase_period;
		return true;
	}
	return false;
}

const int PULSE25_PHASE_COUNT = 4;
uint8_t pulse25(Channel *chan)
{
	if (++chan->phase < 4)
		return 0;
	chan->phase = 0;
	return chan->velocity >> 3;
}

Channel new_pulse25()
{
	return (Channel) {
	.phase_count = PULSE25_PHASE_COUNT,.out_fn = &pulse25,};
}

const int PULSE50_PHASE_COUNT = 2;
uint8_t pulse50(Channel *chan)
{
	if (++chan->phase < 2)
		return 0;
	chan->phase = 0;
	return chan->velocity >> 3;
}

Channel new_pulse50()
{
	return (Channel) {
	.phase_count = PULSE50_PHASE_COUNT,.out_fn = &pulse50};
}

const int SAWTOOTH_PHASE_COUNT = 16;
uint8_t sawtooth(Channel *chan)
{
	if (chan->phase++ >= SAWTOOTH_PHASE_COUNT)
		chan->phase = 0;
	return chan->phase;
}

Channel new_sawtooth()
{
	return (Channel) {
	.phase_count = SAWTOOTH_PHASE_COUNT,.out_fn = &sawtooth};
}

static const uint8_t triangle_amp[] = {
	0b0000, 0b0001, 0b0010, 0b0011, 0b0100, 0b0101, 0b0110, 0b0111,
	0b1000, 0b1001, 0b1010, 0b1011, 0b1100, 0b1101, 0b1110,
	0b1111, 0b1110, 0b1101, 0b1100, 0b1011, 0b1010, 0b1001, 0b1000,
	0b0111, 0b0110, 0b0101, 0b0100, 0b0011, 0b0010, 0b0001,
};

const int TRIANGLE_PHASE_COUNT = sizeof(triangle_amp);
uint8_t triangle(Channel *chan)
{
	if (++chan->phase >= TRIANGLE_PHASE_COUNT)
		chan->phase = 0;
	return triangle_amp[chan->phase];
}

Channel new_triangle()
{
	return (Channel) {
	.phase_count = TRIANGLE_PHASE_COUNT,.out_fn = &triangle};
}

uint8_t lfsr()
{
	static uint16_t state = 0xf00d & 0x7fff;
	state = (state >> 1) | ((state ^ (state << 14)) & 0x4000);
	return state;
}
