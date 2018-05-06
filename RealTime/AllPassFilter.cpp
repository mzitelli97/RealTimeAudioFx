#include "AllPassFilter.h"



AllPassFilter::AllPassFilter()
{
}

bool AllPassFilter::calculateCoeff(float centerFreq, float sampleFreq)
{
	return true;
}

float AllPassFilter::processSingleSampleRaw(const float sample)
{
	// Process one sample, storing the last input and output
	y_1 = (b_0 * sample) + (b_1 * x_1) + (a_1 * y_1);		//Difference equation y[n] = b_0 * x[n] + b_1 * x[n-1] + a_1 * y[n-1]
	x_1 = sample;
	return y_1;
}


AllPassFilter::~AllPassFilter()
{
}
