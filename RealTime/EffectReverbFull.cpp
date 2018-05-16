#include "EffectReverbFull.h"

//Tiempo de delay de los filtos comb para lograr la densidad de ecos de 1000/seg
#define COMB_DELAY_TIMES {0.030f, 0.037f, 0.043f, 0.050f}

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
		combFilters[i].setDelay(combDelays[i] * sampleRate);	//Seteo el delay para cada comb
		combFilters[i].setLowPass(true,2000/sampleRate);		//Seteo que tengan un pasabajos en la realimentacion
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
	//Filtros comb en paralelo
	for (auto& filter : combFilters)
	{
		//Calculo de las ganancias para obtener el tiempo de reverberacion deseado
		float g = pow(10, -3 * combDelays[ind++] / props[0].getValue());
		filter.combFilter(0, g, 1, in, (float*)combBuff, framesPerBuffer);
		//Se hace el promedio de las salidas de los filtros comb
		for (int i = 0; i < framesPerBuffer; i++)
			aux[i] += (1 / (float)COMB_FILTER_COUNT) * combBuff[i];
	}

	//Filtros allPass en serie
	for (auto& filter : allPassFilters)
	{
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
