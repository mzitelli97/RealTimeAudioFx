#include "EffectPhaser.h"
#include "LFO.h"

#define DEFAULT_DEPTH 1
#define DEFAULT_SWEEP 440 //In Hz (LFO amplitud)
#define DEFAULT_MIN_FREQ 220
//#define DEFAULT_FEEDBACK 0
#define DEFAULT_LFO_FREQ 0.5


EffectPhaser::EffectPhaser() : Effect(std::string("Phaser"))
{
	props = { Properties(std::string("Depth"),0,1),Properties(std::string("Sweep Width"),0,2000),/*Properties(std::string("Feedback"),0,0.99),*/
		Properties(std::string("Base Frequency"),0,1000), Properties(std::string("LFO Frequency"),0,10) };
	props[0].setValue(DEFAULT_DEPTH);
	props[1].setValue(DEFAULT_SWEEP);
	//props[2].setValue(DEFAULT_FEEDBACK);
	props[2].setValue(DEFAULT_MIN_FREQ);
	props[3].setValue(DEFAULT_LFO_FREQ);
	filterCount = 1;
	sampleCount = 0;
}

bool EffectPhaser::next(const void * inputBuffer, void * outputBuffer, unsigned long framesPerBuffer)
{
	float *in = (float*)inputBuffer;
	float *out = (float*)outputBuffer;
	float phase = 0;
	for (int i = 0; i < framesPerBuffer; i++)
	{
		phase = (sampleCount++) * props[3].getValue();
		if (phase >= sampleRate)
		{
			phase -= sampleRate;
			sampleCount = 0;
		}
		float filterSignal = in[i];

		//Recalculo los coeficientes del filtro para cada muestra, dado que cambia el delay
		notch.calculateCoeff(props[2].getValue() + props[1].getValue() * lfo(sampleRate, phase, Sine), sampleRate);
		
		//Aplico el filtro notch
		filterSignal = 2 * notch.processSingleSampleRaw(filterSignal);

		out[i] = in[i] - props[0].getValue() * filterSignal;
	}
	for (unsigned i = 0; i < framesPerBuffer; i++)
	{
		out[2 * framesPerBuffer - 1 - 2 * i] = out[framesPerBuffer - 1 - i];
		out[2 * framesPerBuffer - 2 - 2 * i] = out[framesPerBuffer - 1 - i];
	}

	return true;
}


EffectPhaser::~EffectPhaser()
{
}
