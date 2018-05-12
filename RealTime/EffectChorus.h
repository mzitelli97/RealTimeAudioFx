#pragma once
#include "Effect.h"
#include <list>
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
	std::list<float> delayLine;
	unsigned dpw, counter;
	float dpr;
};