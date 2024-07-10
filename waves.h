#ifndef WAVES_H_
#define WAVES_H_

#include <stdint.h>
#include <stdbool.h>

typedef struct Channel {	//TODO: pitch bending!!
	uint32_t phase_count;
	uint32_t phase_period;
	 uint8_t(*out_fn) (struct Channel * chan);

	bool playing;
	uint32_t accum_t;
	uint8_t velocity;

	int phase;
} Channel;

extern void channel_on(Channel * chan, uint8_t key, uint8_t vel);
extern void channel_off(Channel * chan);
extern void channel_set_velocity(Channel * chan, uint8_t vel);
extern bool channel_next(Channel * chan, uint32_t delta_t);

extern const int PULSE25_PHASE_COUNT;
extern uint8_t pulse25(Channel * chan);
extern Channel new_pulse25();
extern const int PULSE50_PHASE_COUNT;
extern uint8_t pulse50(Channel * chan);
extern Channel new_pulse50();
extern const int SAWTOOTH_PHASE_COUNT;
extern uint8_t sawtooth(Channel * chan);
extern Channel new_sawtooth();
extern const int TRIANGLE_PHASE_COUNT;
extern uint8_t triangle(Channel * chan);
extern Channel new_triangle();

extern uint8_t lfsr();

#endif				// WAVES_H_
