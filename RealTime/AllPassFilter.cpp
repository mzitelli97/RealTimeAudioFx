#include "AllPassFilter.h"
#include <cmath>
#define PI 3.141592653589793238463

AllPassFilter::AllPassFilter()
{
	g = 1;
	x_1 = 0;
	y_1 = 0;
}

bool AllPassFilter::calculateCoeff(float centerFreq, float sampleFreq)
{
	float w0 = 2 * PI * centerFreq / sampleFreq;
	if (w0 >= PI) w0 = 0.99 * PI; //This is to avoid calling tan with PI/2 (tan(PI/2)=inf)
	const double tan_half_w0 = tan(0.5 * w0);

	g = (float)((1.0 - tan_half_w0) / (1.0 + tan_half_w0));		//Appling bilinear transformation to the allpass filter with the desired center freq
	return true;
}

float AllPassFilter::processSingleSampleRaw(const float sample)
{
	// Process one sample, storing the last input and output
	y_1 = (g * sample) - x_1 + (g * y_1);		//Difference equation y[n] = g * x[n] - x[n-1] + g * y[n-1]
	x_1 = sample;
	return y_1;
}


AllPassFilter::~AllPassFilter()
{
}
