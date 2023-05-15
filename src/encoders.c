#include "encoders.h"

void setEncoderState(volatile EncoderAxis *axis, int A, int B)
{
	axis->prevA = axis->A;
	axis->A = A;
	axis->prevB = axis->B;
	axis->B = B;

	axis->delta += lookupTable[axis->prevA<<3|axis->prevB<<2|axis->A<<1|axis->B];
}

volatile EncoderAxis x;
volatile EncoderAxis y;
volatile EncoderAxis k;

const int8_t lookupTable[] = {0, -1, 1, 0, 1, 0, 0, -1, -1, 0, 0, 1, 0, 1, -1, 0 };
