#include "EffectReverbLP.h"



EffectReverbLP::EffectReverbLP() : Effect(std::string("Reverb LP"))
{
	props = { Properties(std::string("Feedback"),0,1),Properties(std::string("Dry Wet"),0,1),Properties(std::string("Delay"),0,1024) };
	props[0].setValue(0.7);
	props[1].setValue(1);
	props[2].setValue(0.5);
	buff = std::vector<float>((unsigned)(props[2].getValue() * 44100.0 * 2.0), 0);
	dpw = 0; // As the buffer will be circular (else, infinite memory would be needed) we need a write pointer
	dpr = buff.size() / 2;
}

bool EffectReverbLP::next(const void * inputBuffer, void * outputBuffer, unsigned long framesPerBuffer)
{
	float *in = (float*)inputBuffer;
	float *out = (float*)outputBuffer;
	float a = 0.7; float b_0 = 0.3; float b_1 = 0.3;
	float x_hold = 0; float y_hold = 0; float yh = 0;
	for (unsigned long i = 0; i<framesPerBuffer; i++) //Every sample should be processed
	{
		yh = b_1 * x_hold - a * y_hold;
		y_hold = yh;
		x_hold = buff[(dpr + i) % buff.size()];
		out[i] = (float)(props[1].getValue() * *(in++)+props[0].getValue() * yh); //And is added to the current sample (with a coefficient) LINE A
		buff[(dpw + i) % buff.size()] = out[i]; // The output is saved (also with a coefficient) LINE B
	}
	for (unsigned i = 0; i < framesPerBuffer; i++)
	{
		out[2 * framesPerBuffer - 1 - 2 * i] = out[framesPerBuffer - 1 - i];
		out[2 * framesPerBuffer - 2 - 2 * i] = out[framesPerBuffer - 1 - i];
	}
	dpw += framesPerBuffer;
	dpr += framesPerBuffer;
	dpw %= buff.size();
	dpr %= buff.size();
	return true;
}

bool EffectReverbLP::setProp(unsigned i, double v)
{
	bool ret = false;
	if (i < props.size())
	{
		ret = props[i].setValue(v);
		if (i == 2) buff.resize(props[2].getValue() * 44100.0 * 2.0, 0);
	}
	return ret;
}

EffectReverbLP::~EffectReverbLP()
{
}
