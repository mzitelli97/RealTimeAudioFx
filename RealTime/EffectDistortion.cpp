#include "EffectDistortion.h"

EffectDistortion::EffectDistortion() : Effect(std::string("Distortion"))
{
	props = { Properties(std::string("Threshold"),0,1), Properties(std::string("Gain"),0,1000), 
		Properties(std::string("Type [HardC SoftPol SoftExp HalfRect FullRect]"),0,Distortion_count-1) };
	props[0].setValue(THRESHOLD_DEFAULT);
	props[1].setValue(GAIN_DEFAULT);
	props[2].setValue(FullWaveRect);
	type = (Distortion_type)((int)props[2].getValue());
}

bool EffectDistortion::next(const void * inputBuffer, void * outputBuffer, unsigned long framesPerBuffer)
{
	float *in = (float*)inputBuffer;
	float *out = (float*)outputBuffer;
	for (unsigned long i = 0; i<framesPerBuffer; i++) //Every sample should be processed
	{
		float input = in[i] * props[1].getValue();
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
			if (input > 2.0/3.0)
				out[i] = 1;
			else if (input > 1.0/3.0)
				out[i] = 1.0 - pow(2 - 3 * (input), 2) / 3.0;
			else if (input < -2.0/3.0)
				out[i] = -1;
			else if (input < -1.0/3.0)
				out[i] = -1.0 + pow(2 + 3 * (input), 2) / 3.0;
			else
				out[i] = 2 * input;
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
			out[i] = 0.5*(abs(input) + input);
			break;
		default:
			out[i] = input;
			break;
		}
	}
	for (unsigned i = 0; i < framesPerBuffer; i++)
	{
		out[2 * framesPerBuffer - 1 - 2 * i] = out[framesPerBuffer - 1 - i];
		out[2 * framesPerBuffer - 2 - 2 * i] = out[framesPerBuffer - 1 - i];
	}

	return true;
}

bool EffectDistortion::setProp(unsigned i, double v)
{
	bool ret = false;
	if (i < props.size())
	{
		ret = props[i].setValue(v);
		if (i == 2 && ret)
		{
			int aux = (int)v;
			type = (Distortion_type)aux;
		}
	}

	return ret;
}

EffectDistortion::~EffectDistortion()
{
}
