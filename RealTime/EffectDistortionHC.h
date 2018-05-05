#pragma once
#include "Effect.h"
class EffectDistortionHC :
	public Effect
{
public:
	EffectDistortionHC();
	bool next(const void * inputBuffer, void * outputBuffer, unsigned long framesPerBuffer)override;
	virtual ~EffectDistortionHC();
};

