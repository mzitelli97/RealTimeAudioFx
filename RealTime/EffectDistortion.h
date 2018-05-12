#pragma once
#include "Effect.h"

typedef enum { HardClipping, SoftClipping, ExpSoftClipping, HalfWaveRect, FullWaveRect, Distortion_count }Distortion_type;
class EffectDistortion :
	public Effect
{
public:
	EffectDistortion();
	bool next(const void * inputBuffer, void * outputBuffer, unsigned long framesPerBuffer)override;
	bool setProp(unsigned i, double v)override;
	virtual ~EffectDistortion();
private:
	Distortion_type type;
};

