#include "Effect.h"



Effect::Effect(std::string s)
{
	name = s;
	sampleRate = DEFAULT_SAMPLE_RATE;
}

bool Effect::next(const void * inputBuffer, void * outputBuffer, unsigned long framesPerBuffer)
{
	memcpy(outputBuffer, inputBuffer, sizeof(float)*framesPerBuffer);
	return true;
}

std::string Effect::getName()
{
	return name;
}

std::vector<Properties> Effect::getProps()
{
	return props;
}

bool Effect::setProp(unsigned i, double v)
{
	if (i < props.size())
		return props[i].setValue(v);
	return false;
}

void Effect::setSampleRate(float sR)
{
	sampleRate = sR;
}


Effect::~Effect()
{
}
