#pragma once
#include "Effect.h"
#include "UniversalCombFilter.h"

#define DELAY_DEFAULT 200
#define FB_DEFAULT 0.7
#define FREQ_DEFAULT 5500	//In Hz

class EffectReverbLP :
	public Effect
{
public:
	EffectReverbLP();
	bool next(const void * inputBuffer, void * outputBuffer, unsigned long framesPerBuffer)override;
	bool setProp(unsigned i, double v)override;
	virtual ~EffectReverbLP();
private:
	UniversalCombFilter *filter;
};

