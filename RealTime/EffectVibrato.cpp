#include "EffectVibrato.h"
#include <iostream>
#include <cmath>
#include <queue>

#define PI 3.14159265
EffectVibrato::EffectVibrato() :Effect(std::string("Vibrato"))
{
	props = { Properties(std::string("Frequency"),0,100),Properties(std::string("Width"),0,100) };
	props[0].setValue(1);
	props[1].setValue(0.1);	//Width of modulation in ms
	buff = std::vector<float>((props[1].getValue()/1000)*44100, 0);
	dpw = 0; // As the buffer will be circular (else, infinite memory would be needed) we need a write pointer
	dpr = buff.size() / 2;
	counter = 0;
}


EffectVibrato::~EffectVibrato()
{
}

bool EffectVibrato::next(const void * inputBuffer, void * outputBuffer, unsigned long framesPerBuffer)
{

	float *in = (float*)inputBuffer;
	float *out = (float*)outputBuffer;

	float modFreq = props[0].getValue() / (44100);
	float BL = 0; float FF = 1; float FB = 0;
	for (unsigned long i = 0; i<framesPerBuffer; i++) //Every sample should be processed
	{
		float phase = modFreq * (counter++);
		if (phase >= 1)
		{
			phase = 0;
			counter = 0;
		}

		float currentDelay = 44100.0 * (props[1].getValue()/1000) * (1 + 1 * sin(2 * PI * phase));	//Modulated delay
		dpr = dpw + currentDelay + 2.0 + 10.0;	//Position to read given the actual sample and the delay
		while (dpr >= buff.size()) dpr -= buff.size();

		unsigned M = floor(dpr);
		float fraq = dpr - M;

		float xh_M = fraq * buff[(M+1)%buff.size()]+ (1 - fraq) * buff[M%buff.size()];	//Linear interpolation
		float xh = *(in++) + FB * xh_M;

		buff[dpw] = xh;
		if (++dpw >= buff.size()) dpw = 0;
		out[i] = BL * xh + FF * xh_M;
		//out++;
	}
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
		if (i == 1) buff.resize((props[1].getValue() / 1000) * 44100, 0);
	}
	
	return ret;
}