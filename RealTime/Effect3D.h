#pragma once
#include "Effect.h"
class Effect3D :
	public Effect
{
public:
	bool next(const void * inputBuffer, void * outputBuffer, unsigned long framesPerBuffer)override;

	Effect3D();
	bool hsFilter(const void * inputBuffer, unsigned long framesPerBuffer, void * outputBuffer, double theta, double Fs);
	std::vector<float> buff;


	~Effect3D();
};

