#include "EffectPlain.h"



EffectPlain::EffectPlain():Effect("Plain")
{
}

bool EffectPlain::next(const void * inputBuffer, void * outputBuffer, unsigned long framesPerBuffer)
{
	float* din = (float*)inputBuffer, *dout = (float*)outputBuffer;
	memcpy(outputBuffer, inputBuffer, framesPerBuffer * sizeof(float));
	for (unsigned i = 0; i < framesPerBuffer; i += 1)
	{
		dout[2 * framesPerBuffer - 1 - 2 * i] = dout[framesPerBuffer - 1 - i];
		dout[2 * framesPerBuffer - 2 - 2 * i] = dout[framesPerBuffer - 1 - i];
	}
	return true;
}


EffectPlain::~EffectPlain()
{
}
