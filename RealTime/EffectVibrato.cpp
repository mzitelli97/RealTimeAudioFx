#include "EffectVibrato.h"
#include "LFO.h"
#include <iostream>
#include <cmath>
#include <queue>

#define FREQ_DEFAULT 1
#define WIDTH_DEFAULT 10
#define DEPTH_DEFAULT 1	//for chorus effect

#define PI 3.14159265
EffectVibrato::EffectVibrato() :Effect(std::string("Vibrato"))
{
	props = { Properties(std::string("Frequency"),0.1,15),Properties(std::string("Width"),0.2,30), Properties(std::string("Depht"),0,1) };
	props[0].setValue(FREQ_DEFAULT);
	props[1].setValue(WIDTH_DEFAULT);	//Width of modulation in ms
	props[2].setValue(DEPTH_DEFAULT);
	buff = std::vector<float>((props[1].getValue()/1000.0) * sampleRate * (1 + props[2].getValue()), 0);
	dpw = 0; // As the buffer will be circular (else, infinite memory would be needed) we need a write pointer
	counter = 0;
}


EffectVibrato::~EffectVibrato()
{
}

bool EffectVibrato::next(const void * inputBuffer, void * outputBuffer, unsigned long framesPerBuffer)
{
	float *in = (float*)inputBuffer;
	float *out = (float*)outputBuffer;
	for (unsigned long i = 0; i<framesPerBuffer; i++) //Every sample should be processed
	{
		float phase = props[0].getValue() * (counter++);
		if (phase >= sampleRate)
		{
			phase = 0;
			counter = 0;
		}
		float maxDev = sampleRate * (props[1].getValue() / 1000.0) * props[2].getValue();
		if (maxDev == 0) maxDev = 1;
		float currentDelay = maxDev * 2 * lfo(sampleRate, phase, Sine);	//Modulated delay
		dpr = dpw + currentDelay + 2.0;	//Position to read given the actual sample and the delay
		while (dpr >= buff.size()) dpr -= buff.size();

		//Linear Interpolation
		unsigned M = floor(dpr);
		float fraq = dpr - M;
		float xh_M = fraq * buff[(M+1)%buff.size()]+ (1 - fraq) * buff[M%buff.size()];

		buff[dpw] = in[i];
		dpw = (++dpw) % buff.size();
		out[i] = xh_M;
	}
	//Duplicate the output for stereo
	for (unsigned i = 0; i < framesPerBuffer; i++)
	{
		out[2 * framesPerBuffer - 1 - 2 * i] = out[framesPerBuffer - 1 - i];
		out[2 * framesPerBuffer - 2 - 2 * i] = out[framesPerBuffer - 1 - i];
	}
	return true;
}

bool EffectVibrato::setProp(unsigned i, double v)
{
	bool ret = false;
	if (i < props.size())
	{
		ret = props[i].setValue(v);
		if (i >= 1 && ret)
		{
			buff.resize((props[1].getValue() / 1000.0) * sampleRate * (1 + props[2].getValue()), 0);
			dpw = 0;
		}
	}
	
	return ret;
}