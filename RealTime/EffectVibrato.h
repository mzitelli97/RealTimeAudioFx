#pragma once
#include "Effect.h"
class EffectVibrato :
	public Effect
{
public:
	EffectVibrato();
	bool next(const void * inputBuffer, void * outputBuffer, unsigned long framesPerBuffer)override;
	bool setProp(unsigned i, double v)override;
	virtual ~EffectVibrato();
private:
	std::vector<float> buff;
	unsigned dpw,counter;
	float dpr;
};

