#pragma once
#include "Effect.h"
#include <list>
class EffectReverbLP :
	public Effect
{
public:
	EffectReverbLP();
	bool next(const void * inputBuffer, void * outputBuffer, unsigned long framesPerBuffer)override;
	virtual ~EffectReverbLP();
private:
	std::vector<float> buff;
	std::list<float> delayLine;
	unsigned dpw, counter, dpr;
};

