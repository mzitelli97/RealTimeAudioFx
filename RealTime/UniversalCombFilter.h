#pragma once
#include <vector>

//Implementes an universal comb filter, with or without low pass filter in feedback
class UniversalCombFilter
{
public:
	UniversalCombFilter();
	bool combFilter(float BL, float FB, float FF, float* in, float* out, unsigned len);
	bool setDelay(unsigned int delay);
	bool setLowPass(bool lowpass, float freq);
	virtual ~UniversalCombFilter();
private:
	std::vector<float> buff;
	unsigned dpw, dpr;
	unsigned delay;
	bool isLowPass;
	float lowPassPole;
};

