#pragma once
#include "Effect.h"

typedef enum { HardClipping, SoftClipping, ExpSoftClipping, HalfWaveRect, FullWaveRect, Distortion_count }Distortion_type;
class EffectDistortionHC :
	public Effect
{
public:
	EffectDistortionHC();
	bool next(const void * inputBuffer, void * outputBuffer, unsigned long framesPerBuffer)override;
	bool setProp(unsigned i, double v)override;
	virtual ~EffectDistortionHC();
private:
	Distortion_type type;
};

