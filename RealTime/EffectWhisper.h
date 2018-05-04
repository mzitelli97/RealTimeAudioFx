#pragma once
#include "Effect.h"
#include "FFT.h"
class EffectWhisper :
	public Effect
{
public:
	EffectWhisper();
	~EffectWhisper();
	bool next(const void *inputBuffer, void *outputBuffer, unsigned long framesPerBuffer)override;
private:
	vector<complex<float>> in;
	vector<complex<float>> temp;
};

