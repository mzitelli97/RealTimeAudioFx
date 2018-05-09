#include "EffectPhaser.h"
#include "LFO.h"

#define DEFAULT_DEPTH 1
#define DEFAULT_SWEEP 440 //In Hz (LFO amplitud)
#define DEFAULT_MIN_FREQ 220
#define DEFAULT_FEEDBACK 0
#define DEFAULT_LFO_FREQ 0.5


EffectPhaser::EffectPhaser() : Effect(std::string("Phaser"))
{
	props = { Properties(std::string("Depth"),0,1),Properties(std::string("Sweep Width"),0,2000),Properties(std::string("Feedback"),0,0.99),
		Properties(std::string("Base Frequency"),0,1000), Properties(std::string("LFO Frequency"),0,10) };
	props[0].setValue(DEFAULT_DEPTH);
	props[1].setValue(DEFAULT_SWEEP);
	props[2].setValue(DEFAULT_FEEDBACK);
	props[3].setValue(DEFAULT_MIN_FREQ);
	props[4].setValue(DEFAULT_LFO_FREQ);
	lastFilterOutput = 0;
	filterCount = 1;
	filters = std::vector<AllPassFilter>(filterCount);
	sampleCount = 0;
	filterUpdateInt = 4;
}

bool EffectPhaser::next(const void * inputBuffer, void * outputBuffer, unsigned long framesPerBuffer)
{
	float *in = (float*)inputBuffer;
	float *out = (float*)outputBuffer;
	float phase = 0;
	for (int i = 0; i < framesPerBuffer; i++)
	{
		phase = (sampleCount++) * props[4].getValue();
		if (phase >= sampleRate)
		{
			phase -= sampleRate;
			sampleCount = 0;
		}
		float filterSignal = in[i];
		// If feedback is enabled, include the feedback from the
		// last sample in the input of the allpass filter chain.
		if (props[2].getValue() != 0)
			filterSignal += props[2].getValue() * lastFilterOutput;
		/*for (int j = 0; j < filterCount; j++)
		{
			// First, update the current allpass filter
			// coefficients depending on the parameter settings
			// and the LFO phase. Recalculating the filter
			// coefficients is much more expensive than
			// calculating a sample. Only update the
			// coefficients at a fraction of the sample rate;
			// since the LFO moves slowly, the difference won’t
			// generally be audible.
			//if (((sampleCount++) % filterUpdateInt) == 0)
				//filters[j].calculateCoeff(props[3].getValue() + props[1].getValue() * lfo(sampleRate,phase, Sine), sampleRate);
			//filterSignal = filters[j].processSingleSampleRaw(filterSignal);
		}*/
		notch.calculateCoeff(props[3].getValue() + props[1].getValue() * lfo(sampleRate, phase, Sine), sampleRate);
		filterSignal = notch.processSingleSampleRaw(filterSignal);
		lastFilterOutput = filterSignal;
		// Add the allpass signal to the output
		// depth = 0 --> input only ; depth = 1 --> evenly
		// balanced input and output
		//out[i] = (1.0 - 0.5*props[0].getValue()) * in[i] + 0.5*props[0].getValue()*filterSignal;
		out[i] = in[i] - filterSignal;
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
