#pragma once
#include "Effect.h"
#include <vector>
#include "AllPassFilter.h"

class EffectPhaser :
	public Effect
{
public:
	EffectPhaser();
	bool next(const void * inputBuffer, void * outputBuffer, unsigned long framesPerBuffer)override;
	virtual ~EffectPhaser();
private:
	float lastFilterOutput;
	unsigned int filterCount, sampleCount, filterUpdateInt;
	std::vector<AllPassFilter> filters;
};

