#ifndef ENCODERS_H
#define ENCODERS_H

#include "lpc_types.h"

// lookup table for the quadrature encoders
extern const int8_t lookupTable[];


/* Struct to track state of an encoder axis
 * resolution is the resolution of the encoder, can be configured over USB
 * prevA is the previously read value of the encoder's A terminal
 * A is the encoder's current A terminal state
 * prevB is the previously read value of the encoder's B terminal
 * B is the encoder's current B terminal state
 * delta is the amount the encoder has changed, gets set to zero each time this value
 * is reported via USB
 */
typedef struct
{
	uint32_t resolution;

	bool prevA;
	bool prevB;
	bool A;
	bool B;

	int32_t delta;
} EncoderAxis;

// holds the state of all encoder axes
extern volatile EncoderAxis x;
extern volatile EncoderAxis y;
extern volatile EncoderAxis k;

extern void setEncoderState(volatile EncoderAxis *axis, int A, int B);

#endif
