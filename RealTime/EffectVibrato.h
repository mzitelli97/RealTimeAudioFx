#pragma once
#include "Effect.h"
#include <list>
class EffectVibrato :
	public Effect
{
public:
	EffectVibrato();
	bool next(const void * inputBuffer, void * outputBuffer, unsigned long framesPerBuffer)override;
	virtual ~EffectVibrato();
private:
	std::vector<float> buff;
	std::list<float> delayLine;
	unsigned dpw,counter;
	float dpr;
};

