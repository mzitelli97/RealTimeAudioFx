#pragma once
#include "Effect.h"
#include "UniversalCombFilter.h"
#include <vector>

#define COMB_FILTER_COUNT 4
#define ALLPASS_FILTER_COUNT 3
#define REVERB_TIME_DEFAULT 2
#define MIX_DEFAULT 0.5

class EffectReverbFull :
	public Effect
{
public:
	EffectReverbFull();
	bool next(const void * inputBuffer, void * outputBuffer, unsigned long framesPerBuffer)override;
	virtual ~EffectReverbFull();
private:
	std::vector<UniversalCombFilter> combFilters;
	std::vector<UniversalCombFilter> allPassFilters;
	std::vector<float> combDelays;

};

