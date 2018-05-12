#pragma once
#include "Effect.h"
#include "UniversalCombFilter.h"

enum { Echo = 0, Plane, AllPass };			//Type of delay
#define DELAY_DEFAULT 200
#define FB_DEFAULT 0.7
#define FF_DEFAULT 0.7

class EffectDelay :
	public Effect
{
public:
	EffectDelay();
	bool next(const void * inputBuffer, void * outputBuffer, unsigned long framesPerBuffer)override;
	bool setProp(unsigned i, double v)override;
	~EffectDelay();
private:
	UniversalCombFilter *filter;
};

