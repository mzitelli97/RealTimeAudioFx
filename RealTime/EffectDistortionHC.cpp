#include "EffectDistortionHC.h"

EffectDistortionHC::EffectDistortionHC() : Effect(std::string("Distortion"))
{
	props = { Properties(std::string("Threshold1"),0,1),  Properties(std::string("Threshold2"),0,1), Properties(std::string("Gain"),0,10), Properties(std::string("Type"),0,Distortion_count) };
	props[0].setValue(1.0/3.0);
	props[1].setValue(2.0 / 3.0);
	props[2].setValue(2.0);
	props[3].setValue(FullWaveRect);
}

bool EffectDistortionHC::next(const void * inputBuffer, void * outputBuffer, unsigned long framesPerBuffer)
{
	float *in = (float*)inputBuffer;
	float *out = (float*)outputBuffer;
	for (unsigned long i = 0; i<framesPerBuffer; i++) //Every sample should be processed
	{
		float input = in[i] * props[2].getValue();
		switch (type)
		{
		case HardClipping:
			if (input > props[0].getValue())
				out[i] = props[0].getValue();
			else if (input < -props[0].getValue())
				out[i] = -props[0].getValue();
			else
				out[i] = input;
			break;
		case SoftClipping:
			if (input > props[1].getValue())
				out[i] = 1;
			else if (input > props[0].getValue())
				out[i] = 1.0 - pow(2 - 3 * (input), 2) / 3.0;
			else if (input < -props[1].getValue())
				out[i] = -1;
			else if (input < -props[0].getValue())
				out[i] = -1.0 + pow(2 - 3 * (input), 2) / 3.0;
			else
				out[i] = input;
			break;
		case ExpSoftClipping:
			if (input > 0)
				out[i] = 1.0 - expf(-input);
			else
				out[i] = -1.0 + expf(input);
			break;
		case FullWaveRect:
			out[i] = abs(input);
			break;
		case HalfWaveRect:
			out[i] = 0.5*(abs(*in) + *in);
			break;
		default:
			out[i] = input;
			break;
		}
		//HardClipping
		/*if (*in > props[0].getValue())
			out[i] = props[0].getValue();
		else if (*in < -props[0].getValue())
			out[i] = -props[0].getValue();
		else
			out[i] = *in;
		in++;*/
		//FullWaveRectifier
		//out[i] = abs(*(in++));
		
		//HalfWaveRectifier
		/*out[i] = 0.5*(abs(*in) + *in);
		in++;*/

		//SoftClipping
		/*if (*in > props[0].getValue())
		{
			if (*in > props[1].getValue()) // positive clipping
				*out = 1.0;
			else // soft knee (positive)
				*out = 1.0 - pow(2 - 3 * (*in), 2) / 3.0;
		}
		else
		{
			if (*in < - props[0].getValue())
			{
				if (*in < - props[1].getValue()) // negative clipping
					*out = -1.0f;
				else // soft knee (negative)
					*out = -1.0 + pow(2 - 3 * (*in), 2) / 3.0;
			}
			else // linear region (-1/3..1/3)
				*out *= 2.0;
		}
		in++;
		*(out++) /= 2.0; // divide all by 2 to compensate for extra 6 dB gain boost
		*/

		// Soft clipping based on exponential function
		/*if (input > 0)
			out[i] = 1.0 - expf(-input);
		else
			out[i] = -1.0 + expf(input);*/
	}
	for (unsigned i = 0; i < framesPerBuffer; i++)
	{
		out[2 * framesPerBuffer - 1 - 2 * i] = out[framesPerBuffer - 1 - i];
		out[2 * framesPerBuffer - 2 - 2 * i] = out[framesPerBuffer - 1 - i];
	}

	return true;
}

bool EffectDistortionHC::setProp(unsigned i, double v)
{
	bool ret = false;
	if (i < props.size())
	{
		ret = props[i].setValue(v);
		if (i == 3 && ret)
		{
			int aux = (int)v;
			type = (Distortion_type)aux;
		}
	}

	return ret;
}

EffectDistortionHC::~EffectDistortionHC()
{
}
