#include "EffectReverbFull.h"



EffectReverbFull::EffectReverbFull() : Effect(std::string("Full Reverb"))
{
}

bool EffectReverbFull::next(const void * inputBuffer, void * outputBuffer, unsigned long framesPerBuffer)
{
	float *in = (float*)inputBuffer;
	float *out = (float*)outputBuffer;

	for (auto& filter : combFilters)
	{
		//Filtros comb en paralelo
		//ve
		filter.combFilter(0,0,0,in,out,framesPerBuffer);
	}

	for (auto& filter : allPassFilters)
	{
		//Filtros allPass en serie
		filter.combFilter(0, 0, 0, out, out, framesPerBuffer);
	}

	for (unsigned i = 0; i < framesPerBuffer; i++)
	{
		out[2 * framesPerBuffer - 1 - 2 * i] = out[framesPerBuffer - 1 - i];
		out[2 * framesPerBuffer - 2 - 2 * i] = out[framesPerBuffer - 1 - i];
	}
	return true;
}

bool EffectReverbFull::setProp(unsigned i, double v)
{
	return true;
}


EffectReverbFull::~EffectReverbFull()
{
}
