#pragma once
#include "Effect.h"
class Effect3D :
	public Effect
{
	
	public:
	bool next(const void * inputBuffer, void * outputBuffer, unsigned long framesPerBuffer)override;
	bool setProp(unsigned i, double v)override;
	Effect3D();
	~Effect3D();

	private:
	bool hsFilter(const void * inputBuffer, std::vector<float> * outputBuffer, unsigned long framesPerBuffer, float theta,char channel);
	float prevInL, prevInR, prevHSoutL, prevHSoutR, prevOutL, prevOutR;
	


	
};

