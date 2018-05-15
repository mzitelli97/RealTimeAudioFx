#include "EffectReverbLP.h"


EffectReverbLP::EffectReverbLP() : Effect(std::string("Reverb LP"))
{
	props = { Properties(std::string("Delay"),0,1000) , Properties(std::string("Feedback"),0,1),  
		Properties(std::string("Filter Frequency"),0,sampleRate/4) };
	props[0].setValue(DELAY_DEFAULT);
	props[1].setValue(FB_DEFAULT);
	props[2].setValue(FREQ_DEFAULT);

	filter = new UniversalCombFilter();
	filter->setDelay(props[0].getValue() * sampleRate / 1000.0);
	filter->setLowPass(true, props[2].getValue() / sampleRate);
}

bool EffectReverbLP::next(const void * inputBuffer, void * outputBuffer, unsigned long framesPerBuffer)
{
	float *in = (float*)inputBuffer;
	float *out = (float*)outputBuffer;
	filter->combFilter(1, props[1].getValue(), 0, in, out, framesPerBuffer);

	//For stereo output
	for (unsigned i = 0; i < framesPerBuffer; i++)
	{
		out[2 * framesPerBuffer - 1 - 2 * i] = out[framesPerBuffer - 1 - i];
		out[2 * framesPerBuffer - 2 - 2 * i] = out[framesPerBuffer - 1 - i];
	}
	return true;
}

bool EffectReverbLP::setProp(unsigned i, double v)
{
	bool ret = false;
	if (i < props.size())
	{
		ret = props[i].setValue(v);
		if (i == 0 && ret)
			filter->setDelay(props[0].getValue() * sampleRate / 1000.0);
		if (i == 2 && ret)
			filter->setLowPass(true, props[2].getValue() / sampleRate);
	}
	return ret;
}

EffectReverbLP::~EffectReverbLP()
{
	if (filter != nullptr)
		delete filter;
}
