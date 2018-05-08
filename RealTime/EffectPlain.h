#pragma once
#include "Effect.h"
class EffectPlain :
	public Effect
{
public:
	EffectPlain();
	bool next(const void * inputBuffer, void * outputBuffer, unsigned long framesPerBuffer);
	~EffectPlain();
};

