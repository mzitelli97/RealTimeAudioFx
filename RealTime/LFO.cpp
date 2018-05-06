#include "LFO.h"
#include <cmath>

#define PI 3.14159265
LFO::LFO()
{
}

float LFO::execute_lfo(float sampleRate, float freq, waveformType waveType)
{
	float lfo = 0;
	switch (waveType)
	{
	case Sine:
		lfo = (1 + sin(2 * PI*freq / sampleRate)) / 2.0;
		break;
	case Triangular:
		break;
	default:
		break;
	}
	return lfo;
}



LFO::~LFO()
{
}
