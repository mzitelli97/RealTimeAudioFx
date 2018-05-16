#include "EffectDelay.h"
#include <iostream>


EffectDelay::EffectDelay() :Effect(std::string("Delay"))
{
	props = { Properties(std::string("Delay [ms]"),0,1000),Properties(std::string("Feedback"),0,1),
		Properties(std::string("Feedforward"),-1,1), Properties(std::string("Type [Echo, Plane, AllPass]"),0,2) };
	props[0].setValue(DELAY_DEFAULT);	//Delay in ms
	props[1].setValue(FB_DEFAULT);
	props[2].setValue(FF_DEFAULT);
	props[3].setValue(AllPass);
	filter = new UniversalCombFilter();
	filter->setDelay(props[0].getValue() * sampleRate / 1000.0);
}

bool EffectDelay::next(const void * inputBuffer, void * outputBuffer, unsigned long framesPerBuffer)
{
	float *in = (float*)inputBuffer;
	float *out = (float*)outputBuffer;
	
	std::vector<double> parameters(3);
	if (props[3].getValue() == Echo)
		parameters = { 1,0,props[2].getValue() };	//BL, FB, FF
	else if (props[3].getValue() == Plane)
		parameters = { 1,props[1].getValue(),0 };	//BL, FB, FF
	else //AllPass
		parameters = {-props[1].getValue(), props[1].getValue(), 1};	//BL, FB, FF
	//Apply the corresponding configuration of the universal comb filter
	filter->combFilter(parameters[0], parameters[1], parameters[2], in, out, framesPerBuffer);

	//For stereo output
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
		if (i == 3) v = floor(v);	//Redondeo para tomar el tipo de delay
		ret = props[i].setValue(v);
		if (i == 0 && ret)
			filter->setDelay(props[0].getValue() * sampleRate / 1000.0);
	}
	return ret;
}

EffectDelay::~EffectDelay()
{
	if (filter != nullptr)
		delete filter;
}
