#include "UniversalCombFilter.h"



UniversalCombFilter::UniversalCombFilter(bool lowPass)
{
	this->lowPassFeedback = lowPass;
	delay = 0;
	buff = std::vector<float>((unsigned)(2 * delay), 0);
	dpw = 0; // As the buffer will be circular (else, infinite memory would be needed) we need a write pointer
	dpr = buff.size() / 2;
}

bool UniversalCombFilter::combFilter(float BL, float FB, float FF, float * in, float * out, unsigned len)
{
	//esto implementa un filtro comb universal
	//float BL = -0.7; float FB = 0.7; float FF = 1;
	float temp = 0, yh = 0, xh = 0;
	for (unsigned long i = 0; i < len; i++) //Every sample should be processed
	{
		yh = 0.3 * temp - 0.7 * yh;		//Difference equation y[n] = b_0 * x[n] + b_1 * x[n-1] - a_1 * y[n-1]
		temp = buff[(dpr + i) % buff.size()]; //The older sample is retrieved
		if(lowPassFeedback)
			xh = 0.7 * in[i] + FB * yh;
		else
			xh = 0.7 * in[i] + FB * temp;
		out[i] = FF * temp + BL * xh; //And is added to the current sample (with a coefficient) LINE A
		buff[(dpw + i) % buff.size()] = xh; // The output is saved (also with a coefficient) LINE B
	}
	//To explain what this does in terms of digital systems and signals analysis, this "effect" has the following shape
	// y(n) = feedback_*y(n-delay_)+dry_wet_*x(n)

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

bool UniversalCombFilter::setLowPassFeedback(bool lowpass)
{
	this->lowPassFeedback = lowpass;
	return true;
}



UniversalCombFilter::~UniversalCombFilter()
{
}
