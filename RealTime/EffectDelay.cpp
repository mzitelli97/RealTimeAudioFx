#include "EffectDelay.h"
#include <iostream>


EffectDelay::EffectDelay() :Effect(std::string("Delay"))
{
	props = {Properties(std::string("Feedback"),0,1),Properties(std::string("Dry Wet"),0,1),Properties(std::string("Delay"),0,1024) };
	props[0].setValue(0.6);
	props[1].setValue(0.7);
	props[2].setValue(200);
	filter = new UniversalCombFilter(false);
	filter->setDelay(props[2].getValue() * sampleRate / 1000.0);
	buff = std::vector<float>((unsigned)(props[2].getValue() * sampleRate * 2.0 / 1000.0),0);
	dpw = 0; // As the buffer will be circular (else, infinite memory would be needed) we need a write pointer
	dpr = buff.size()/2;
}

bool EffectDelay::next(const void * inputBuffer, void * outputBuffer, unsigned long framesPerBuffer)
{
	//esto implementa un filtro comb universal
	float *in = (float*)inputBuffer;
	float *out = (float*)outputBuffer;
	/*float BL = -0.7; float FB = 0.7; float FF = 1;
	for (unsigned long i = 0; i < framesPerBuffer; i++) //Every sample should be processed
	{
		float temp = buff[(dpr + i) % buff.size()]; //The older sample is retrieved
													//std::cout << *out << '\n'; 
		float xh = (float)(props[1].getValue() * *(in++) + FB * temp);
		out[i] = FF * temp + BL * xh; //And is added to the current sample (with a coefficient) LINE A
		buff[(dpw + i) % buff.size()] = xh; // The output is saved (also with a coefficient) LINE B
	}
	for (unsigned i = 0; i < framesPerBuffer; i++)
	{
		out[2 * framesPerBuffer - 1 - 2 * i] = out[framesPerBuffer - 1 - i];
		out[2 * framesPerBuffer - 2 - 2 * i] = out[framesPerBuffer - 1 - i];
	}/*
	//To explain what this does in terms of digital systems and signals analysis, this "effect" has the following shape
	// y(n) = feedback_*y(n-delay_)+dry_wet_*x(n)
	//The second term is easy to appreciate in LINE A, where the raw input is taken directly to the ouptut (attenuated by dry_wet_)
	//The first term of the equation appears as "temp" in LINE A. As you can see, in LINE B the output is stored with a coefficient (feedback_).
	//delay_ samples later temps adquires that stored valued, so it appears as "feedback_*y(n-delay)" !

	//WARNING: Be very careful with systems' stability. In this example, using feedback_ > 1 would increase the otput at each cycle
	//ending in saturation (and believe me, your PC's speakers don't like that too much) 

	//Pointer incrementation, considering the buffer is circular
	/*dpw += framesPerBuffer;
	dpr += framesPerBuffer;
	dpw %= buff.size();
	dpr %= buff.size();*/
	filter->combFilter(-0.7, 0.7, 1, in, out, framesPerBuffer);
	for (unsigned i = 0; i < framesPerBuffer; i++)
	{
		out[2 * framesPerBuffer - 1 - 2 * i] = out[framesPerBuffer - 1 - i];
		out[2 * framesPerBuffer - 2 - 2 * i] = out[framesPerBuffer - 1 - i];
	}
	return true;
}

bool EffectDelay::setProp(unsigned i, double v)
{
	bool ret = false;
	if (i < props.size())
	{
		ret = props[i].setValue(v);
		if (i == 2 && ret)
		{
			buff.resize(props[2].getValue() * sampleRate * 2.0 / 1000.0, 0);
			dpw = 0;
			dpr = buff.size() / 2;
			filter->setDelay(props[2].getValue() * sampleRate / 1000.0);
		}
	}
	return ret;
}

EffectDelay::~EffectDelay()
{
	if (filter != nullptr)
		delete filter;
}
