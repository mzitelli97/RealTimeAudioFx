#include "EffectVibrato.h"
#include <iostream>
#include <cmath>
#include <queue>

#define PI 3.14159265
EffectVibrato::EffectVibrato() :Effect(std::string("Delay"))
{
	props = { Properties(std::string("Delay"),0,1024),Properties(std::string("Width"),0,1024),Properties(std::string("ModFreq"),0,20) };
	props[0].setValue(0);
	props[1].setValue(0.5);
	props[2].setValue(1);
	std::cout << "Setting size to " << (unsigned)(props[2].getValue() * 44100 * 2);
	buff = std::vector<float>(2+(unsigned)(props[0].getValue()+ 2*props[1].getValue()), 0);
	dpw = 0; // As the buffer will be circular (else, infinite memory would be needed) we need a write pointer
	dpr = buff.size() / 2;
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
	float modFreq = props[2].getValue() / (44100 * 2);
	float BL = 0.5; float FB = 0; float FF = 0.7;
	for (unsigned long i = 0; i<framesPerBuffer; i++) //Every sample should be processed
	{
		float mod = sin(2*PI*modFreq*i);
		float zeiger = 1 + props[0].getValue() + props[1].getValue() * mod;
		int M = floor(zeiger);
		float fraq = zeiger - i;

		delayLine.push_front(*(in++));
		delayLine.pop_back();
		
		//float temp = buff[(dpr + i) % buff.size()]; //The older sample is retrieved
													//std::cout << *out << '\n'; 
		float temp2=0, temp1=0;
		int j = 0;
		for (std::list<float>::iterator it = delayLine.begin(); j < M; it++, j++)
		{
			temp2 = *it;
			temp1 = *(++it);
			break;
		}
		//float xh = (float)(props[1].getValue() * *(in++) +FB * temp);
		//*out = FF * temp + BL * xh; //And is added to the current sample (with a coefficient) LINE A
		*out = temp1 * fraq + temp2 * (1 - fraq);
		out++;
		//buff[(dpw + i) % buff.size()] = xh; // The output is saved (also with a coefficient) LINE B
	}

	dpw += framesPerBuffer;
	dpr += framesPerBuffer;
	dpw %= buff.size();
	dpr %= buff.size();
	return true;
}