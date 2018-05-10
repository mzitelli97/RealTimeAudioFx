#include "EffectReverbFull.h"

#define COMB_DELAY_TIMES {0.03f, 0.0367f, 0.0433f, 0.05f}
#define AP_DELAY_TIMES {0.02f, 0.03f}
#define E 2.718281828459045235360
EffectReverbFull::EffectReverbFull() : Effect(std::string("Full Reverb"))
{
	props = { Properties(std::string("Feedback"),0,1),Properties(std::string("Dry Wet"),0,1),Properties(std::string("Delay"),0,1024) };
	combFilters = std::vector<UniversalCombFilter>(COMB_FILTER_COUNT);
	allPassFilters = std::vector<UniversalCombFilter>(ALLPASS_FILTER_COUNT);
	float combDelay []= { 0.03, 0.0367, 0.0433, 0.05 };
	float allPassDelay[] = { 0.02, 0.03 };
	for (int i = 0; i < COMB_FILTER_COUNT; i++)
	{
		combFilters[i].setDelay(combDelay[i] * sampleRate);
	}

	for (int i = 0; i < ALLPASS_FILTER_COUNT; i++)
	{
		allPassFilters[i].setDelay( (0.1 / pow(3,i)) * sampleRate);
	}
}

bool EffectReverbFull::next(const void * inputBuffer, void * outputBuffer, unsigned long framesPerBuffer)
{
	float *in = (float*)inputBuffer;
	float *out = (float*)outputBuffer;
	float* aux = (float*)calloc(framesPerBuffer, sizeof(float));
	float* combBuff = (float*)calloc(framesPerBuffer, sizeof(float));
	float combDelay[] = { 0.03, 0.0367, 0.0433, 0.05 };
	int ind = 0;
	for (auto& filter : combFilters)
	{

		//Filtros comb en paralelo
		float g = pow(E, -3 * combDelay[ind++] / 2.0);
		filter.combFilter(1,g,0,in,(float*)combBuff,framesPerBuffer);
		//Se hace el promedio de las salidas de los filtros comb
		for (int i = 0; i < framesPerBuffer; i++)
			aux[i] += (1 / (float)COMB_FILTER_COUNT) * combBuff[i];
	}

	for (auto& filter : allPassFilters)
	{
		//Filtros allPass en serie
		filter.combFilter(-0.708, 0.708, 1, aux, aux, framesPerBuffer);
	}
	for (unsigned int i = 0; i < framesPerBuffer; i++)
		out[i] = in[i] + 1 * aux[i];
	
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

bool EffectReverbFull::setProp(unsigned i, double v)
{
	return true;
}


EffectReverbFull::~EffectReverbFull()
{
}
