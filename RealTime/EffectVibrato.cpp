#include "EffectVibrato.h"
#include <iostream>
#include <cmath>
#include <queue>

#define PI 3.14159265
EffectVibrato::EffectVibrato() :Effect(std::string("Vibrato"))
{
	props = { Properties(std::string("Frequency"),0,100),Properties(std::string("Width"),0,100) };
	props[0].setValue(50);
	props[1].setValue(0.1);	//Width of modulation in ms
	buff = std::vector<float>((props[1].getValue()/1000)*44100, 0);
	dpw = 0; // As the buffer will be circular (else, infinite memory would be needed) we need a write pointer
	dpr = buff.size() / 2;
	counter = 0;
	for (int i = 0; i < 2 + (unsigned)((props[0].getValue() + 2 * props[1].getValue()) * 44100 * 2); i++)
		delayLine.push_back(0);	
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

		float currentDelay = (props[1].getValue()/1000) * (0.5 + 0.5 * sinf(2 * PI * phase));	//Modulated delay
		dpr = dpw - (currentDelay * 44100) - 2.0;	//Position to read given the actual sample and the delay
		while (dpr < 0) dpr += buff.size();

		unsigned M = floor(dpr);
		float fraq = dpr - M;

		float xh_M = fraq * buff[(M+1)%buff.size()]+ (1 - fraq) * buff[M%buff.size()];	//Linear interpolation
		float xh = *(in++) + FB * xh_M;

		buff[dpw] = xh;
		if (++dpw >= buff.size())
			dpw = 0;
		*out = BL * xh + FF * xh_M;
		out++;
		/*float zeiger = 0.01 * (0.5 + 0.5 * sin(2*PI * phase));
		int M = floor(zeiger);
		float fraq = zeiger - M;
		std::list<float>::iterator it = delayLine.begin();
		std::advance(it, M);
		float temp2 = *it;
		float temp1 = *(++it);

		float xh_M = temp1 * fraq + temp2 * (1 - fraq);
		float xh = *(in++) + FB * xh_M;

		delayLine.push_front(xh);
		delayLine.pop_back();
		
		*out = BL * xh + FF * xh_M;
		out++;*/
	}

	return true;
}