#pragma once
#include <vector>

class UniversalCombFilter
{
public:
	UniversalCombFilter(bool lowPass = false);
	bool combFilter(float BL, float FB, float FF, float* in, float* out, unsigned len);
	bool setDelay(unsigned int delay);
	bool setLowPassFeedback(bool lowpass);
	virtual ~UniversalCombFilter();
private:
	std::vector<float> buff;
	unsigned dpw, dpr;
	unsigned delay;
	bool lowPassFeedback;
};
