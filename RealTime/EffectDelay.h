#pragma once
#include "Effect.h"
class EffectDelay :
	public Effect
{
public:
	EffectDelay();
	bool next(const void * inputBuffer, void * outputBuffer, unsigned long framesPerBuffer)override;
	~EffectDelay();
private:
	std::vector<float> buff;
	unsigned dpw,dpr;
};

