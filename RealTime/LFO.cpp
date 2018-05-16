#include "LFO.h"
#include <cmath>

#define PI 3.14159265

//Implementa diferentes tipos de osciladores de baja frecuencia
float lfo(float sampleRate, float freq, waveformType waveType)
{
	float lfo = 0;
	float normFreq = freq / sampleRate;
	switch (waveType)
	{
	case Sine:
		lfo = (1 + sin(2 * PI * normFreq)) / 2.0;
		break;
	case Triangular:
		if (normFreq < 0.25)
			lfo = 0.5f + 2.0 * normFreq;
		else if (normFreq < 0.75)
			lfo = 1.0f - 2.0f*(normFreq - 0.25f);
		else
			lfo = 2.0f*(normFreq - 0.75f);
		break;
	case Square:
		if (normFreq < 0.5)
			lfo = 1.0;
		else
			lfo = 0;
		break;
	case Sawtooth:
		if (normFreq < 0.5f)
			lfo = 0.5f + normFreq;
		else
			lfo = normFreq - 0.5f;
	default:
		break;
	}
	return lfo;
}