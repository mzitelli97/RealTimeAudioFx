#pragma once
#include "Effect.h"
#include "FFT.h"
class EffectRobot :
	public Effect
{
public:
	EffectRobot();
	bool next(const void *inputBuffer, void *outputBuffer, unsigned long framesPerBuffer)override;
	~EffectRobot();
private:
	vector<complex<float>> in;
	vector<complex<float>> temp;
};

