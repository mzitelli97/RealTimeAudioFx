#include "EffectDistortionHC.h"

EffectDistortionHC::EffectDistortionHC() : Effect(std::string("Distortion"))
{
	props = { Properties(std::string("Threshold1"),0,1),  Properties(std::string("Threshold2"),0,1) };
	props[0].setValue(1.0/3.0);
	props[1].setValue(2.0 / 3.0);
}

bool EffectDistortionHC::next(const void * inputBuffer, void * outputBuffer, unsigned long framesPerBuffer)
{
	float *in = (float*)inputBuffer;
	float *out = (float*)outputBuffer;
	for (unsigned long i = 0; i<framesPerBuffer; i++) //Every sample should be processed
	{
		//HardClipping
		/*if (*in > props[0].getValue())
			*(out++) = props[0].getValue();
		else if (*in < -props[0].getValue())
			*(out++) = -props[0].getValue();
		else
			*(out++) = *in;
		in++;*/
		//FullWaveRectifier
		//*(out++) = abs(*(in++));
		
		//HalfWaveRectifier
		/**(out++) = 0.5*(abs(*in) + *in);
		in++;*/

		//SoftClipping
		if (*in > props[0].getValue())
		{
			if (*in > props[1].getValue()) // positive clipping
			{
				*out = 1.0;
			}
			else // soft knee (positive)
			{
				*out = 1.0 - pow(2 - 3 * (*in), 2) / 3.0;
			}
		}
		else
		{
			if (*in < - props[0].getValue())
			{
				if (*in < - props[1].getValue()) // negative clipping
				{
					*out = -1.0f;
				}
				else // soft knee (negative)
				{
					*out = -1.0 + pow(2 - 3 * (*in), 2) / 3.0;
				}
			}
			else // linear region (-1/3..1/3)
			{
				*out *= 2.0;
			}
		}
		in++;
		*(out++) /= 2.0; // divide all by 2 to compensate for extra 6 dB gain boost
	}
	return true;
}


EffectDistortionHC::~EffectDistortionHC()
{
}
