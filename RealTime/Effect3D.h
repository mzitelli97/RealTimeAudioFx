#pragma once
#include "Effect.h"
class Effect3D :
	public Effect
{
	
	public:
	bool next(const void * inputBuffer, void * outputBuffer, unsigned long framesPerBuffer)override;
	Effect3D();
	~Effect3D();

	private:
	bool hsFilter(const void * inputBuffer, std::vector<float> * outputBuffer, unsigned long framesPerBuffer, float theta);
	


	
};

