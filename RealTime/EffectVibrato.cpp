#include "EffectVibrato.h"
#include <iostream>
#include <cmath>
#include <queue>

#define PI 3.14159265
EffectVibrato::EffectVibrato() :Effect(std::string("Delay"))
{
	props = { Properties(std::string("Delay"),0,1024),Properties(std::string("Frequency"),0,5),Properties(std::string("Width"),0,100) };
	props[0].setValue(0.5);
	props[1].setValue(5);
	props[2].setValue(0.01);
	std::cout << "Setting size to " << (unsigned)(props[2].getValue() * 44100 * 2);
	buff = std::vector<float>(2+(unsigned)( 100 *44100*2), 0);
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
	/*unsigned int delaySize = 0;
	if (delaySize != 0)
	{
		for (unsigned int i = 0; i < framesPerBuffer; i++)
		{
			*out++ = *in + attenuation * prevInput[i%delaySize];
			prevInput[i%delaySize] = *in++;
		}
	}
	else
		for (unsigned int i = 0; i < framesPerBuffer; i++)
			*out++ = *in++;
	counter += 1/1000;
	if (counter >(1 / delayFreq))	counter = 0;

	delaySize = stof(FLANGER_AVG_DELAY) * (1 + abs(floor(delayPeak*sin(2 * PI * delayFreq * counter)))) * 2;
	prevInput.resize(delaySize, 0);
	*/

	float modFreq = props[2].getValue() / (44100 * 2);
	float BL = 0; float FF = 1; float FB = 0;
	for (unsigned long i = 0; i<framesPerBuffer; i++) //Every sample should be processed
	{
		float phase = modFreq * i;
		if (phase >= 1) phase -= 1;

		float currentDelay = props[2].getValue() * (0.5 + 0.5 * sinf(2 * PI * phase));
		dpr = fmodf((float)dpw - (float)(currentDelay * 44100) + (float)buff.size() - 2.0, (float)buff.size());
		unsigned M = floor(dpr);
		float fraq = dpr - M;

		float xh_M = fraq * buff[(M+1)%buff.size()]+ (1 - fraq) * buff[M%buff.size()];
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

	//dpw += framesPerBuffer;
	//dpr += framesPerBuffer;
	//dpw %= buff.size();
	//dpr %= buff.size();
	return true;
}