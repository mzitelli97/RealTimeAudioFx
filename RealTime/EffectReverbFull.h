#pragma once
#include "Effect.h"
#include "UniversalCombFilter.h"
#include <vector>

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

