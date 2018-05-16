#pragma once
#include "Effect.h"
class EffectChorus :
	public Effect
{
public:
	EffectChorus();
	bool next(const void * inputBuffer, void * outputBuffer, unsigned long framesPerBuffer)override;
	bool setProp(unsigned i, double v)override;
	virtual ~EffectChorus();
private:
	std::vector<float> buff;
	unsigned dpw, counter;
	float dpr;
};