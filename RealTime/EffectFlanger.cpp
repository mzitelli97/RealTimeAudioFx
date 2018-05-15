#include "EffectFlanger.h"
#include "LFO.h"
#include <iostream>
#include <cmath>

#define FREQ_DEFAULT 0.5
#define WIDTH_DEFAULT 1
#define MIX_DEFAULT 0.7
#define DELAY_DEFAULT 5

#define PI 3.14159265
EffectFlanger::EffectFlanger() :Effect(std::string("Flanger"))
{
	props = { Properties(std::string("Frequency"),0.1,10),Properties(std::string("Width"),0.2,10), Properties(std::string("Mix"),0,1),
		Properties(std::string("Delay"),0,10) };
	props[0].setValue(FREQ_DEFAULT);
	props[1].setValue(WIDTH_DEFAULT);	//Width of modulation in ms
	props[2].setValue(MIX_DEFAULT);
	props[3].setValue(DELAY_DEFAULT);	//Minimun delay in ms
	buff = std::vector<float>(( (props[1].getValue() + props[3].getValue()) / 1000.0) * sampleRate + 2, 0);
	dpw = 0; // As the buffer will be circular (else, infinite memory would be needed) we need a write pointer
	counter = 0;
}


EffectFlanger::~EffectFlanger()
{
}

bool EffectFlanger::next(const void * inputBuffer, void * outputBuffer, unsigned long framesPerBuffer)
{
	float *in = (float*)inputBuffer;
	float *out = (float*)outputBuffer;
	float BL = 0.7; float FF = props[2].getValue(); float FB = 0.7;
	for (unsigned long i = 0; i<framesPerBuffer; i++) //Every sample should be processed
	{
		float phase = props[0].getValue() * (counter++);
		if (phase >= sampleRate)
		{
			phase = 0;
			counter = 0;
		}
		float maxDev = sampleRate * (props[1].getValue() / 1000.0);
		float currentDelay = sampleRate * (props[3].getValue() / 1000.0) + maxDev * lfo(sampleRate, phase, Sine);	//Modulated delay
		dpr = dpw + currentDelay + 1.0;	//Position to read given the actual sample and the delay
		while (dpr >= buff.size()) dpr -= buff.size();

		//Linear Interpolation
		unsigned M = floor(dpr);
		float fraq = dpr - M;
		float xh_M = fraq * buff[(M + 1) % buff.size()] + (1 - fraq) * buff[M%buff.size()];

		float xh = in[i] + FB * xh_M;
		buff[dpw] = xh;
		dpw = (++dpw) % buff.size();
		out[i] = BL * xh + FF * xh_M;
	}
	//Duplicate the output for stereo
	for (unsigned i = 0; i < framesPerBuffer; i++)
	{
		out[2 * framesPerBuffer - 1 - 2 * i] = out[framesPerBuffer - 1 - i];
		out[2 * framesPerBuffer - 2 - 2 * i] = out[framesPerBuffer - 1 - i];
	}
	return true;
}

bool EffectFlanger::setProp(unsigned i, double v)
{
	bool ret = false;
	if (i < props.size())
	{
		ret = props[i].setValue(v);
		if (i >= 1 && ret)
		{
			buff.resize( ((props[1].getValue() + props[3].getValue()) / 1000.0) * sampleRate + 2, 0);
			dpw = 0;
		}
	}

	return ret;
}