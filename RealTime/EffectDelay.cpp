#include "EffectDelay.h"
#include <iostream>


EffectDelay::EffectDelay() :Effect(std::string("Delay"))
{
	props = {Properties(std::string("Feedback"),0,1),Properties(std::string("Dry Wet"),0,1),Properties(std::string("Delay"),0,1024) };
	props[0].setValue(0.6);
	props[1].setValue(0.7);
	props[2].setValue(0.5);
	std::cout << "Setting size to " << (unsigned)(props[2].getValue() * 44100 * 2);
	buff = std::vector<float>((unsigned)(props[2].getValue() * 44100 * 2),0);
	dpw = 0; // As the buffer will be circular (else, infinite memory would be needed) we need a write pointer
	dpr = buff.size()/2;
}

bool EffectDelay::next(const void * inputBuffer, void * outputBuffer, unsigned long framesPerBuffer)
{
	//esto implementa un filtro comb universal
	float *in = (float*)inputBuffer;
	float *out = (float*)outputBuffer;
	float BL = 0.5; float FB = 0; float FF = 0.7;
	for (unsigned long i = 0; i<framesPerBuffer; i++) //Every sample should be processed
	{
		float temp = buff[(dpr + i) % buff.size()]; //The older sample is retrieved
													//std::cout << *out << '\n'; 
		float xh = (float)(props[1].getValue() * *(in++) + FB * temp);
		*out = FF * temp + BL * xh; //And is added to the current sample (with a coefficient) LINE A
		buff[(dpw + i) % buff.size()] = xh; // The output is saved (also with a coefficient) LINE B
		out++;
	}
	/*float *in = (float*)inputBuffer;
	float *out = (float*)outputBuffer;
	for (unsigned long i = 0; i<framesPerBuffer; i++) //Every sample should be processed
	{
		float temp = buff[(dpr+i)%buff.size()]; //The older sample is retrieved
									   //std::cout << *out << '\n'; 
		*out = (float) (props[1].getValue()* *(in++) + temp); //And is added to the current sample (with a coefficient) LINE A
		buff[(dpw + i) % buff.size()] =(float) props[0].getValue() * *(out++); // The output is saved (also with a coefficient) LINE B
	}*/

	//To explain what this does in terms of digital systems and signals analysis, this "effect" has the following shape
	// y(n) = feedback_*y(n-delay_)+dry_wet_*x(n)
	//The second term is easy to appreciate in LINE A, where the raw input is taken directly to the ouptut (attenuated by dry_wet_)
	//The first term of the equation appears as "temp" in LINE A. As you can see, in LINE B the output is stored with a coefficient (feedback_).
	//delay_ samples later temps adquires that stored valued, so it appears as "feedback_*y(n-delay)" !

	//WARNING: Be very careful with systems' stability. In this example, using feedback_ > 1 would increase the otput at each cycle
	//ending in saturation (and believe me, your PC's speakers don't like that too much) 

	//Pointer incrementation, considering the buffer is circular
	dpw += framesPerBuffer;
	dpr += framesPerBuffer;
	dpw %= buff.size();
	dpr %= buff.size();
	return true;
}


EffectDelay::~EffectDelay()
{
}
