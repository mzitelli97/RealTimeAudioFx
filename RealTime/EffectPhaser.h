#pragma once
#include "Effect.h"
#include "NotchFilter2Order.h"

class EffectPhaser :
	public Effect
{
public:
	EffectPhaser();
	bool next(const void * inputBuffer, void * outputBuffer, unsigned long framesPerBuffer)override;
	virtual ~EffectPhaser();
private:
	unsigned int filterCount, sampleCount;
	NotchFilter2Order notch;
};

