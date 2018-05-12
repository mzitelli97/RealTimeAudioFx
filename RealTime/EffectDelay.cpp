#include "EffectDelay.h"
#include <iostream>


EffectDelay::EffectDelay() :Effect(std::string("Delay"))
{
	props = { Properties(std::string("Delay [ms]"),0,1000),Properties(std::string("Feedback"),0,1),
		Properties(std::string("Feedforward"),0,1), Properties(std::string("Type [Echo, Plane, AllPass]"),0,2) };
	props[0].setValue(DELAY_DEFAULT);	//Delay in ms
	props[1].setValue(FB_DEFAULT);
	props[2].setValue(FF_DEFAULT);
	props[3].setValue(AllPass);
	filter = new UniversalCombFilter(false);
	filter->setDelay(props[0].getValue() * sampleRate / 1000.0);
}

bool EffectDelay::next(const void * inputBuffer, void * outputBuffer, unsigned long framesPerBuffer)
{
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
	}/*
	//To explain what this does in terms of digital systems and signals analysis, this "effect" has the following shape
	// y(n) = feedback_*y(n-delay_)+dry_wet_*x(n)
	//The second term is easy to appreciate in LINE A, where the raw input is taken directly to the ouptut (attenuated by dry_wet_)
	//The first term of the equation appears as "temp" in LINE A. As you can see, in LINE B the output is stored with a coefficient (feedback_).
	//delay_ samples later temps adquires that stored valued, so it appears as "feedback_*y(n-delay)" !

	/*dpw += framesPerBuffer;
	dpr += framesPerBuffer;
	dpw %= buff.size();
	dpr %= buff.size();*/
	std::vector<double> parameters(3);
	if (props[3].getValue() == Echo)
		parameters = { 1,0,props[2].getValue() };	//BL, FB, FF
	else if (props[3].getValue() == Plane)
		parameters = { 1,props[1].getValue(),0 };	//BL, FB, FF
	else //AllPass
		parameters = {-props[1].getValue(), props[1].getValue(), 1};	//BL, FB, FF
	filter->combFilter(parameters[0], parameters[1], parameters[2], in, out, framesPerBuffer);
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
		if (i == 3) v = floor(v);	//Redondeo
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
