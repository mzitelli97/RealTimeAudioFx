#pragma once
#include "Effect.h"
#define GAIN_DEFAULT 1
#define THRESHOLD_DEFAULT 0.01

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

