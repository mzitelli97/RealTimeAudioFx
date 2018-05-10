#pragma once
#include "Effect.h"
#include "UniversalCombFilter.h"
#include <vector>

#define COMB_FILTER_COUNT 4
#define ALLPASS_FILTER_COUNT 5

class EffectReverbFull :
	public Effect
{
public:
	EffectReverbFull();
	bool next(const void * inputBuffer, void * outputBuffer, unsigned long framesPerBuffer)override;
	bool setProp(unsigned i, double v)override;
	virtual ~EffectReverbFull();
private:
	std::vector<UniversalCombFilter> combFilters;
	std::vector<UniversalCombFilter> allPassFilters;

};

