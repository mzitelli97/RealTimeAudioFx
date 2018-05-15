#include "EffectReverbFull.h"

#define COMB_DELAY_TIMES {0.030f, 0.037f, 0.043f, 0.050f}
#define E 2.718281828459045235360

EffectReverbFull::EffectReverbFull() : Effect(std::string("Full Reverb"))
{
	props = { Properties(std::string("Reverberation time"),0.01,5), Properties(std::string("Mix"),0,1) };
	props[0].setValue(REVERB_TIME_DEFAULT);
	props[1].setValue(MIX_DEFAULT);
	combFilters = std::vector<UniversalCombFilter>(COMB_FILTER_COUNT);
	allPassFilters = std::vector<UniversalCombFilter>(ALLPASS_FILTER_COUNT);
	combDelays = COMB_DELAY_TIMES;
	for (int i = 0; i < COMB_FILTER_COUNT; i++)
	{
		combFilters[i].setDelay(combDelays[i] * sampleRate);
		combFilters[i].setLowPass(true,2000/sampleRate);
	}

	for (int i = 0; i < ALLPASS_FILTER_COUNT; i++)
	{
		allPassFilters[i].setDelay( (0.10 / pow(3,i)) * sampleRate);
	}
}

bool EffectReverbFull::next(const void * inputBuffer, void * outputBuffer, unsigned long framesPerBuffer)
{
	float *in = (float*)inputBuffer;
	float *out = (float*)outputBuffer;
	float* aux = (float*)calloc(framesPerBuffer, sizeof(float));
	float* combBuff = (float*)calloc(framesPerBuffer, sizeof(float));
	int ind = 0;
	for (auto& filter : combFilters)
	{

		//Filtros comb en paralelo
		float g = pow(10, -3 * combDelays[ind++] / props[0].getValue());
		filter.combFilter(0, g, 1, in, (float*)combBuff, framesPerBuffer);
		//Se hace el promedio de las salidas de los filtros comb
		for (int i = 0; i < framesPerBuffer; i++)
			aux[i] += (1 / (float)COMB_FILTER_COUNT) * combBuff[i];
	}

	for (auto& filter : allPassFilters)
	{
		//Filtros allPass en serie
		filter.combFilter(-0.7, 0.7, 1, aux, aux, framesPerBuffer);
	}
	for (unsigned int i = 0; i < framesPerBuffer; i++)
		out[i] = (1 - props[1].getValue()) * in[i] + props[1].getValue() * aux[i];
	
	free(aux);
	free(combBuff);

	//For stereo output
	for (unsigned i = 0; i < framesPerBuffer; i++)
	{
		out[2 * framesPerBuffer - 1 - 2 * i] = out[framesPerBuffer - 1 - i];
		out[2 * framesPerBuffer - 2 - 2 * i] = out[framesPerBuffer - 1 - i];
	}
	return true;
}


EffectReverbFull::~EffectReverbFull()
{
}
