#include "UniversalCombFilter.h"
#include <cmath>
#define PI 3.14159265

UniversalCombFilter::UniversalCombFilter()
{
	isLowPass = false;
	lowPassPole = 0;
	delay = 0;
	buff = std::vector<float>((unsigned)(2 * delay), 0);
	dpw = 0; // As the buffer will be circular (else, infinite memory would be needed) we need a write pointer
	dpr = buff.size() / 2;
}

bool UniversalCombFilter::combFilter(float BL, float FB, float FF, float * in, float * out, unsigned len)
{
	//Variables para aplicar el filtro pasa bajos
	float x_0 = 0, x_1 = 0, yh = 0;
	float a = lowPassPole, b = 0;
	float gain = (1 - a) / (1 + b);

	float xh = 0;
	for (unsigned long i = 0; i < len; i++) //Every sample should be processed
	{
		yh = gain * (x_0 + b * x_1) + a * yh;		//Difference equation y[n] = G * (x[n] + b * x[n-1]) + a * y[n-1]
		x_1 = x_0;
		x_0 = buff[(dpr + i) % buff.size()];

		if(isLowPass)
			xh = in[i] + FB * yh;
		else
			xh = in[i] + FB * x_0;
		out[i] = FF * x_0 + BL * xh; //And is added to the current sample (with a coefficient) LINE A
		buff[(dpw + i) % buff.size()] = xh; // The output is saved (also with a coefficient) LINE B
	}
	//To explain what this does in terms of digital systems and signals analysis, this "effect" has the following shape
	// y[n] = FeedForward * y[n-delay] + BL * (0.7*x[n] + Feddback * y[n-delay])

	//Pointer incrementation, considering the buffer is circular
	dpw += len;
	dpr += len;
	dpw %= buff.size();
	dpr %= buff.size();
	return true;
}

bool UniversalCombFilter::setDelay(unsigned int delay)
{
	this->delay = delay;
	buff.resize(delay * 2.0, 0);
	dpw = 0;
	dpr = buff.size() / 2;
	return true;
}

bool UniversalCombFilter::setLowPass(bool lowpass, float freq)
{
	this->isLowPass = lowpass;
	lowPassPole = cos(2 * PI * freq);
	return true;
}

UniversalCombFilter::~UniversalCombFilter()
{
}
