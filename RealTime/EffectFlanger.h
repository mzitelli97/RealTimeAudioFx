#pragma once
#include "Effect.h"
class EffectFlanger :
	public Effect
{
public:
	EffectFlanger();
	bool next(const void * inputBuffer, void * outputBuffer, unsigned long framesPerBuffer)override;
	bool setProp(unsigned i, double v)override;
	virtual ~EffectFlanger();
private:
	std::vector<float> buff;
	unsigned dpw, counter;
	float dpr;
};