#include "NotchFilter2Order.h"
#include <cmath>
#define PI 3.141592653589793238463


NotchFilter2Order::NotchFilter2Order()
{
	a_0 = 1;
	a_1 = a_2 = b_0 = b_1 = b_2 = 0;
}

bool NotchFilter2Order::calculateCoeff(float centerFreq, float sampleFreq)
{
	float w0 = 2 * PI * centerFreq / sampleFreq;
	float deltaW = w0 / 1000.0;
	float R = 1 - deltaW / 2.0;
	a_0 = 1;
	a_1 = -2 * cos(w0);
	a_2 = 1;

	b_0 = 1;
	b_1 = -2 * R * cos(w0);
	b_2 = pow(R, 2);

	return true;
}

float NotchFilter2Order::processSingleSampleRaw(const float sample)
{
	// Process one sample, storing the previous input and output
	//Difference equation: y[n]-2cos(w0)y[n-1]+y[n-2]=x[n]-2*R*cos(w0)x[n-1]+R^2x[n-2]

	float y = b_0 * sample + b_1 * x_1 + b_2 * x_2 - a_1 * y_1 - a_2 * y_2;
	x_2 = x_1;
	x_1 = sample;
	y_2 = y_1;
	y_1 = y;
	return y;
}


NotchFilter2Order::~NotchFilter2Order()
{
}
